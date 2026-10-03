#include "Widget.h"
#include "config.h"
#include "videoplayer.h"
#include <windows.h>
#include "network.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QtGlobal>
#include <QTime>

Widget::Widget(VideoPlayer* videoPlayer, QWidget *parent) :
    QWidget(parent),
    soundEngine(this),
    videoPlayer(videoPlayer),
    network(new Network(this))
{
    QScreen *screen = QGuiApplication::primaryScreen();
    setGeometry(screen->geometry());

    connect(network, &Network::doGameStarted, this, &Widget::receiveGameStarted);
    connect(network, &Network::shipSpeedChanged, this, &Widget::onShipSpeedChanged);
    connect(network, &Network::spawnEnemy, this, &Widget::spawnEnemyCommand);
    connect(network, &Network::despawnEnemy, this, &Widget::despawnEnemyCommand);
    connect(network, &Network::gameOver, this, &Widget::gameLose);
    connect(network, &Network::gameWon, this, &Widget::gameWin);
    connect(network, &Network::meteorAmountChanged, this, &Widget::onMeteorAmountChanged);
    connect(network, &Network::invalidCommandReceived, [](const QString& cmd) {
        qWarning() << "Ungültiger Befehl empfangen:" << cmd;
    });

    connect(network, &Network::roleAssigned, this, [](const QString& role) {
        qDebug() << "Role assigned:" << role;
    });

    network->connectToServer();

    setAttribute(Qt::WA_NoMousePropagation);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);

    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoFillBackground(false);

    setMouseTracking(false);
    setCursor(Qt::BlankCursor);

    textOpacity = 1.0;

    playerInit();

    companionImage = QPixmap(companionFilePath);

    enemyImage = QPixmap(enemyShipFilePath1);
    enemyImageSize = width() * 0.04;

    enemyBulletSize = width() * 0.008;

    menuImage = QPixmap(menuImagePath);
    winImage = QPixmap(winImagePath);
    loseImage = QPixmap(loseImagePath);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Widget::game);
    connect(timer, &QTimer::timeout, this, &Widget::movement);
    timer->start(33);

    connect(&hitTimer, &QTimer::timeout, this, [this]() {
        update();
    });

    //connect(&enemySpawnTimer, &QTimer::timeout, this, &Widget::spawnEnemy);

    connect(&enemyShootStartTimer, &QTimer::timeout, this, [this]() {
        enemyShootTimer.start(500);
        enemyShootStartTimer.stop();
    });

    connect(&enemyShootTimer, &QTimer::timeout, this, &Widget::enemyShoot);

    soundEngine.playBGM(menuMusicPath);

    textDisplayTimer = new QTimer(this);
    connect(textDisplayTimer, &QTimer::timeout, this, [this]() {refreshTitle = false; hideText();});

    // Burst-Timer
    connect(&burstTimer, &QTimer::timeout, this, [this]() {
        if (currentShootPattern == 0) {
            isBurstActive = true;
            enemyShootTimer.start(300);
            QTimer::singleShot(2000, this, [this]() {
                isBurstActive = false;
                enemyShootTimer.start(500);
            });
        }
    });

    // Wechsel des Schussmusters
    currentShootPattern = rand() % 3;
    connect(&patternChangeTimer, &QTimer::timeout, this, [this]() {
        currentShootPattern = rand() % 3;
    });
    patternChangeTimer.start(15000);
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, false);

    QColor backgroundColor = QColor(0, 0, 0, 1);
    painter.fillRect(rect(), backgroundColor);


    if (!gameStarted) {
        // Startbildschirm anzeigen
        painter.drawPixmap(rect(), menuImage, menuImage.rect());
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 50, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "Press ENTER to Start");
        return;
    }



    if (gameWon) {
        if (!pressedKeys.contains(Qt::Key_W)) {
            painter.drawPixmap(rect(), winImage, winImage.rect());
        }
        painter.setPen(Qt::green);
        painter.setFont(QFont("Arial", 50, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "YOU WIN");
        return;
    }

    if (gameLost) {
        painter.drawPixmap(rect(), loseImage, loseImage.rect());
        painter.setPen(Qt::red);
        painter.setFont(QFont("Arial", 50, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "YOU LOSE");
        return;
    }

    // Normales Spiel zeichnen
    if(gameStarted)
    {
        srand(static_cast<uint>(QTime::currentTime().msec()));

        QSize targetSize(50, 50);
        if (activateCompanion)
            painter.drawPixmap(companionPos , companionImage.scaled(targetSize, Qt::KeepAspectRatio));

        QRect ufoRect(playerRectX + (playerRectSize - playerImageSize) / 2,
                      playerRectY + (playerRectSize - playerImageSize * 0.6) / 2,
                      playerImageSize,
                      playerImageSize * 0.6);
        painter.drawPixmap(ufoRect, playerImage);

        if (pressedKeys.contains(Qt::Key_Shift)) {
            painter.setPen(Qt::red);
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(playerRectX - 0.45, playerRectY - 2.8, playerRectSize, playerRectSize);
        }

        // Asteroid und Geschosse zeichnen
        for (const auto &circle : circles) {
            if (circle.isEnemyBullet) {
                QPixmap bulletImage;
                if (circle.color == Qt::red) {
                    bulletImage = QPixmap(BulletRedFilePath);
                } else if (circle.color == Qt::blue) {
                    bulletImage = QPixmap(BulletBlueFilePath);
                } else if (circle.color == Qt::yellow) {
                    bulletImage = QPixmap(BulletYellowFilePath);
                } else if (circle.color == Qt::green) {
                    bulletImage = QPixmap(BulletGreenFilePath);
                }

                QPixmap scaledBullet = bulletImage.scaled(
                    circle.radius * 2, circle.radius * 2,
                    Qt::KeepAspectRatio, Qt::SmoothTransformation
                    );
                QRect bulletRect(circle.pos.x() - circle.radius, circle.pos.y() - circle.radius,
                                 circle.radius * 2, circle.radius * 2);
                painter.drawPixmap(bulletRect, scaledBullet);

            } else {
                // 1. Lade das entsprechende Asteroiden-Bild basierend auf der Farbe
                QPixmap asteroidImage;
                if (circle.color == Qt::red) {
                    asteroidImage = QPixmap(redMeteorite);
                } else if (circle.color == Qt::blue) {
                    asteroidImage = QPixmap(blueMeteorite);
                } else if (circle.color == Qt::yellow) {
                    asteroidImage = QPixmap(yellowMeteorite);
                } else if (circle.color == Qt::green) {
                    asteroidImage = QPixmap(greenMeteorite);
                }

                // 2. Skaliere das Bild auf die richtige Größe
                QPixmap scaledAsteroid = asteroidImage.scaled(
                    circle.radius * 2, circle.radius * 2,
                    Qt::KeepAspectRatio, Qt::SmoothTransformation
                    );

                // 3. Erstelle eine rotierte Version des Bildes
                QPixmap rotatedAsteroid = scaledAsteroid.transformed(
                    QTransform().rotate(circle.rotationAngle),
                    Qt::SmoothTransformation
                    );

                // 4. Zeichne das rotierte Bild zentriert an der Asteroiden-Position
                painter.drawPixmap(
                    circle.pos.x() - rotatedAsteroid.width()/2,
                    circle.pos.y() - rotatedAsteroid.height()/2,
                    rotatedAsteroid
                    );
            }
        }

        if (enemyUfo.active) {
            QRect enemyRect(enemyUfo.pos.x(), enemyUfo.pos.y(), enemyImageSize, enemyImageSize * 0.6);
            painter.drawPixmap(enemyRect, enemyImage);
        }

        // Radio
        if (showText) {
            painter.setPen(QColor(26, 246, 180));
            painter.setFont(QFont("Consolas", 15, QFont::Bold));
            QString text = soundEngine.musicTitle;
            QRect textRect = QRect(0, height() - 45, width(), 50);

            painter.setOpacity(textOpacity);

            painter.drawText(textRect, Qt::AlignCenter, text);
        }

        // Controls-Hilfe
        if (tabHeld)
        {
            showText = false;

            // Verdunkelt Hintergrund
            painter.setBrush(QColor(0, 0, 0, 190));
            painter.drawRect(0, 0, width(), height());
            // Text
            painter.setPen(QColor(26, 246, 180));
            painter.setFont(QFont("Consolas", 15, QFont::Bold));
            QString text = "---- CONTROLS ----"
                           "\n"
                           "8 - Gegner einfügen\n"
                           "M - Playlist1: Musik ändern\n"
                           "N - Playlist2:Musik ändern\n"
                           "J - Playlist3:Musik ändern\n"
                           "B - BG Musik anmachen\n"
                           "K - BG Video ändern\n"
                           "L - Matthias umbringen/wiederbeleben\n"
                           "Left Arrow/Right Arrow - Matthias Position ändern\n"
                           "P - BG Video ändern: Special\n"
                           "Shift - Hitbox anzeigen lassen\n"
                           "ESC - Spiel beenden\n";

            QRect textRect = QRect(0, 0, height(), width());

            painter.drawText(textRect, text);
        }
    }
}

void Widget::spawnEnemy() {
    if (!gameStarted || enemyUfo.active) return;
    spawnEnemyCommand();
}

void Widget::enemyShoot() {
    if (!enemyUfo.active) {
        return;
    }

    const float radius = playerRectSize * 0.2;

    struct BulletType {
        Qt::GlobalColor color;
        const char* imagePath;
    };

    const BulletType bulletTypes[] = {
        {Qt::blue, BulletBlueFilePath},
        {Qt::red, BulletRedFilePath},
        {Qt::yellow, BulletYellowFilePath},
        {Qt::green, BulletGreenFilePath}
    };

    float bulletSpeedNormal = height() * 0.003;
    float bulletSpeedBurst = height() * 0.017;

    soundEngine.playSound(shootSoundPath, 0.04f);

    static float spiralAngleOffset = 0.0f;

    switch (currentShootPattern) {
    case 0: {
        int bulletCount = 18;
        float angleStep = 2 * M_PI / bulletCount;

        for (int i = 0; i < bulletCount; ++i) {
            float angle = spiralAngleOffset + i * angleStep;
            float speed = isBurstActive ? bulletSpeedBurst : bulletSpeedNormal;

            const BulletType& bulletType = bulletTypes[rand() % 4];
            QPixmap bulletPixmap(bulletType.imagePath);

            Circle bullet;
            bullet.pos = QPointF(
                enemyUfo.pos.x() + enemyImageSize / 2 + radius * cos(angle),
                enemyUfo.pos.y() + enemyImageSize / 2 + radius * sin(angle)
                );
            bullet.direction = QPointF(cos(angle), sin(angle));
            bullet.speed = speed;
            bullet.radius = enemyBulletSize;
            bullet.color = bulletType.color;
            bullet.isEnemyBullet = true;
            bullet.bulletImage = bulletPixmap;

            circles.push_back(bullet);
        }
        spiralAngleOffset += 0.1f;
        break;
    }

    case 1: {
        int bulletCount = 10;
        float angleStep = 2 * M_PI / bulletCount;
        static float rotationAngle = 0.0f;

        for (int i = 0; i < bulletCount; ++i) {
            float angle = rotationAngle + i * angleStep;

            const BulletType& bulletType = bulletTypes[rand() % 4];
            QPixmap bulletPixmap(bulletType.imagePath);

            Circle bullet;
            bullet.pos = QPointF(
                enemyUfo.pos.x() + enemyImageSize / 2 + radius * cos(angle),
                enemyUfo.pos.y() + enemyImageSize / 2 + radius * sin(angle)
                );
            bullet.direction = QPointF(cos(angle), sin(angle));
            bullet.speed = bulletSpeedNormal * 7;
            bullet.radius = enemyBulletSize;
            bullet.color = bulletType.color;
            bullet.isEnemyBullet = true;
            bullet.bulletImage = bulletPixmap;

            circles.push_back(bullet);
        }

        rotationAngle -= 0.1f;
        break;
    }

    case 2: {
        static QElapsedTimer bigShotTimer;
        if (!bigShotTimer.isValid() || bigShotTimer.hasExpired(1000)) {
            for (int i = 0; i < 2; i++) {
                float directionX = (i == 0) ? -0.8f : 0.8f;
                float directionY = 0.8f;

                const BulletType& bulletType = bulletTypes[rand() % 4];
                QPixmap bulletPixmap(bulletType.imagePath);

                Circle bullet;
                bullet.pos = QPointF(
                    enemyUfo.pos.x() + enemyImageSize / 2,
                    enemyUfo.pos.y() + enemyImageSize
                    );
                bullet.direction = QPointF(directionX, directionY);
                bullet.speed = height() * 0.005;
                bullet.radius = enemyBulletSize * 3;
                bullet.color = bulletType.color;
                bullet.isEnemyBullet = true;
                bullet.bulletImage = bulletPixmap;
                bullet.isDelayedShot = true;
                bullet.delayTimer.start();

                circles.push_back(bullet);
            }

            soundEngine.playSound(shootSoundPath, 0.04f);

            if (bigShotTimer.isValid()) {
                bigShotTimer.restart();
            } else {
                bigShotTimer.start();
            }
        }

        for (auto& circle : circles) {
            if (circle.isDelayedShot && circle.delayTimer.hasExpired(1250)) {
                QPointF playerCenter(playerRectX + playerRectSize / 2,
                                     playerRectY + playerRectSize / 2);
                QPointF direction = playerCenter - circle.pos;
                float length = sqrt(direction.x() * direction.x() + direction.y() * direction.y());
                if (length > 0) {
                    direction /= length;
                    circle.direction = direction;
                    circle.speed = height() * 0.02;
                    circle.isDelayedShot = false;
                }
            }
        }
        break;
    }
    }
    update();
}

void Widget::keyReleaseEvent(QKeyEvent *event) {
    pressedKeys.remove(event->key());

    if (event->key() == Qt::Key_Tab) {
        tabHeld = false;
    }
}

void Widget::playerInit()
{
    playerRectSize = width() * playerSizeMultiplier;
    playerRectX = (width() - playerRectSize) / 2;
    playerRectY = height() * playerStartRectY;
    playerSpeed = width() * playerSpeedMultiplier;
    playerImageSize = width() * 0.04;
    playerImage = QPixmap(playerShipFilePath);
}

void Widget::hideText() {
    if (!refreshTitle) {
        if (fadeOutAnimation) {
            fadeOutAnimation->stop();
            fadeOutAnimation->deleteLater();
        }

        fadeOutAnimation = new QPropertyAnimation(this, "textOpacity");
        fadeOutAnimation->setDuration(1000);
        fadeOutAnimation->setStartValue(1.0);
        fadeOutAnimation->setEndValue(0.0);

        connect(fadeOutAnimation, &QPropertyAnimation::finished, this, [this]() {
            showText = false;
            fadeOutAnimation->deleteLater();
            fadeOutAnimation = nullptr;
            update();
        });

        fadeOutAnimation->start();
    } else {
        textOpacity = 1.0;
    }
}

void Widget::gameWin() {
    gameWon = true;
    gameLost = false;

    videoPlayer->stopVideo();

    soundEngine.playBGM(winMusicPath);

    timer->stop();
    //enemySpawnTimer.stop();
    enemyShootTimer.stop();
    enemyShootStartTimer.stop();
    burstTimer.stop();

    enemyUfo.active = false;
    enemyUfo.pos = QPointF(width() / 2 - enemyImageSize / 2, -enemyImageSize);
    enemyUfo.targetY = height() * 0.2;
    enemyUfo.isMovingDown = true;
    enemyUfo.swayOffset = 0.0f;

    circles.erase(std::remove_if(circles.begin(), circles.end(),
                                 [](const Circle& circle) {
                                     return circle.isEnemyBullet;
                                 }),
                  circles.end());

    winImage = QPixmap(winImagePath);

    update();
}

void Widget::gameLose() {
    gameWon = false;
    gameLost = true;

    videoPlayer->stopVideo();

    soundEngine.playBGM(loseMusicPath);

    timer->stop();
    enemySpawnTimer.stop();
    enemyShootTimer.stop();
    enemyShootStartTimer.stop();
    burstTimer.stop();

    enemyUfo.active = false;
    enemyUfo.pos = QPointF(width() / 2 - enemyImageSize / 2, -enemyImageSize);
    enemyUfo.targetY = height() * 0.2;
    enemyUfo.isMovingDown = true;
    enemyUfo.swayOffset = 0.0f;

    circles.erase(std::remove_if(circles.begin(), circles.end(),
                                 [](const Circle& circle) {
                                     return circle.isEnemyBullet;
                                 }),
                  circles.end());

    update();
}

void Widget::gameStart() {
    gameStarted = true;
    gameWon = false;
    gameLost = false;

    soundEngine.stopBGM();

    timer->start(16);
    //enemySpawnTimer.start(3000);
    burstTimer.start(10000);

    update();
}

void Widget::resetGame() {
    gameStarted = false;
    gameWon = false;
    gameLost = false;

    circles.clear();

    playerRectX = (width() - playerRectSize) / 2;
    playerRectY = height() * playerStartRectY;

    videoPlayer->stopVideo();

    soundEngine.playBGM(menuMusicPath);

    //enemySpawnTimer.stop();
    enemyShootTimer.stop();
    enemyShootStartTimer.stop();
    burstTimer.stop();

    enemyUfo.active = false;
    enemyUfo.pos = QPointF(width() / 2 - enemyImageSize / 2, -enemyImageSize);
    enemyUfo.targetY = height() * 0.2;
    enemyUfo.isMovingDown = true;
    enemyUfo.swayOffset = 0.0f;

    circles.erase(std::remove_if(circles.begin(), circles.end(),
                                 [](const Circle& circle) {
                                     return circle.isEnemyBullet;
                                 }),
                  circles.end());

    winImage = QPixmap(winImagePath);

    update();
}

void Widget::setGameStarted(bool newGameStarted)
{
    gameStarted = newGameStarted;
}

bool Widget::getGameStarted() const
{
    return gameStarted;
}

void Widget::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void Widget::mouseMoveEvent(QMouseEvent *event) {
    event->ignore();
}

void Widget::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void Widget::onMeteorAmountChanged(int amount) {
    currentMeteorAmount = amount;
    qDebug() << "Meteor amount changed to:" << amount;

    // Berechnen Sie die Spawn-Wahrscheinlichkeit basierend auf dem Wert (0-100)
    // Je höher der Wert, desto höher die Spawn-Wahrscheinlichkeit
    meteorSpawnProbability = qMax(25, 100 - currentMeteorAmount);
}

void Widget::receiveGameStarted()
{
    soundEngine.playSound(enterSoundPath);

    int randomValue = rand() % 100;
    if (randomValue < 0) {
        videoPlayer->playPeak();
    } else {
        videoPlayer->playVideo();
    }

    gameStart();

    showText = false;
}

void Widget::onShipSpeedChanged(float current, float max) {
    if (max > 0) {
        currentSpeedFactor = current / max;
        qDebug() << "Ship speed changed to factor:" << currentSpeedFactor;
    }
}

void Widget::spawnEnemyCommand() {
    if (!enemyUfo.active && gameStarted) {
        handleEnemySpawn(true);
    }
}

void Widget::despawnEnemyCommand() {
    if (enemyUfo.active) {
        handleEnemySpawn(false);
    }
}

void Widget::handleEnemySpawn(bool spawn) {
    if (spawn) {
        enemyUfo.pos = QPointF(width() / 2 - enemyImageSize / 2, -enemyImageSize);
        enemyUfo.targetY = height() * 0.2;
        enemyUfo.active = true;
        enemyUfo.isMovingDown = true;

        // Clear existing enemy bullets
        circles.erase(std::remove_if(circles.begin(), circles.end(),
                                     [](const Circle& circle) { return circle.isEnemyBullet; }),
                      circles.end());

        enemyShootStartTimer.start(1000);
    } else {
        enemyUfo.active = false;
        enemyShootTimer.stop();
        enemyShootStartTimer.stop();

        // Clear existing enemy bullets
        circles.erase(std::remove_if(circles.begin(), circles.end(),
                                     [](const Circle& circle) { return circle.isEnemyBullet; }),
                      circles.end());
    }
    update();
}

void Widget::hitAsteroid() {
    if (!gameStarted) return;

    QPointF playerCenter(playerRectX - 0.45 + playerRectSize / 2, playerRectY - 2.8 + playerRectSize / 2);
    float playerRadius = playerRectSize / 2;

    for (auto it = circles.begin(); it != circles.end(); ) {
        float dx = playerCenter.x() - it->pos.x();
        float dy = playerCenter.y() - it->pos.y();
        float distance = sqrt(dx * dx + dy * dy);
        float meteorRadius = it->radius;

        if (distance < (playerRadius + meteorRadius)) {
            soundEngine.playSound(hitSoundPath, 0.05f);

            QString color;
            QString colorMessage;

            if (it->color == Qt::blue) {
                color = "blue";
                colorMessage = "Getroffener Asteroid: blau";
                //qDebug() << colorMessage;
            } else if (it->color == Qt::red) {
                color = "red";
                colorMessage = "Getroffener Asteroid: rot";
                //qDebug() << colorMessage;
            } else if (it->color == Qt::yellow) {
                color = "yellow";
                colorMessage = "Getroffener Asteroid: gelb";
                //qDebug() << colorMessage;
            } else if (it->color == Qt::green) {
                color = "green";
                colorMessage = "Getroffener Asteroid: grün";
                //qDebug() << colorMessage;
            }

            if (network) {
                QString command = QString("#hitmeteor:%1").arg(color);
                network->sendCommand(command);
            }

            it = circles.erase(it);
        } else {
            ++it;
        }
    }
}

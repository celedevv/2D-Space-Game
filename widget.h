#ifndef WIDGET_H
#define WIDGET_H

#include <cstdlib>
#include <ctime>
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QPointer>
#include <QSet>
#include <QTimer>
#include <QTextLayout>
#include <QTextLine>

#include <QWidget>
#include <vector>
#include <QVBoxLayout>
#include <QPalette>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QDynamicPropertyChangeEvent>
#include <QSize>

#include "game_engine.h"
#include "sound_engine.h"
#include "videoplayer.h"
#include "network.h"

struct Circle {
    QPointF pos;
    QPointF direction;
    float radius;
    float speed;
    QColor color;
    bool isEnemyBullet = false;
    bool isDelayedShot = false;
    QElapsedTimer delayTimer;
    QPixmap bulletImage;

    float rotationAngle = 0.0f;

    float rotationSpeed;
    bool rotateClockwise;
};

struct EnemyUFO {
    bool active = false;
    QPointF pos;
    float targetY;
    bool isMovingDown = true;
    bool isDespawning = false;
    float swayOffset = 0.0f;
    float despawnSpeed = 0.0f;
};

class Widget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal textOpacity READ getTextOpacity WRITE setTextOpacity NOTIFY textOpacityChanged)

private:
    game_engine gameEngine;
    sound_engine soundEngine;

    Network* network;

    int currentMeteorAmount = 50;

    void playerInit();
    void hideText();
    void onShipSpeedChanged(float current, float max);

    float currentSpeedFactor = 1.0f;
    float playerRectX, playerRectY, playerRectSize;
    float playerSpeed;
    float playerImageSize;
    float enemyImageSize;
    float enemyBulletSize;
    std::vector<Circle> circles;
    QTimer *textDisplayTimer;
    QTimer *timer;
    QTimer enemySpawnTimer;
    QTimer enemyShootStartTimer;
    QTimer enemyShootTimer;
    QSet<int> pressedKeys;
    QPixmap playerImage;
    QPixmap enemyImage;
    QPixmap enemyBullet;
    QPixmap menuImage;
    QPixmap winImage;
    QPixmap loseImage;
    QPixmap companionImage;

    bool showHitMessage = false;
    QTimer hitTimer;

    EnemyUFO enemyUfo;

    VideoPlayer* videoPlayer;

    qreal textOpacity = 1.0;

    QPointer<QPropertyAnimation> fadeOutAnimation;

    QTimer burstTimer;
    bool isBurstActive = false;

    int currentShootPattern = 0;
    QTimer patternChangeTimer;

    int meteorSpawnProbability = 10;

    int newRotationAngle;

    void handleEnemySpawn(bool spawn);

public slots:
    void spawnEnemyCommand();
    void despawnEnemyCommand();

public:
    explicit Widget(VideoPlayer* videoPlayer, QWidget *parent = nullptr);

    void hitAsteroid();

    qreal getTextOpacity() const { return textOpacity; }
    void setTextOpacity(qreal opacity) { textOpacity = opacity; update(); emit textOpacityChanged(); }

    bool getGameStarted() const;

    bool showText = true;
    bool refreshTitle = false;
    void hitEnemyBullet();

    QPointF companionPos;
    float companionOffset = 0.0;
    float matthiasOffsetX = 4;
    float matthiasOffsetY = 0;
    bool activateCompanion = true;

    void setGameStarted(bool newGameStarted);

signals:
    void textOpacityChanged();

private slots:
    void movement();
    void spawnEnemy();
    void enemyShoot();
    void onMeteorAmountChanged(int amount);
    void receiveGameStarted();

protected:
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void game();
    void gameWin();
    void gameLose();
    void gameStart();
    void resetGame();
    bool gameStarted = false;
    bool gameWon = false;
    bool gameLost = false;
    bool tabHeld = false;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // WIDGET_H

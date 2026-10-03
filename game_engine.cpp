#include "game_engine.h"
#include "widget.h"
#include "network.h"
#include <QDebug>
#include <config.h>
#include <QtGlobal>

game_engine::game_engine() {}

float game_engine::getRandSpeed()
{
    return randSpeed();

}

QPointF game_engine::getRandDirection()
{
    return randDirection();
}

float game_engine::randSpeed()
{
    return static_cast<float>(rand() % 11 + 5) / 1000; //Zufälliger Float von 0.5% bis 1.5%

}

QPointF game_engine::randDirection()
{
        float randNum = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.6f;  // Zufallszahl zwischen 0.2 und 0.8


        float dirY = 1.0f - randNum;
        return QPointF(randNum,dirY);
}

void Widget::movement() {
    // Player
    if (!gameStarted) return;
    float baseSpeed = playerSpeed * currentSpeedFactor;
    if (pressedKeys.contains(Qt::Key_Shift)) {
        baseSpeed *= 0.6;
    }
    if (pressedKeys.contains(Qt::Key_A)) {
        playerRectX = std::max(0.0f, playerRectX - baseSpeed);
    }
    if (pressedKeys.contains(Qt::Key_D)) {
        playerRectX = std::min(width() - playerRectSize, playerRectX + baseSpeed);
    }
    if (pressedKeys.contains(Qt::Key_W)) {
        playerRectY = std::max(0.0f, playerRectY - baseSpeed);
    }
    if (pressedKeys.contains(Qt::Key_S)) {
        playerRectY = std::min(height() - playerRectSize, playerRectY + baseSpeed);
    }

    // Matthias
    if (pressedKeys.contains(Qt::Key_Right)) {
        matthiasOffsetX = 3;
    } else if (pressedKeys.contains(Qt::Key_Left)) {
        matthiasOffsetX = -5.5;
    }

    if(activateCompanion)
    {
        // Bewegung des Begleiters mit Verzögerung
        float lerpFactor = 0.1; // Bestimmt, wie stark der Ball folgt
        companionPos.setX(companionPos.x() + matthiasOffsetX + (playerRectX - companionPos.x()) * lerpFactor);
        companionPos.setY(companionPos.y() + (playerRectY - companionPos.y()) * lerpFactor);

        // Wellenbewegung
        companionOffset += 0.1;
        companionPos.setY(companionPos.y() + sin(companionOffset) * 2);

        update();
    }
    else
    {
        companionPos.setX(width() / 2);
        companionPos.setY(height() / 2);
    }

    // Gegner Bewegung
    if (enemyUfo.active) {
        if (enemyUfo.isMovingDown) {
            // Move down to target position
            enemyUfo.pos.setY(enemyUfo.pos.y() + height() * 0.005);

            if (enemyUfo.pos.y() >= enemyUfo.targetY) {
                enemyUfo.isMovingDown = false;
            }
        }
        else if (enemyUfo.isDespawning) {
            // Move up to despawn
            enemyUfo.pos.setY(enemyUfo.pos.y() - enemyUfo.despawnSpeed);

            // Check if fully despawned
            if (enemyUfo.pos.y() <= enemyUfo.targetY) {
                enemyUfo.active = false;
                enemyUfo.isDespawning = false;
            }
        }
        else {
            // Sway movement when in position
            enemyUfo.swayOffset += 0.02f;
            enemyUfo.pos.setX(width() / 2 - enemyImageSize / 2 + 50 * sin(enemyUfo.swayOffset));
        }
    }
}

void Widget::game() {
    if (!gameStarted) return;

    // Spawn-Logik mit dynamischer Wahrscheinlichkeit
    if (currentMeteorAmount > 0 && rand() % meteorSpawnProbability == 0) {
        Circle c;
        QPointF circlePos;

        int spawnSide = rand() % 3;
        circlePos = QPointF(rand() % width(), 0);

        // Rest des Codes bleibt gleich...
        c.rotationSpeed = static_cast<float>(rand() % 26 + 5) / 10.0f;
        c.rotateClockwise = rand() % 2 == 0;
        c.rotationAngle = static_cast<float>(rand() % 360);
        c.pos = circlePos;
        c.radius = (rand() % 2 + 2) * (width() * 0.01);
        c.speed = (height() * gameEngine.getRandSpeed());

        QPointF randDirection = gameEngine.getRandDirection();
        if (spawnSide == 1) {
            randDirection.setX(randDirection.x()*-1);
            c.direction = QPointF(randDirection.x(),randDirection.y());
        } else if (spawnSide == 2) {
            c.direction = QPointF(randDirection);
        } else {
            c.direction = QPointF(0,1);
        }

        Qt::GlobalColor colors[] = {Qt::blue, Qt::red, Qt::yellow, Qt::green};
        c.color = colors[rand() % 4];

        circles.push_back(c);
    }

    // Asteroid und EnemyBullet
    for (auto it = circles.begin(); it != circles.end(); ) {
        it->pos += it->direction * it->speed;

        // Rotation aktualisieren
        if (!it->isEnemyBullet) {
            if (it->rotateClockwise) {
                it->rotationAngle += it->rotationSpeed;
            } else {
                it->rotationAngle -= it->rotationSpeed;
            }

            // Winkel auf 0-360° begrenzen
            if (it->rotationAngle > 360.0f) {
                it->rotationAngle -= 360.0f;
            } else if (it->rotationAngle < 0.0f) {
                it->rotationAngle += 360.0f;
            }
        }

        // EnemyBullet
        if (it->isEnemyBullet) {
            bool isOffScreen = it->pos.x() + it->radius < 0 ||
                               it->pos.x() - it->radius > width() ||
                               it->pos.y() + it->radius < 0 ||
                               it->pos.y() - it->radius > height();
            if (isOffScreen) {
                it = circles.erase(it);
            } else {
                ++it;
            }
        } else {
            // Asteroid
            if (it->pos.y() > height() || it->pos.x() < 0 || it->pos.x() > width()) {
                it = circles.erase(it);
            } else {
                ++it;
                newRotationAngle = rand() % 360;
            }
        }
    }

    hitAsteroid();

    update();
}

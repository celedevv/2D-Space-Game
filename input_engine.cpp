#include "input_engine.h"
#include "widget.h"

void Widget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        QApplication::quit();
    }


    if (event->key() == Qt::Key_M && gameStarted && !tabHeld) {
        if (textDisplayTimer->isActive()) {
            textDisplayTimer->stop();
            refreshTitle = true;

            if (fadeOutAnimation && fadeOutAnimation->state() != QAbstractAnimation::Stopped) {
                fadeOutAnimation->stop(); // Stoppe die Animation
                fadeOutAnimation->deleteLater();  // Lösche die alte Animation
                fadeOutAnimation = nullptr;
            }

            textOpacity = 1.0;
        }

        showText = true;
        textDisplayTimer->start(3000);

        soundEngine.incrimentBGM1();
        videoPlayer->stopMusic();
    }
    if (event->key() == Qt::Key_N && gameStarted && !tabHeld) {
        if (textDisplayTimer->isActive()) {
            textDisplayTimer->stop();
            refreshTitle = true;

            if (fadeOutAnimation && fadeOutAnimation->state() != QAbstractAnimation::Stopped) {
                fadeOutAnimation->stop(); // Stoppe die Animation
                fadeOutAnimation->deleteLater();  // Lösche die alte Animation
                fadeOutAnimation = nullptr;
            }

            textOpacity = 1.0;
        }

        showText = true;
        textDisplayTimer->start(3000);

        soundEngine.incrimentBGM2();
        videoPlayer->stopMusic();
    }
    if (event->key() == Qt::Key_8) {
        // Reset shoot timers
        enemyShootTimer.stop();
        enemyShootStartTimer.start(1500);

        // Reset sway movement
        enemyUfo.swayOffset = 0.0f;

        spawnEnemyCommand();
    } else if (event->key() == Qt::Key_9) {
        // Reset shoot timers
        enemyShootTimer.stop();
        enemyShootStartTimer.stop();

        // Reset sway movement
        enemyUfo.swayOffset = 0.0f;

        despawnEnemyCommand();
    }
    if (event->key() == Qt::Key_J && gameStarted && !tabHeld) {
        if (textDisplayTimer->isActive()) {
            textDisplayTimer->stop();
            refreshTitle = true;

            if (fadeOutAnimation && fadeOutAnimation->state() != QAbstractAnimation::Stopped) {
                fadeOutAnimation->stop(); // Stoppe die Animation
                fadeOutAnimation->deleteLater();  // Lösche die alte Animation
                fadeOutAnimation = nullptr;
            }

            textOpacity = 1.0;
        }

        showText = true;
        textDisplayTimer->start(3000);

        soundEngine.incrimentBGM3();
        videoPlayer->stopMusic();
    }
    if (event->key() == Qt::Key_B && gameStarted && !tabHeld) {
        videoPlayer->playMusic();
        soundEngine.stopBGM();

        showText = false;
    }

    if (event->key() == Qt::Key_P && gameStarted && !tabHeld) {
        videoPlayer->stopVideo();
        videoPlayer->playPeak();
        videoPlayer->playMusic();
        soundEngine.stopBGM();

        showText = false;
    }
    if (event->key() == Qt::Key_K && gameStarted && !tabHeld ) {
        videoPlayer->stopVideo();
        videoPlayer->playVideo();
        videoPlayer->playMusic();
        soundEngine.stopBGM();

        showText = false;
    }
    if (event->key() == Qt::Key_L && gameStarted && !tabHeld) {
        if(activateCompanion)
            activateCompanion = false;
        else
            activateCompanion = true;
    }
    if (event->key() == Qt::Key_C && gameStarted && !tabHeld) {
        soundEngine.playSound(cartiSoundPath, 10.0f);
    }
    if (event->key() == Qt::Key_X && gameStarted && !tabHeld) {
        soundEngine.playSound(swampSoundPath, 10.0f);
    }
    if (event->key() == Qt::Key_1 && gameStarted) {
        gameWin();
    }
    if (event->key() == Qt::Key_2 && gameStarted) {
        gameLose();
    }
    if (event->key() == Qt::Key_3 && gameStarted) {
        resetGame();
    }
    if (event->key() == Qt::Key_Tab) {
        if (textDisplayTimer->isActive()) {
            textDisplayTimer->stop();

            if (fadeOutAnimation && fadeOutAnimation->state() != QAbstractAnimation::Stopped) {
                fadeOutAnimation->stop(); // Stoppe die Animation
                fadeOutAnimation->deleteLater();  // Lösche die alte Animation
                fadeOutAnimation = nullptr;
            }

            textOpacity = 1.0;
        }
        if (!tabHeld) {
            tabHeld = true;
        }
    }
    if (event->key() == Qt::Key_Return && !gameStarted) {
        soundEngine.playSound(enterSoundPath);

        int randomValue = rand() % 100;
        if (randomValue < 1) {
            videoPlayer->playPeak();
        } else {
            videoPlayer->playVideo();
        }

        gameStart();

        showText = false;
    }
    if (event->key() == Qt::Key_V)
    {
        if (enemyImage.cacheKey() == QPixmap(enemyShipFilePath3).cacheKey()) {
            enemyImage = QPixmap(enemyShipFilePath1);
            enemyImageSize = width() * 0.04;
            soundEngine.playSound(lash2SoundPath, 20.0f);
        }
        else if (enemyImage.cacheKey() == QPixmap(enemyShipFilePath1).cacheKey()) {
            enemyImage = QPixmap(enemyShipFilePath2);
            enemyImageSize = width() * 1;
            soundEngine.playSound(lashSoundPath, 10.0f);
        }
        else if (enemyImage.cacheKey() == QPixmap(enemyShipFilePath2).cacheKey()) {
            enemyImage = QPixmap(enemyShipFilePath3);
            enemyImageSize = width() * 1;
        }
    }
    if (event->key() == Qt::Key_W && gameWon) {
        soundEngine.stopBGM();
        videoPlayer->playWinVideo();
        videoPlayer->playMusic();
        soundEngine.stopBGM();
        winImage = QPixmap();
        update();
    }
    if (event->key() == Qt::Key_Q && gameWon) {
        soundEngine.stopBGM();
        videoPlayer->playWin2Video();
        videoPlayer->playMusic();
        soundEngine.stopBGM();
        winImage = QPixmap();
        update();
    }
    if (event->key() == Qt::Key_U && gameLost) {
        soundEngine.stopBGM();
        videoPlayer->playLoseVideo();
        videoPlayer->playMusic();
        soundEngine.stopBGM();
        loseImage = QPixmap();
        update();
    }
    if (event->key() == Qt::Key_O && gameStarted) {
        soundEngine.stopBGM();
        videoPlayer->playCubeVideo();
        videoPlayer->playMusic();
        soundEngine.stopBGM();
        update();
    }
    pressedKeys.insert(event->key());
}

input_engine::input_engine() {}

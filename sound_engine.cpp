#include "sound_engine.h"
#include <QDebug>
#include "config.h"
#include "widget.h"


sound_engine::sound_engine(Widget* widget) {

    this->widget = widget;

    bgMusic1 = {
        bg1MusicPath, bg2MusicPath, bg3MusicPath, bg4MusicPath, bg5MusicPath,
        bg6MusicPath, bg7MusicPath, bg8MusicPath, bg9MusicPath, bg10MusicPath,
        bg11MusicPath, bg12MusicPath, bg13MusicPath, bg14MusicPath
    };

    bgMusic2 = {
        bg15MusicPath, bg16MusicPath, bg17MusicPath, bg18MusicPath, bg19MusicPath, bg20MusicPath, bg21MusicPath
    };

    bgMusic3 = {
        bg22MusicPath, bg23MusicPath, bg24MusicPath, bg25MusicPath, bg26MusicPath, bg27MusicPath
    };
}

void sound_engine::playSound(const QString &soundFilePath, float volume) {
    QMediaPlayer *player = new QMediaPlayer();
    QAudioOutput *audioOutput = new QAudioOutput();
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(volume);

    QObject::connect(player, &QMediaPlayer::playbackStateChanged, [this, player](QMediaPlayer::PlaybackState state) {
        if (state == QMediaPlayer::StoppedState) {
            activePlayers.removeOne(player);
            player->deleteLater();
        }
    });

    player->setSource(QUrl::fromLocalFile(soundFilePath));
    player->play();

    activePlayers.append(player);
}

void sound_engine::playBGM(const QString &musicFilePath)
{
    if (!bgmPlayer) {
        bgmPlayer = new QMediaPlayer();
        bgmOutput = new QAudioOutput();
        bgmPlayer->setAudioOutput(bgmOutput);
        bgmOutput->setVolume(1.0);
    }

    bgmPlayer->setSource(QUrl::fromLocalFile(musicFilePath));
    bgmPlayer->play();

    connect(bgmPlayer, &QMediaPlayer::mediaStatusChanged, this, &sound_engine::loopBGM);

}

void sound_engine::stopBGM() {
    if (bgmPlayer && bgmPlayer->playbackState() == QMediaPlayer::PlayingState) {
        bgmPlayer->stop();
    }
}

void sound_engine::loopBGM(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        bgmPlayer->play();
    }
}

void sound_engine::incrimentBGM1()
{
    if (activePlaylist != 1) {
        currentMusic1 = 0;
        activePlaylist = 1;
    }

    UpdateRadio();

    playBGM(bgMusic1[currentMusic1]);

    currentMusic1 = (currentMusic1 + 1) % bgMusic1.size();
}

void sound_engine::incrimentBGM2()
{
    if (activePlaylist != 2) {
        currentMusic2 = 0;
        activePlaylist = 2;
    }

    UpdateRadio();

    playBGM(bgMusic2[currentMusic2]);

    currentMusic2 = (currentMusic2 + 1) % bgMusic2.size();
}

void sound_engine::incrimentBGM3()
{
    if (activePlaylist != 3) {
        currentMusic3 = 0;
        activePlaylist = 3;
    }

    UpdateRadio();

    playBGM(bgMusic3[currentMusic3]);

    currentMusic3 = (currentMusic3 + 1) % bgMusic3.size();
}

void sound_engine::UpdateRadio()
{
    if (activePlaylist == 1)
    {
        widget->showText = true;
        musicTitle = RadioSongs1[currentMusic1];
        RadioSongs2[0];
        RadioSongs3[0];
    }
    else if (activePlaylist == 2)
    {
        widget->showText = true;
        musicTitle = RadioSongs2[currentMusic2];
        RadioSongs1[0];
        RadioSongs3[0];
    }
    else if (activePlaylist == 3)
    {
        widget->showText = true;
        musicTitle = RadioSongs3[currentMusic3];
        RadioSongs1[0];
        RadioSongs2[0];
    }
}

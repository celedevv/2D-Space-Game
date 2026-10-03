#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVideoWidget>
#include <QApplication>
#include <QScreen>
#include <QStringList>
#include <cstdlib>
#include <ctime>
#include <QVideoWidget>
#include <QMouseEvent>

#include "config.h"

class VideoPlayer : public QWidget {
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();

    void playVideo();
    void playPeak();
    void playWinVideo();
    void playWin2Video();
    void playLoseVideo();
    void playCubeVideo();
    void stopVideo();
    void playMusic();
    void stopMusic();
    void setVolume(float volume);

    QVideoWidget* getVideoWidget() const;
    QString getCurrentVideoPath() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
    QVideoWidget *videoWidget;
    QString currentVideoPath;
    bool isPlayingPeak;

    QStringList videoPaths = {
        QString(bg1VideoPath),
        QString(bg2VideoPath),
        QString(bg3VideoPath),
        QString(bg4VideoPath),
        QString(bg5VideoPath),
        QString(bg6VideoPath)
    };

    QStringList videoPaths2 = {
        QString(bgLebronVideoPath),
        QString(bgLebronVideoPath2),
        QString(bgLebronVideoPath3),
        QString(bgLebronVideoPath4),
        QString(bgLebronVideoPath5),
        QString(bgKobeVideoPath),
        QString(bgKobeShaqVideoPath),
        QString(bgJordanVideoPath),
        QString(bgLebronHeatVideoPath)
    };
};

#endif // VIDEOPLAYER_H

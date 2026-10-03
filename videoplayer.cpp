#include "config.h"
#include "videoplayer.h"
#include <QMediaFormat>

VideoPlayer::VideoPlayer(QWidget *parent) : QWidget(parent),
    mediaPlayer(new QMediaPlayer(this)),
    audioOutput(new QAudioOutput(this)),
    videoWidget(new QVideoWidget(this)),
    isPlayingPeak(false) {

    setAttribute(Qt::WA_NoMousePropagation);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);

    videoWidget->setAttribute(Qt::WA_OpaquePaintEvent, true);
    videoWidget->setAttribute(Qt::WA_NoSystemBackground, true);
    videoWidget->setAutoFillBackground(false);

    QMediaFormat format;
    format.setFileFormat(QMediaFormat::MPEG4);
    format.setVideoCodec(QMediaFormat::VideoCodec::H264);

    mediaPlayer->setVideoOutput(videoWidget);
    mediaPlayer->setAudioOutput(audioOutput);
    audioOutput->setVolume(1.0f);

    QScreen *screen = QGuiApplication::primaryScreen();
    setGeometry(screen->geometry());
    videoWidget->setGeometry(rect());
    videoWidget->show();
    videoWidget->lower();

    videoWidget->setStyleSheet("background-color: black;");

    std::srand(static_cast<unsigned int>(std::time(0)));

    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &VideoPlayer::handleMediaStatusChanged);
}

VideoPlayer::~VideoPlayer() {
    mediaPlayer->stop();
    mediaPlayer->setVideoOutput(nullptr);
    delete videoWidget;
    delete mediaPlayer;
    delete audioOutput;
}

void VideoPlayer::playVideo() {
    isPlayingPeak = false;
    int randomIndex = std::rand() % videoPaths.size();
    currentVideoPath = videoPaths[randomIndex];

    mediaPlayer->setSource(QUrl::fromLocalFile(currentVideoPath));
    mediaPlayer->play();
    videoWidget->show();
}

void VideoPlayer::playPeak() {
    isPlayingPeak = true;
    int randomIndex = std::rand() % videoPaths2.size();
    currentVideoPath = videoPaths2[randomIndex];

    mediaPlayer->setSource(QUrl::fromLocalFile(currentVideoPath));
    mediaPlayer->play();
    videoWidget->show();
}

void VideoPlayer::playWinVideo() {
    isPlayingPeak = false;
    mediaPlayer->setSource(QUrl::fromLocalFile(winVideoPath));
    mediaPlayer->play();
    videoWidget->show();
}

void VideoPlayer::playWin2Video() {
    isPlayingPeak = false;
    mediaPlayer->setSource(QUrl::fromLocalFile(winVideo2Path));
    mediaPlayer->play();
    videoWidget->show();
}

void VideoPlayer::playLoseVideo() {
    isPlayingPeak = false;
    mediaPlayer->setSource(QUrl::fromLocalFile(loseVideoPath));
    mediaPlayer->play();
    videoWidget->show();
}

void VideoPlayer::playCubeVideo() {
    isPlayingPeak = false;
    mediaPlayer->setSource(QUrl::fromLocalFile(cubeVideoPath));
    mediaPlayer->play();
    videoWidget->show();
}

void VideoPlayer::setVolume(float volume) {
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    audioOutput->setVolume(volume);
}

void VideoPlayer::stopVideo() {
    mediaPlayer->stop();
}

void VideoPlayer::playMusic() {
    audioOutput->setVolume(1.0);
}

void VideoPlayer::stopMusic() {
    audioOutput->setVolume(0.0);
}

QVideoWidget* VideoPlayer::getVideoWidget() const {
    return videoWidget;
}

QString VideoPlayer::getCurrentVideoPath() const {
    return currentVideoPath;
}

void VideoPlayer::handleMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        if (isPlayingPeak) {
            playVideo();
        } else {
        mediaPlayer->setPosition(0);
        mediaPlayer->play();
        }
    }
}

void VideoPlayer::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void VideoPlayer::mouseMoveEvent(QMouseEvent *event) {
    event->ignore();
}

void VideoPlayer::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

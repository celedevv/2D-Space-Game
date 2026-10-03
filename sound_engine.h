#ifndef SOUND_ENGINE_H
#define SOUND_ENGINE_H

#include <QString>
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>

class Widget;


class sound_engine : public QObject{
    Q_OBJECT

    Widget* widget;

public:
    sound_engine(Widget* widget);

    void incrimentBGM1();
    void incrimentBGM2();
    void incrimentBGM3();
    void UpdateRadio();
    void playSound(const QString &soundFilePath, float volume = 0.5f);
    void playBGM(const QString &musicFilePath);
    void stopBGM();

    QString musicTitle;
    int currentMusic1 = 0;
    int currentMusic2 = 0;
    int currentMusic3 = 0;

private:
    QMediaPlayer *bgmPlayer = nullptr;
    QAudioOutput *bgmOutput;

    QMediaPlayer *soundPlayer = nullptr;
    QAudioOutput *soundOutput;

    void loopBGM(QMediaPlayer::MediaStatus status);

    int activePlaylist = 1;

    QVector<QString> bgMusic1;
    QVector<QString> bgMusic2;
    QVector<QString> bgMusic3;

    QList<QMediaPlayer*> activePlayers;
};

#endif // SOUND_ENGINE_H

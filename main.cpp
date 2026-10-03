#include <QApplication>
#include "Widget.h"
#include "videoplayer.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    VideoPlayer *videoPlayer = new VideoPlayer();
    videoPlayer->showFullScreen();

    Widget *game = new Widget(videoPlayer);
    game->showFullScreen();
    game->raise();

    return app.exec();
}

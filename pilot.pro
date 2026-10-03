QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    config.cpp \
    game_engine.cpp \
    input_engine.cpp \
    main.cpp \
    network.cpp \
    sound_engine.cpp \
    videoplayer.cpp \
    widget.cpp

HEADERS += \
    config.h \
    game_engine.h \
    input_engine.h \
    network.h \
    sound_engine.h \
    videoplayer.h \
    widget.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

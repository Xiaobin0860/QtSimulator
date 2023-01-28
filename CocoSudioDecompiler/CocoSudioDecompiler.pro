#-------------------------------------------------
#
# Project created by QtCreator 2015-01-25T10:01:05
#
#-------------------------------------------------

QT       += core gui xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CocoSudioDecompiler
TEMPLATE = app

DESTDIR = $$PWD/../@bin

INCLUDEPATH +=  \
    $$PWD/../DevBase \
    $$PWD/../3rd/include

LIBS +=             \
    -L$$PWD/../@lib \
    -lDevBase

SOURCES += main.cpp \
    mainwindow.cpp  \
    cocoswidget.cpp \
    AppDelegate.cpp \
    rightwidget.cpp \
    trigger/acts.cpp \
    trigger/cons.cpp \
    scenemanager.cpp

HEADERS  += mainwindow.h \
    cocoswidget.h \
    AppDelegate.h \
    rightwidget.h \
    trigger/acts.h \
    trigger/cons.h \
    trigger/EventDef.h \
    scenemanager.h \
    Singleton.h

FORMS += mainwindow.ui

CONFIG += c++11

macx {
    INCLUDEPATH += \
        /Users/xiaobin/Documents/codes/cocos2d-x-3.4/cocos \
        /Users/xiaobin/Documents/codes/cocos2d-x-3.4/external/glfw3/include/mac \
        /System/Library/Frameworks

    LIBS += \
        -framework Cocoa \
        -framework OpenGL \
        -framework AudioToolbox \
        -framework OpenAL \
        -framework QuartzCore \
        -framework ApplicationServices \
        -framework IOKit \
        -framework Foundation   \
        -lcocos2d   \
        -lpng   \
        -lz
}

win32 {
    DEFINES += WIN32 _WIN32 _WINDOWS UNICODE

    INCLUDEPATH += \
        $$PWD/../../../../../cocos2d-x-3.4   \
        $$PWD/../../../../../cocos2d-x-3.4/cocos \
        $$PWD/../../../../../cocos2d-x-3.4/cocos/editor-support \
        $$PWD/../../../../../cocos2d-x-3.4/external   \
        $$PWD/../../../../../cocos2d-x-3.4/extensions   \
        $$PWD/../../../../../cocos2d-x-3.4/external/win32-specific   \
        $$PWD/../../../../../cocos2d-x-3.4/external/win32-specific/gles/include/OGLES  \
        $$PWD/../../../../../cocos2d-x-3.4/external/glfw3/include/win32

    LIBS +=             \
        -llibcocos2d    \
        -lglfw3
}

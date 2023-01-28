#-------------------------------------------------
#
# Project created by QtCreator 2015-01-25T10:01:05
#
#-------------------------------------------------

QT       += core gui

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
    rightwidget.cpp

HEADERS  += mainwindow.h \
    cocoswidget.h \
    AppDelegate.h \
    rightwidget.h

FORMS += mainwindow.ui

CONFIG += c++11

macx {
    INCLUDEPATH += \
        /Users/xiaobin/Documents/codes/cocos2d-x-3.2/cocos \
        /Users/xiaobin/Documents/codes/cocos2d-x-3.2/external/glfw3/include/mac \
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
        $$PWD/../../cocos2d-x-3.2   \
        $$PWD/../../cocos2d-x-3.2/cocos \
        $$PWD/../../cocos2d-x-3.2/cocos/platform/win32 \
        $$PWD/../../cocos2d-x-3.2/cocos/editor-support \
        $$PWD/../../cocos2d-x-3.2/external   \
        $$PWD/../../cocos2d-x-3.2/extensions   \
        $$PWD/../../cocos2d-x-3.2/external/win32-specific   \
        $$PWD/../../cocos2d-x-3.2/external/win32-specific/gles/include/OGLES  \
        $$PWD/../../cocos2d-x-3.2/external/glfw3/include/win32

    LIBS +=                     \
        libcocos2d.lib          \
        libCocosStudio.lib      \
        glfw3.lib               \
        Winmm.lib               \
        Ws2_32.lib              \
        libpng.lib              \
        libtiff.lib             \
        websockets.lib          \
        libcurl_imp.lib         \
        glew32.lib              \
        freetype250.lib         \
        libwebp.lib             \
        libjpeg.lib             \
        libzlib.lib             \
        libjpeg.lib             \
        libGUI.lib              \
        opengl32.lib            \
        libchipmunk.lib         \
        libExtensions.lib       \
        libAudio.lib            \
        libBox2D.lib            \
        libiconv.lib            \
        Advapi32.lib

}

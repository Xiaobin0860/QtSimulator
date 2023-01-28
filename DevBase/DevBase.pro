#-------------------------------------------------
#
# Project created by QtCreator 2015-01-25T10:05:52
#
#-------------------------------------------------

QT       -= gui

TARGET = DevBase
TEMPLATE = lib
CONFIG +=   \
    staticlib \
    -std=c++11

DESTDIR = $$PWD/../@lib

DEFINES += DEVBASE_LIBRARY

SOURCES += \
    logger.cpp

HEADERS +=\
    logger.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

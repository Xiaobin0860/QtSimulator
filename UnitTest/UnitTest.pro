#-------------------------------------------------
#
# Project created by QtCreator 2015-01-25T10:04:36
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_loggertest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = $$PWD/../@bin

INCLUDEPATH +=  \
    $$PWD/../DevBase

LIBS +=             \
    -L$$PWD/../@lib \
    -lDevBase

SOURCES += tst_loggertest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

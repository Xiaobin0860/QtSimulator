#include <QApplication>

#include "mainwindow.h"

#define LOG_TAG "main"
#include "logger.h"

MainWindow *g_mainWindow = nullptr;

int main(int argc, char *argv[])
{
    LOGGER->initLogSystem(argc, argv);

    QApplication a(argc, argv);

    MainWindow w;
    g_mainWindow = &w;
    w.show();

    return a.exec();
}

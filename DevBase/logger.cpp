#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <QStandardPaths>
#include <QFileInfo>
#include <QtCore>

#ifdef _WINDOWS
#	include <qt_windows.h>
#endif // _WINDOWS

void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
#if ENABLE_CONSOLE
#	ifdef _WINDOWS
		QString message = msg + "\n";
		OutputDebugStringA(qPrintable(message));
#	else
		fprintf(stderr, "%s\n", qPrintable(msg));
		fflush(stderr);
#	endif // _WINDOWS
#endif

    QString logfile = LOGGER->logPath();
//    fprintf(stderr, "%s\n", qPrintable(logfile));

    QFile outFile(logfile);
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
//        fprintf(stderr, "write message to file\n");
        QTextStream ts(&outFile);
        ts << msg << endl;
    }
//    fflush(stderr);
}

namespace CW {

Logger *Logger::_instance = NULL;

Logger::Logger() : _appName(""), _logPath(".")
{
}

Logger::~Logger()
{
    qDebug() << "~Logger()";
    qInstallMessageHandler(NULL);
}

bool Logger::initLogSystem(int argc, char *argv[], const QString& logPath)
{
//#if ENABLE_CONSOLE
//#ifdef _WINDOWS
//	AllocConsole();
//	FILE *stream = NULL;
//	freopen_s(&stream, "conout$", "w", stderr);
//#endif // _WINDOWS
//#endif

    if (argc)
    {
        QFileInfo info(argv[0]);
        _appName = info.baseName();
    }
    _logPath = logPath;
    QString lastLog = QString("%1/%2_%3.log").arg(_logPath).arg(_appName).arg(QDateTime::currentDateTime().addDays(-1).toString("yy-MM-dd"));
    QFile file(lastLog);
    if (file.exists()) {
        file.remove();
    }
    qInstallMessageHandler(myMessageHandler);
    return true;
}

} // namespace CW


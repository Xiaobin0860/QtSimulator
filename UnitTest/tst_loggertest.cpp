#include <QString>
#include <QtTest>

#define ENABLE_DEBUG_LOG 0
#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_TAG "LoggerTest"
#include "logger.h"

class LoggerTest : public QObject
{
    Q_OBJECT

public:
    LoggerTest();

private Q_SLOTS:
    void testLogger();
};

LoggerTest::LoggerTest()
{
}

void LoggerTest::testLogger()
{
    LOGT("This is a trace log.");
    LOGD("Debug: " << "testLogger");
    LOGI("Info: " << QString("QString"));
    LOGW("[Warn]: " << "!!!!!");
    LOGE("Error! code=" << -1);
    LOGF("!!!!fatal message!!!!");
}

//QTEST_APPLESS_MAIN(LoggerTest)
int main(int argc, char *argv[])
{
    LOGGER->initLogSystem(argc, argv);
    LoggerTest tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "tst_loggertest.moc"

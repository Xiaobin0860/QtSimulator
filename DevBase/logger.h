////////////////////////////////////////////////////////////////////////////////
///
/// \brief qt log4j
///
/// \author lxb
///
/// \note
/// 1.log level：FATAL > ERROR > WARN > INFO > DEBUG > TRACE > OFF
/// 2.usage:
///     在实现文件中
///
///     //自定义tag，该文件中所有日志输出都带有这个tag，便于过滤日志
///     #define LOG_TAG "YOUR_TAG"
///
///     //当前文件日志级别，LEVEL（含）以上的日志才输出, OFF级则日志调用都不进行编译
///     #define LOG_LEVEL LOG_LEVEL_DEBUG
///
///     //是否开启当前文件的调试日志，关闭则DEBUG（含）以下的日志不进行编译
///     #define ENABLE_DEBUG_LOG 1
///
/// \warning
/// 1. 不要在全局清理过程、全局析构中使用logger
///     logger本身是个单例，析构顺序未知
///
///
////////////////////////////////////////////////////////////////////////////////
#ifndef DEVBASE_LOGGER_H
#define DEVBASE_LOGGER_H
#include <QString>
#include <QDateTime>
#include <QThread>
#include <QTextStream>
#include <QDebug>

/*!
 * @macro ENABLE_DEBUG_LOG 用于屏蔽调试日志，通过"#define ENABLE_DEBUG_LOG 0"即可屏蔽Debug
 *      及其以下的日志输出
 */
#ifndef ENABLE_DEBUG_LOG
#   define ENABLE_DEBUG_LOG 1
#endif

/*!
 * @macro LOG_TAG 日志标签，用于日志分类
 */
#ifndef LOG_TAG
#   define LOG_TAG "LOG_TAG"
#endif

#ifndef LOG_LEVEL
#   define LOG_LEVEL LOG_LEVEL_TRACE
#endif

#ifndef LOG_SEP
#   define LOG_SEP " -- "
#endif

#define ENABLE_CONSOLE 1

//Qt 日志级别只有4级
//QtDebugMsg, QtWarningMsg, QtCriticalMsg, QtFatalMsg
//重新定义一下
#define LOG_FLAG_FATAL  (1 << 0)  // 0...000001
#define LOG_FLAG_ERROR  (1 << 1)  // 0...000010
#define LOG_FLAG_WARN   (1 << 2)  // 0...000100
#define LOG_FLAG_INFO   (1 << 3)  // 0...001000
#define LOG_FLAG_DEBUG  (1 << 4)  // 0...010000
#define LOG_FLAG_TRACE  (1 << 5)  // 0...100000

#define LOG_LEVEL_OFF   0                                                                                           // 0...000000
#define LOG_LEVEL_FATAL (LOG_FLAG_FATAL)                                                                            // 0...000001
#define LOG_LEVEL_ERROR (LOG_FLAG_FATAL|LOG_FLAG_ERROR)                                                             // 0...000011
#define LOG_LEVEL_WARN  (LOG_FLAG_FATAL|LOG_FLAG_ERROR|LOG_FLAG_WARN)                                               // 0...000111
#define LOG_LEVEL_INFO  (LOG_FLAG_FATAL|LOG_FLAG_ERROR|LOG_FLAG_WARN|LOG_FLAG_INFO)                                 // 0...001111
#define LOG_LEVEL_DEBUG (LOG_FLAG_FATAL|LOG_FLAG_ERROR|LOG_FLAG_WARN|LOG_FLAG_INFO|LOG_FLAG_DEBUG)                  // 0...011111
#define LOG_LEVEL_TRACE (LOG_FLAG_FATAL|LOG_FLAG_ERROR|LOG_FLAG_WARN|LOG_FLAG_INFO|LOG_FLAG_DEBUG|LOG_FLAG_TRACE)   // 0...111111

#if LOG_LEVEL
#   define LOG_MESSAGE_INTERNAL(_flag_, _msg_) do {                 \
        if (_flag_ & LOG_LEVEL) {                                   \
            QString _message_;                                      \
            QTextStream _ts_(&_message_);                           \
            _ts_ << LOGGER->logPrefix() << _msg_;                   \
            QMessageLogContext _context_;                           \
            _context_.line = _flag_;                                \
            qt_message_output(QtCriticalMsg, _context_, _message_); \
        }   \
    } while (0)
#else
#   define LOG_MESSAGE_INTERNAL(flag, msg)   ((void)0)
#endif

#define LOGF(msg)   LOG_MESSAGE_INTERNAL(LOG_FLAG_FATAL, "F/" << LOG_TAG << LOG_SEP << msg)

#define LOGE(msg)   LOG_MESSAGE_INTERNAL(LOG_FLAG_ERROR, "E/" << LOG_TAG << LOG_SEP << msg)

#define LOGW(msg)   LOG_MESSAGE_INTERNAL(LOG_FLAG_WARN, "W/" << LOG_TAG << LOG_SEP << msg)

#define LOGI(msg)   LOG_MESSAGE_INTERNAL(LOG_FLAG_INFO, "I/" << LOG_TAG << LOG_SEP << msg)

#if ENABLE_DEBUG_LOG
#   define LOGD(msg)    LOG_MESSAGE_INTERNAL(LOG_FLAG_DEBUG, "D/" << LOG_TAG << LOG_SEP << msg)
#else
#   define LOGD(msg)   ((void)0)
#endif

#if ENABLE_DEBUG_LOG
#   define LOGT(msg)    LOG_MESSAGE_INTERNAL(LOG_FLAG_TRACE, "T/" << LOG_TAG << LOG_SEP << msg)
#else
#   define LOGT(msg)   ((void)0)
#endif

#define LOGGER (CW::Logger::getInstance())

namespace CW {

class Logger
{
public:
    static Logger* getInstance();
    void destroyInstance();

    Logger();
    ~Logger();

    bool initLogSystem(int argc, char *argv[], const QString& logPath = ".");

    QString logPrefix();
    QString& logPath();

private:
    QString _appName;
    QString _logPath;

    static Logger *_instance;
};

inline Logger *Logger::getInstance()
{
    if (!_instance) {
        _instance = new Logger;
    }
    return _instance;
}

inline void Logger::destroyInstance()
{
    if (_instance) delete _instance;
    _instance = NULL;
}

inline QString Logger::logPrefix()
{
    return QString("%1[%2.%3] ").arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz")).arg(_appName).arg((quint64)QThread::currentThreadId());
}

inline QString& Logger::logPath()
{
    static QString logPath = QString("%1/%2_%3.log").arg(_logPath).arg(_appName).arg(QDateTime::currentDateTime().toString("yy-MM-dd"));
    return logPath;
}

}


#endif // LOGGER_H

#ifndef CPRINTLOG_H
#define CPRINTLOG_H

#include <QThread>
#include <QSemaphore>
#include <QMutex>
//#include "../../logger/logger_service.h"

#define LOGFILE "/Runlog/logger.txt"
#define LOGGERCONFIG "/Runlog/logConfig.ini"

#define PRINTLOG CPrintLog::CreateLogger()

enum LogLevel
{
	LOG_LEVEL_FATAL = 1,
	LOG_LEVEL_ERROR = 2,
	LOG_LEVEL_WARNING = 3,
	LOG_LEVEL_INFO = 4,
	LOG_LEVEL_DEBUG = 5,
	LOG_LEVEL_TRACE = 6
};

class CPrintLog : public QThread
{
    Q_OBJECT
public:
    //explicit CPrintLog(QObject *parent = nullptr);
    ~CPrintLog();

    static CPrintLog *CreateLogger();

    void WriteLog(LogLevel level, const QString &message);

    void WriteLog(LogLevel level, const char *format, ...);
private:
    void addLogger(LogLevel level, const QString &str_msg);
    void writeFile(QByteArray &loggerData);
    void autoremove(const QString path);

protected:
    void run();

private:
    explicit CPrintLog(QObject *parent = nullptr);

private:
    QByteArray m_logger_cache;
    QMutex m_mutex;
    QString m_strLogFile;
    QString loggerConfig;
    bool m_isRun;
    QSemaphore m_sem;
};

#endif // CPRINTLOG_H

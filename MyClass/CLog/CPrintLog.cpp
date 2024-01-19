#include "CPrintLog.h"
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QApplication>
#include <QMap>
#include <QDir>

static int loggerLevel = LOG_LEVEL_INFO; // 默认写文件级数
static int loggerMax = 1 * 1024 * 1024; // 默认单个文件最大内容, 单位:Byte
static int loggerTimeout = 5 * 1000; // 默认单次写文件时间间隔, 单位:ms
static int MAX_LOG_FILES = 15;       //默认日志文件最多15个

static QMap<int, QString> LogLevelMap = {
    {LOG_LEVEL_FATAL, "FATAL"},
    {LOG_LEVEL_ERROR, "ERROR"},
    {LOG_LEVEL_WARNING, "WARNING"},
    {LOG_LEVEL_INFO, "INFO"},
    {LOG_LEVEL_DEBUG, "DEBUG"},
    {LOG_LEVEL_TRACE, "TRACE"},
};

CPrintLog::CPrintLog(QObject *parent) : QThread(parent)
  , m_isRun(false)
{
    QString appPath = QApplication::applicationDirPath();
    m_strLogFile = appPath + LOGFILE;
    loggerConfig = appPath + LOGGERCONFIG;
    m_isRun = true;
    this->start();
}

CPrintLog::~CPrintLog()
{
    if(m_isRun)
    {
        m_isRun = false;
        this->quit();
        this->wait();
        this->deleteLater();
    }
}

CPrintLog *CPrintLog::CreateLogger()
{
	// TODO: 在此处插入 return 语句
    static CPrintLog s_logger;

    return &s_logger;
}

void CPrintLog::WriteLog(LogLevel level, const QString &message)
{
    addLogger(level, message);//注释
}

void CPrintLog::WriteLog(LogLevel level, const char *format, ...)
{
    //日志正文
    std::string strLogMsg;

    //先计算一下不定参数的长度，以便于分配空间
    va_list ap;
    va_start(ap, format);//注释
    int nLogMsgLength = vsnprintf(NULL, 0, format, ap);//注释
    va_end(ap);//注释

    //容量必须算上最后一个\0
    if ((int)strLogMsg.capacity() < nLogMsgLength + 1)
    {
        strLogMsg.resize(nLogMsgLength + 1);//注释
    }

    va_list aq;
    va_start(aq, format);//注释
    vsnprintf((char*)strLogMsg.data(), strLogMsg.capacity(), format, aq);//注释
    va_end(aq);//注释

    QString msg(strLogMsg.data());//注释
    addLogger(level, msg);//注释
}

void CPrintLog::addLogger(LogLevel level,const QString &str_msg)
{
    if(m_isRun)
    {
        QString str_loger = QString("%1 [%2] %3\r\n")
                        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"))
                        .arg(LogLevelMap.value(level))
                        .arg(str_msg);//注释

        qDebug() << str_loger;

        QMutexLocker lock(&m_mutex);
        m_logger_cache.append(str_loger);//注释
        m_sem.release();
    }
}

void CPrintLog::writeFile(QByteArray &loggerData)
{
    QFile fileOld(m_strLogFile);//注释
    if (fileOld.size() >= loggerMax)
    {
        QString str_newName = QString("%1.%2").arg(m_strLogFile).arg(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"));//注释
        fileOld.rename(str_newName);//注释
        autoremove(QApplication::applicationDirPath() + "/Runlog");
    }

    QFile fileNew(m_strLogFile);//注释
    if (!fileNew.open(QFile::Append))
    {
        return;
    }
//    fileNew.write(loggerData);//注释
    QTextStream fileStream(&fileNew);
    fileStream.setCodec("UTF-8");
    fileStream << loggerData;
    fileNew.close();//注释
}

void CPrintLog::autoremove(const QString path)
{
    //这个函数首先获取指定目录中所有日志文件的列表，然后根据文件名的时间对它们进行排序。
    //如果文件数超过最大值，它将删除最旧的文件，直到文件数等于最大值。在这个例子中，最大值为10，但你可以根据需要更改它。

    QDir dir(path);
    QStringList namesend;
    QStringList results;
    namesend<<"*.txt.*";
    results = dir.entryList(namesend,QDir::Files,QDir::Time);

    int size = results.length();

    while(size>MAX_LOG_FILES)
    {
        QString fileName = path + "/" + results[size-1];
        size --;
        QFile fileNew(fileName);//注释
        fileNew.remove();
    }
}

void CPrintLog::run()
{
    while (m_isRun)
    {
        int iCount = m_sem.available();
        iCount = iCount == 0 ? 1 : iCount;
        m_sem.tryAcquire(iCount, 200);
        QByteArray tempData;
        if (!m_logger_cache.isEmpty())
        {
            QMutexLocker lock(&m_mutex);
            tempData.swap(m_logger_cache);
        }

        if(!tempData.isEmpty())
        {
            writeFile(tempData);//注释
        }
    }
}

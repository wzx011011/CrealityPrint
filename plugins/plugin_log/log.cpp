#include "log.h"
#include <QString>
#include <QLoggingCategory>
#include <QIODevice>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QMutex>
#include <QApplication>
#include <QRegExp>
#include <QDesktopServices>
#include <stdio.h>

static QString logDirectory;
static QString logFileName;
static QMutex mutex;
static QFile file;
static QTextStream gblTextStream(stdout);
static bool gblHaveOpenStrem = false;

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
#ifdef QT_NO_DEBUG
    if (!gblHaveOpenStrem)
    {
        gblHaveOpenStrem = true;
        file.setFileName(logFileName);
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        gblTextStream.setDevice(&file);
    }
#endif
    mutex.lock();// 加锁
    QString text = "[default]";
    switch(type)//log 信息类型
    {
    case QtDebugMsg:
        text = QString("[Debug]");
        break;
    case QtInfoMsg:
        text = QString("[Info]");
        break;
    case QtWarningMsg:
        text = QString("[Warning]");
        break;
    case QtCriticalMsg:
        text = QString("[Critical]");
        break;
    case QtFatalMsg:
        text = QString("[Fatal]");
    }

    // 设置输出信息格式
    text.append(QString(" [%1] ").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
#ifdef QT_DEBUG
    QString filename = context.file;
    int pos = filename.lastIndexOf('\\');
    if (pos > 0)
    {
        filename = filename.mid(pos + 1);
    }
    else
    {
        pos = filename.lastIndexOf('/');
        if (pos > 0)
        {
            filename = filename.mid(pos + 1);
        }
    }
    text.append(QString(" [%1: Line: %2] ").arg(QString(filename)).arg(context.line));
#endif
    text.append(QString(" [Function: %1] ").arg(QString(context.function)));

    //file.setFileName(logFileName);
    //file.open(QIODevice::WriteOnly | QIODevice::Append);
    //gblTextStream.setDevice(&file);

    // 输出信息至文件中（读写、追加形式）
    //qDebug() << "text.lenght = " << text.length() << "  " << text.size();  

    gblTextStream << text << QString(" %1").arg(msg) << endl;
    gblTextStream.flush();
    //file.close();

    mutex.unlock();// 解锁
}

void logInit()
{
#ifdef QT_NO_DEBUG
    QString Directory =QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(Directory);
    if(!dir.exists())
        dir.mkdir(Directory);

    // 日志存放路径
    logDirectory  = Directory + "/Log/";
    dir.setPath(logDirectory);
    if(!dir.exists())
        dir.mkdir(logDirectory);

    //获取APP名字作为log名字
    logFileName = logDirectory + (QApplication::applicationName()+".log");

    qInfo() << logFileName;

    /* 以下这段代码的含义是初始化时检查日志文件是否存在一个月之前的日志，如果存在删除之 */
    {
        QMutexLocker locker(&mutex);
        //mutex.lock();
        QFile file(logFileName);
        file.open(QIODevice::ReadOnly);
        QTextStream gblTextStream(&file);
        QString temp;
        QStringList tempList;
        QRegExp regExp(".*(20\\d\\d-\\d\\d-\\d\\d).*");
        while ((temp = gblTextStream.readLine()).isEmpty() == false)
        {
            if(temp.indexOf(regExp) >= 0)
            {
                QDate date = QDate::fromString(regExp.cap(1), "yyyy-MM-dd");
                QDate currentDate = QDate::currentDate();
                /* 判断当前行所记载的日期和现今的日期天数之差是否大于记录该条日志时的那个月的天数 */
                if(date.daysTo(currentDate) < date.day())
                {
                    tempList << temp;
                    tempList << gblTextStream.readLine();
                }
            }
        }
        file.close();
        file.open(QIODevice::Truncate | QIODevice::WriteOnly);
        gblTextStream.setDevice(&file);
        for(auto iterator = tempList.begin(); iterator != tempList.end(); iterator++)
        {
            gblTextStream << *iterator << endl;
        }
        file.close();
        //mutex.unlock();
    }
#endif

    /* 安装上述自定义函数 */
    qInstallMessageHandler(outputMessage);

#ifdef QT_NO_DEBUG
    file.setFileName(logFileName);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    gblTextStream.setDevice(&file);
    gblHaveOpenStrem = true;
//#else
//    gblTextStream.setDevice(stdout);
#endif
}

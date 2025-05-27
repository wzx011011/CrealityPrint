#ifndef _networkmanagerthread_H
#define _networkmanagerthread_H
#include <QtCore/QObject>
#include <QtNetwork>
#include <QRunnable>
#include <QString>
#include <functional>
#include <list>
#include<unordered_map>
#include "networktaskthread.h"

struct requestInfo
{
    int type; //1: net; 2: oss; 3: sensors
    QMap<QString, QString> dataMap;
    std::function<void(QByteArray)> call_back;
    std::function<void(QByteArray)> call_back1;

    requestInfo()
    {
        type = 0;
        dataMap.clear();
        call_back = NULL;
        call_back1 = NULL;
    }
};

class NetworkManagerThread : public QThread
{
    Q_OBJECT
public:
    NetworkManagerThread(QObject* parent = 0);
    void setRunningState(bool state);
    void addRequestInfoList(int type, const QMap<QString, QString>& data, const std::function<void(QByteArray)>& call_back, const std::function<void(QByteArray)>& call_back1);
    void setThreadPool(QThreadPool* pool) { m_ThreadPool = pool; }

private:
    std::list<requestInfo> requestInfoList;
    bool isRunning;
    QThreadPool* m_ThreadPool;
    QMutex mutex;

protected:
    //QThread的虚函数
    //线程处理函数
    //不能直接调用，通过start()间接调用
    void run() override;

};

#endif // _networkmanagerthread_H

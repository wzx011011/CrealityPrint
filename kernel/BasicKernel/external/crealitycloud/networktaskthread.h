#ifndef _networktaskthread_H
#define _networktaskthread_H
#include <QtCore/QObject>
#include <QObject>
#include <QRunnable>
#include <QString>
#include <functional>
#include "cxnet.h"

class NetworkTaskThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    NetworkTaskThread(QObject* parent = 0);

    void setCallbackFun(const std::function<void(QByteArray)>& call_back);
    void setRequestData(const QMap<QString, QString>& data);

private:
    void setRequestUrl(QString url) { m_url = url; }
    void setRequestBody(QString body) { m_body = body; }
    void setRequestHeader(QString osVer, QString duid, QString osLang, QString appId, QString platform, QString requestId, QString appVer, QString token, QString uid);
    void setRequestTimeOut(int value) { m_timeOut = value; }
    QString getOSVersion();
    QString getDUID();
    QString getCurrentLanguage();
    QString getCloudUrl();

private:
    cxnet::CXHttpNet m_httpnet;
    typedef std::function<void(QByteArray)> Func;
    Func m_call_back;
    QString m_app_version;
    QString m_url;
    QString m_body;
    int m_timeOut = 0; //ms
    int m_request_type; //1: post; 2 :get
    std::unordered_map<std::string, std::string> m_head;
    


protected:
    //QThread的虚函数
    //线程处理函数
    //不能直接调用，通过start()间接调用
    void run() override;

};

#endif // _networktaskthread_H

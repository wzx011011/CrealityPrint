#ifndef _Camerap2pConnectThread_H
#define _Camerap2pConnectThread_H
#include <QtCore/QObject>
#include <QTcpSocket>
#include <QObject>
#include <QtNetwork>
#include <QRunnable>

class Camerap2pInterface;
class Camerap2pConnectThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    Camerap2pConnectThread(Camerap2pInterface* p2pInterface, QObject* parent = 0);
    //void setNetworkDescriptor(qintptr  socketDescriptor);

private:
    void* m_rtspSession;
    Camerap2pInterface* m_p2pInterface;

signals:
    void sigP2PConnectSuccess(void* p2pSession);

protected:
    //QThread的虚函数
    //线程处理函数
    //不能直接调用，通过start()间接调用
    void run() override;

};

#endif // _Camerap2pConnectThread_H

#ifndef _RtspThread_H
#define _RtspThread_H

//#include <QtCore/QThread>
#include <QObject>
#include <QtNetwork>
#include <QRunnable>

class Camerap2pInterface;
class RtspThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit RtspThread(Camerap2pInterface* p2pInterface, QObject *parent = 0);
    ~RtspThread();


    //int startServer();
    int stopServer();
    //int init_rstp_server();
    int clientClose();
    void setRtspServerPort(int port);
    void setStopFunction(const std::function<void()>& func);
    void setRestartFunction(const std::function<void()>& func);
    void setP2PSession(void* psession);
    void setNetworkDescriptor(qintptr  socketDescriptor);
    void setCameraDeviceStr(QString strDev);

private slots:
    //void onNewConnected();
    void onDisConnected();
    void readMessage();
    void onWriteMessage(char* message, int size);
    void onStopFinish();
    void onRestartXY();

signals:
    void sigWriteMessage(char* buf, int size);
    void sigRunFinished();
    void sigRestartXY(QString strdid);

protected:
    //QThread的虚函数
    //线程处理函数
    //不能直接调用，通过start()间接调用
    void run() override;


private:
    int m_port;
    QTcpServer* tcpServer;
    QTcpSocket* clientConnection;
    int g_proxy_stop;
    bool m_p2pConnected;
    bool hasNewConnect;
    int g_proxy_errno;
    void* m_rtspSession;

    QString strdevice;
    
    typedef std::function<void()> Func;
    Func m_funStopFinish;
    Func m_funRestart;
    bool hasFinshFunc;
    Camerap2pInterface* m_p2pInterface;
};

#endif // _RtspThread_H
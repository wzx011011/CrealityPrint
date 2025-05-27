#ifndef _CAMERASERVER_H
#define _CAMERASERVER_H
#include <QtCore/QObject>
#include <QTcpServer>
#include <QThreadPool>
#include "camerap2pConnectThread.h"

class Camerap2pInterface;
class CameraServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CameraServer(int port = 554, QObject* parent = nullptr);
    int startP2PServer(std::string productName, std::string strDid, std::string strApiLicense, std::string strInit, std::string strCrc = "C2r@-w.5", std::string strP2P = "aY.6-E@S");
    void stopVideoThread();

protected:
    virtual void incomingConnection(qintptr  socketDescriptor);

public slots:
    void onP2PConnected(void* pSession);
    void onRestartXY(QString strDID);

signals:
    void sigCameraRestart(QString strdid);

private:
    //Camerap2pConnectThread* p2pthread;
    void* m_session;
    qintptr  m_socketDescriptor;
    QString m_productName;
    QThreadPool m_cameraThreadPool;
    QThreadPool m_syThreadPool;
    int m_port;
    Camerap2pInterface* p2pInterface;
};



#endif // _CAMERASERVER_H

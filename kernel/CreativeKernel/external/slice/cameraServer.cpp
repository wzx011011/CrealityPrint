#include "cameraServer.h"
#include <QDebug>
#include "rtspthread.h"
#include "camerap2pinterface.h"
#define 	_PPCS_API___INC_

CameraServer::CameraServer(int port, QObject* parent) : QTcpServer(parent)
{
    bool rst = listen(QHostAddress::LocalHost, port);
    if (!rst)
    {
        qDebug() <<"CameraServer listen error : " <<errorString();
    }
    m_session = NULL;
    m_port = port;
    p2pInterface = new Camerap2pInterface();
}

void CameraServer::incomingConnection(qintptr  socketDescriptor)
{
    qDebug() << "incomingConnection socketDescriptor: " << socketDescriptor << " port:" << m_port;
    m_socketDescriptor = socketDescriptor;

    Camerap2pConnectThread* p2pthread = new Camerap2pConnectThread(p2pInterface);
    connect(p2pthread, &Camerap2pConnectThread::sigP2PConnectSuccess, this, &CameraServer::onP2PConnected);
    m_syThreadPool.start(p2pthread);
    //m_rtspThread->start();
}

int CameraServer::startP2PServer(std::string productName, std::string strDid, std::string strApiLicense, std::string strInit, std::string strCrc, std::string strP2P)
{
    xy_para_t pipelineParam;
    m_productName = QString::fromStdString(productName);
    memcpy(pipelineParam.pDIDString, strDid.c_str(), SIZE_DID);
    memcpy(pipelineParam.pAPILicense, strApiLicense.c_str(), SIZE_APILICENSE);
    memcpy(pipelineParam.pInitString, strInit.c_str(), SIZE_INITSTRING);
    memcpy(pipelineParam.pCRCKey, strCrc.c_str(), SIZE_APILICENSE);
    memcpy(pipelineParam.pP2PKey, strP2P.c_str(), SIZE_APILICENSE);
    p2pInterface->set_xy_para(pipelineParam);

    qDebug() << "strDid = " << strDid.c_str();
    qDebug() << "strApiLicense = " << strApiLicense.c_str();
    qDebug() << "strInit = " << strInit.c_str();
    qDebug() << "portNum = " << m_port;


    int ret = p2pInterface->xy_init();
    if (0 != ret) {
        qWarning() << "xy_init() error :"<< ret;
        return -1;
    }

    return 0;
}

void CameraServer::onP2PConnected(void* pSession)
{
    qDebug() << "onP2PConnected pSeeion: " << pSession;
    m_session = pSession;
    RtspThread* m_rtspThread = new RtspThread(p2pInterface);
    m_rtspThread->setP2PSession(m_session);
    m_rtspThread->setNetworkDescriptor(m_socketDescriptor);
    m_rtspThread->setCameraDeviceStr(m_productName);

    QObject::disconnect(m_rtspThread, SIGNAL(sigRestartXY(QString)), this, SLOT(onRestartXY(QString)));
    QObject::connect(m_rtspThread, SIGNAL(sigRestartXY(QString)), this, SLOT(onRestartXY(QString)));

    m_cameraThreadPool.start(m_rtspThread);
}

void CameraServer::onRestartXY(QString strDID)
{
    emit sigCameraRestart(strDID);
}

void CameraServer::stopVideoThread()
{
    m_cameraThreadPool.waitForDone(20);
    m_syThreadPool.waitForDone(20);
}
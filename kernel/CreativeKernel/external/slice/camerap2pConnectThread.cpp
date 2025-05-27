#include "camerap2pConnectThread.h"
#include <QTcpSocket>
#include <QDebug>
#include "camerap2pinterface.h"

Camerap2pConnectThread::Camerap2pConnectThread(Camerap2pInterface* p2pInterface,QObject* parent)
{
	//m_socketDescriptor = NULL;
	rtsp_session_t* sesson = (rtsp_session_t*)calloc(1, sizeof(rtsp_session_t));
	m_rtspSession = sesson;
	m_p2pInterface = p2pInterface;
}

void Camerap2pConnectThread::run()
{
	rtsp_session_t* cli = (rtsp_session_t*)m_rtspSession;// = getRtspSession();
	cli->run = 1;

	//p2p连接
	while (1) {
		int ret = m_p2pInterface->xy_connect(cli);
		if (0 == ret)
		{
			//m_p2pConnected = true;
			qDebug() << "Camerap2pConnectThread xy_connect true";
			sigP2PConnectSuccess(m_rtspSession);
			break;
		}
		else
		{
			qDebug() << "Camerap2pConnectThread xy_connect false " << ret;
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			continue;
		}
	}

	return;

}
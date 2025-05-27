#include "rtspthread.h"
#include <QDebug>
#include "camerap2pinterface.h"

RtspThread::RtspThread(Camerap2pInterface* p2pInterface, QObject* parent)
{
	m_rtspSession = NULL;
	m_port = 0;
	g_proxy_stop = 0;
	m_p2pConnected = false;
	hasNewConnect = false;
	m_funStopFinish = NULL;
	hasFinshFunc = false;
	tcpServer = NULL;
	clientConnection = NULL;
	strdevice = "";
	m_p2pInterface = p2pInterface;
}
RtspThread::~RtspThread()
{

}

int RtspThread::stopServer()
{
	qDebug() << "stopServer";
	clientClose();
	
	if (tcpServer)
	{
		tcpServer->close();
		tcpServer = NULL;
	}
	
	return 0;
}

int RtspThread::clientClose()
{
	g_proxy_stop = 1;
	m_p2pConnected = false;
	hasNewConnect = false;

	if (clientConnection)
	{
		clientConnection->close();
		clientConnection = NULL;
	}

	return 0;
}

void RtspThread::onDisConnected()
{
	qDebug() << "RtspThread onDisConnected";
	clientClose();
}

void RtspThread::onStopFinish()
{
	qDebug() << "RtspThread onStopFinish";
	m_p2pConnected = false;
	hasNewConnect = false;

	if (clientConnection)
	{
		clientConnection->close();
		clientConnection = NULL;
	}
	PPCS_DeInitialize();

	if (hasFinshFunc)
	{
		m_funStopFinish();
		hasFinshFunc = false;
	}
	
}

void RtspThread::onRestartXY()
{
	qWarning() << "onRestartXY beg";

	m_funRestart();
}

void RtspThread::readMessage()
{
	qDebug() << "readMessage beg "<< m_rtspSession;
	rtsp_session_t* cli = (rtsp_session_t*)m_rtspSession;// = getRtspSession();
	cli->run = 1;
	int retry = 0;

	if (!g_proxy_stop && cli->run)
	{
		int ok = 0;
		char buf[512 + 1] = { 0 };

		int ret = clientConnection->read((char*)buf, sizeof(buf) - 1);
		qDebug() << "RtspThread readMessage get length " << ret;
		QString msg = QString(QLatin1String(buf));
		qDebug() << "RtspThread readMessage get data " << msg;

		int iWlen = 0;
		retry = 0;

		if (ret > 0) {
			while (iWlen < ret && retry++ < 3)
			{
				if (0 > (ok = m_p2pInterface->xy_write(cli->p2pSessionHandle, CH_CMD, (char*)&buf[iWlen], ret - iWlen))) {
					qWarning() << "RtspThread xy_write error " << ok;
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
					break;
				}
				else {
					qDebug() << "RtspThread xy_write success";
					iWlen += ok;
				}
			}
			if (iWlen != ret) {
				qWarning() << "iWlen != ret";
			}
		}

		if (retry >= 3)
		{
			qDebug() << "readMessage retry >= 3";
			PPCS_ForceClose(cli->p2pSessionHandle);
			cli->run = 0;
			cli->rtsp_sd = -1;
			cli->p2pSessionHandle = -1;
			free(cli);
			cli = NULL;
		}
	}
	else
	{
		qDebug() << "readMessage error";
		PPCS_ForceClose(cli->p2pSessionHandle);
		cli->rtsp_sd = -1;
		cli->p2pSessionHandle = -1;
		free(cli);
		cli = NULL;
	}

	return;
}

void RtspThread::onWriteMessage(char* message, int size)
{
	int ret = -1;
	if (clientConnection)
	{
		ret = clientConnection->write(message, size);
		if (ret < 0)
		{
			qWarning() << "clientConnection->write error -1" << " error msg:" << clientConnection->errorString();
			//clientClose();
		}
		//qDebug() << "CameraProxy writeMessage "<< ret;
	}
	else
	{
		qWarning() << "clientConnection = null";
		//clientClose();
	}
}


void RtspThread::run()
{
	qDebug() << "RtspThread::run() beg";
	while (!hasNewConnect)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		continue;
	}

	qDebug() << "RtspThread::read and write beg";
	rtsp_session_t* p_sess = (rtsp_session_t*)m_rtspSession;//getRtspSession();
	unsigned char buf[512] = {0};
	int ok = -1;
	int ret = -1;
	int ReadSize = sizeof(buf);
	int retry = 0;
	int maxTimes = 10;

	while (!g_proxy_stop && p_sess->run) {
		int iWlen = 0;
		ok = 0;
		memset(buf, 0, ReadSize);
		if (0 < (ret = m_p2pInterface->xy_read(p_sess->p2pSessionHandle, CH_CMD, (char*)buf, ReadSize))) {
			//qDebug() << "xy_read success ret= " << ret;
			//buf[ret] = 0;
			retry = 0;
			if (strstr((char*)buf, "xy_p2p_Exceed_max_session") != NULL)
			{
				p_sess->run = 0;
				break;
			}
			emit sigWriteMessage((char*)&buf[iWlen], ret - iWlen);
		}
		else if (ret < 0) {
			//qWarning() << "FAIL xy_read " << m_rtspSession;
			retry++;
			if (retry > maxTimes)
			{
				qDebug() << "sigRestartXY()";
				//PPCS_ForceClose(p_sess->p2pSessionHandle);
				emit sigRestartXY(strdevice);
				retry = 0;
				maxTimes += 50;
				//break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			continue;
		}
		else if (ret == 0)
		{
			//qDebug() << "xy_read length == 0 continue";
			//std::this_thread::sleep_for(std::chrono::milliseconds(2));
			continue;
		}

		if (-1 == ret || ok < 0) {
			p_sess->run = 0;
			qWarning() << "==run 0==";
			break;
		}

	}
	//emit sigRunFinished();
	qDebug() << "RtspThread Run End";
	return;
}

void RtspThread::setRtspServerPort(int port)
{
	m_port = port;
}

void RtspThread::setStopFunction(const std::function<void()>& func)
{
	hasFinshFunc = true;
	m_funStopFinish = func;
}

void RtspThread::setRestartFunction(const std::function<void()>& func)
{
	m_funRestart = func;
}

void RtspThread::setP2PSession(void* psession)
{
	m_rtspSession = psession;
}
void RtspThread::setNetworkDescriptor(qintptr  socketDescriptor)
{
	clientConnection = new QTcpSocket();
	clientConnection->setSocketDescriptor(socketDescriptor);

	QObject::disconnect(clientConnection, &QTcpSocket::disconnected, this, &RtspThread::onDisConnected);
	QObject::connect(clientConnection, &QTcpSocket::disconnected, this, &RtspThread::onDisConnected);

	QObject::disconnect(clientConnection, &QTcpSocket::readyRead, this, &RtspThread::readMessage);
	QObject::connect(clientConnection, &QTcpSocket::readyRead, this, &RtspThread::readMessage);

	QObject::disconnect(this, SIGNAL(sigWriteMessage(char*, int)), this, SLOT(onWriteMessage(char*, int)));
	QObject::connect(this, SIGNAL(sigWriteMessage(char*, int)), this, SLOT(onWriteMessage(char*, int)), Qt::BlockingQueuedConnection);

	QObject::disconnect(this, SIGNAL(sigRunFinished()), this, SLOT(onStopFinish()));
	QObject::connect(this, SIGNAL(sigRunFinished()), this, SLOT(onStopFinish()));

	//QObject::disconnect(this, SIGNAL(sigRestartXY()), this, SLOT(onRestartXY()));
	//QObject::connect(this, SIGNAL(sigRestartXY()), this, SLOT(onRestartXY()));

	hasNewConnect = true;
}

void RtspThread::setCameraDeviceStr(QString strDev)
{
	strdevice = strDev;
}

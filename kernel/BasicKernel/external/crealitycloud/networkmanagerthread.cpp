#include "networkmanagerthread.h"
#include <QTcpSocket>
#include <QDebug>


NetworkManagerThread::NetworkManagerThread(QObject* parent)
{
	isRunning = false;
}

void NetworkManagerThread::run()
{
	while (isRunning)
	{
		mutex.lock();
		if (requestInfoList.empty())
		{
			mutex.unlock();
			QThread::msleep(100);
			continue;
		}
		auto requestInfo = requestInfoList.front();
		requestInfoList.pop_front();
		mutex.unlock();

		NetworkTaskThread* networkthread = new NetworkTaskThread();
		networkthread->setCallbackFun(requestInfo.call_back);
		networkthread->setRequestData(requestInfo.dataMap);
		m_ThreadPool->start(networkthread);
		QThread::msleep(20);
	}

	return;

}

void NetworkManagerThread::setRunningState(bool state)
{
	isRunning = state;

	//if (!state)
	//{
	//	//m_ThreadPool.waitForDone(20);
	//}
}

void NetworkManagerThread::addRequestInfoList(int type, const QMap<QString, QString>& data, const std::function<void(QByteArray)>& call_back, const std::function<void(QByteArray)>& call_back1)
{
	requestInfo info;
	info.type = type;
	info.dataMap = data;
	info.call_back = call_back;
	info.call_back1 = call_back1;

	mutex.lock();
	requestInfoList.push_back(info);
	qDebug() << " requestInfoList size = " << requestInfoList.size();
	mutex.unlock();
}


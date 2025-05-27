#include "printernetmng.h"
#include "inout/cxopenandsavefilemanager.h"
#include "interface/modelinterface.h"

namespace creative_kernel
{
	PrinterNetMng::PrinterNetMng(QObject* parent)
		: QObject(parent)
		, m_udpPort(PrinterCommand::PortDesktopUdp)
		, m_findThreadIsAlive(false)
		, m_scanPrinterThread(nullptr)
	{
		m_udpSocket = new QUdpSocket(this);
		m_findPrinterSocket = new QUdpSocket(this);
		m_findPrinterSocket->bind(QHostAddress::Any, m_udpPort, QUdpSocket::ShareAddress);

		m_tcpSocket = new QTcpSocket(this);
		connect(this, SIGNAL(signalTcpConnect(QHostAddress)), this, SLOT(slotTcpConnect(QHostAddress)));
		qRegisterMetaType<QHostAddress>("QHostAddress");

		//
		m_payloadSize = 4 * 1024;   //4K
		m_totalBytes = 0;
		m_bytesWritten = 0;
		m_bytesToWrite = 0;
		connect(m_tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(updateClientProgress(qint64)));
		m_mapMacAndRow.clear();
	}
	
	PrinterNetMng::~PrinterNetMng()
	{
		clearPrinterDatas();
	}



	void PrinterNetMng::ScanPrinterThread::run()
	{
		qDebug() << "ScanPrinterThread start";
		m_socket->bind(QHostAddress::Any, m_printerNetMng->getUdpPort(), QUdpSocket::ShareAddress);
		m_printerNetMng->notifyFindThreadStatus(true);

		QByteArray datagram;
		QHostAddress targetaddr;
		while (!m_stop)
		{
			if (m_socket->hasPendingDatagrams())
			{
				datagram.resize(int(m_socket->pendingDatagramSize()));
				m_socket->readDatagram(datagram.data(), datagram.size(), &targetaddr);

				qDebug() << "receive " << datagram;

				m_printerNetMng->receivePrinterMsg(datagram, targetaddr);
			}
		}
		m_printerNetMng->notifyFindThreadStatus(false);
		m_socket->close();
		qDebug() << "FindPrinterThread over";
	}

	PrinterNetMng::ScanPrinterThread::ScanPrinterThread(PrinterNetMng* printerNetMng)
		: m_printerNetMng(printerNetMng)
		, m_stop(false)
	{
		m_socket = new QUdpSocket();
		
	}

	void PrinterNetMng::ScanPrinterThread::stopFind()
	{
		m_stop = true;
	}
	void PrinterNetMng::ScanPrinterThread::startFind()
	{
		m_stop = false;
	}



	void PrinterNetMng::sendError(QAbstractSocket::SocketError socketError)
	{
		//emit signalShowMsg("tcpsocket error " + socketError);
	}

	void PrinterNetMng::notifyFindThreadStatus(bool isAlive)
	{
		m_findThreadIsAlive = isAlive;
	}

	void PrinterNetMng::scanPrinter()
	{
		clearPrinterDatas();
		disconnect(m_findPrinterSocket, SIGNAL(readyRead()), this, SLOT(slotFindPrinter()));
		connect(m_findPrinterSocket, SIGNAL(readyRead()), this, SLOT(slotFindPrinter()));
		QString data = QString::number(PrinterCommand::CmdScanAsk) + "\t" + QString::number(m_udpPort);
		QByteArray datagram = data.toUtf8();

		m_udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, PrinterCommand::PortDeviceUdp);
		qDebug() << " datagram data =" << datagram;
	}

	void PrinterNetMng::receivePrinterMsg(QByteArray datagram, QHostAddress targetaddr)
	{
		QString data(datagram);

		qDebug() << data;
		//emit signalShowMsg(data);

		QStringList strlist = data.split('\t');
		int command = strlist.at(0).toInt();
		if (command == PrinterCommand::CmdPrinterOnLine)
		{
#ifdef _DEBUG
			qDebug() << "CmdPrinterOnLine";
#endif
			PrinterData pd;
			pd.m_mac = strlist.at(1);
			pd.m_printerName = strlist.at(2);
			pd.m_hostAddr = targetaddr;
			if (strlist.size() > 4)
			{
				pd.m_printerType = strlist.at(3);
				int printerState = strlist.at(4).toInt();
				if (printerState == 0)
				{
					pd.m_status = 0;
				}
				else if (printerState == 1)
				{
					pd.m_status = -1;
				}
				else
				{
					pd.m_status = -1;
				}
			}
			
			m_printerDatas[pd.m_mac] = pd;
			qDebug() << pd.toString();
			emit signalPrinterMsg(pd);
		}
		else if (command == PrinterCommand::CmdPrinterAcceptConnect)
		{
			//emit signalShowMsg("accept CmdPrinterAccentConnect");
			emit signalTcpConnect(targetaddr);
			qDebug() << "targetaddr =" << targetaddr;
			for (int i = 0; i < m_needPrinter.size(); i++)
			{
				if (m_needPrinter[i].m_hostAddr == targetaddr)
				{
					m_connectedPrinter.append(m_needPrinter[i]);
					m_needPrinter.erase(m_needPrinter.begin() + i);
					break;
				}
			}
		}
	}
	
	void PrinterNetMng::checkPrinters(QList<QString> devicesList)
	{
		m_needPrinter.clear();
		m_connectedPrinter.clear();

		QString connectline = QString::number(PrinterCommand::CmdConnectAsk);
		QByteArray datagram = connectline.toUtf8();
		for (QString mac : devicesList)
		{
			QMap<QString, PrinterData>::iterator it = m_printerDatas.find(mac);
			if (it != m_printerDatas.end())
			{
				const PrinterData& pd = it.value();
				m_udpSocket->writeDatagram(datagram, pd.m_hostAddr, PrinterCommand::PortDeviceUdp);
				m_needPrinter.append(pd);
				//emit signalShowMsg("sendto " + pd.toString());
			}
		}
	}

	void PrinterNetMng::stopScanPrinter()
	{
		m_scanPrinterThread->stopFind();
	}

	void PrinterNetMng::clearPrinterDatas()
	{
		m_printerDatas.clear();
	}

	int PrinterNetMng::numPrinters()
	{
		return m_printerDatas.size();
	}

	int PrinterNetMng::getUdpPort()
	{
		return m_udpPort;
	}

	//void PrinterNetMng::connectPrinter()
	//{
	//	//QString purename = m_realName;
	//	int pos = m_filename.lastIndexOf('/');
	//	QString strFileName = CXFILE.currOpenFile() + ".cxdlp";
	//	qDebug() << "strFileName ==" << strFileName;
	//	QString purename = m_filename.mid(pos + 1);
	//	purename = strFileName;
	//	QFile f(m_filename);
	//	f.open(QIODevice::ReadOnly);
	//	QByteArray block;

	//	QByteArray data = f.readAll();
	//	QByteArray fileNameByte = purename.toUtf8();
	//	//qDebug() << "1";

	//	QDataStream dts(&block, QIODevice::WriteOnly);
	//	dts.setVersion(QDataStream::Qt_5_0);
	//	dts << qint64(0) << qint64(0) << fileNameByte;//purename;
	//	dts.device()->seek(0);

	//	qDebug() << "2";
	//	qDebug() << "3:" << (qint64)(block.size());
	//	qDebug() << "33:" << (qint64)(block.size() + f.size());
	//	qDebug() << "33#:" << (block.size() - sizeof(qint64) * 2);
	//	dts << (qint64)(block.size() + f.size());
	//	dts << (qint64)(block.size() - sizeof(qint64) * 2);

	//	//qDebug() << "3";

	//	dts << fileNameByte;// purename;

	//	//qDebug() << "4";

	//	dts << data;

	//	//qDebug() << "senddata:" << block.length();

	//	//emit signalShowMsg("tcp write " + block.length());
	//	m_tcpSocket->write(block);
	//	m_tcpSocket->disconnectFromHost();
	//	bool ok = m_tcpSocket->waitForDisconnected();
	//	if (!ok) {
	//		qDebug() << "m_tcpSocket disconnected error";
	//	}
	//	 m_tcpSocket->close();
	//}

	void PrinterNetMng::slotTcpConnect(QHostAddress targetaddr)
	{
		//emit signalShowMsg("slotTcpConnect");

		m_tcpSocket->connectToHost(targetaddr, PrinterCommand::PortDeviceTcp);
		qDebug() << "state:" << m_tcpSocket->state();
		connectPrinter();

	}
	void PrinterNetMng::slotFindPrinter()
	{
		QByteArray datagram;
		QHostAddress targetaddr;
		while (m_findPrinterSocket->hasPendingDatagrams())
		{
			qDebug() << "receive message";
			datagram.resize(int(m_findPrinterSocket->pendingDatagramSize()));
			
			m_findPrinterSocket->readDatagram(datagram.data(), datagram.size(), &targetaddr);
			qDebug() << "targetaddr =" << targetaddr.toString();
			qDebug() << "receive " << datagram;

			receivePrinterMsg(datagram, targetaddr);
		}
	}
	void PrinterNetMng::setFileName(QString filename)
	{
		m_filename = filename;
	}
	void PrinterNetMng::connectPrinter()
	{
		m_bytesWritten = 0;
		QFileInfo fileInfo(m_filename);
		QString fileSuffix = fileInfo.suffix();
		m_localFile = new QFile(m_filename);
		if (!m_localFile->open(QFile::ReadOnly)) {
			qDebug() << "client：open file error!";
			return;
		}
		m_totalBytes = m_localFile->size();
		QDataStream sendOut(&m_outBlock, QIODevice::WriteOnly);
		sendOut.setVersion(QDataStream::Qt_5_7);
		QString currentFileName = CXFILE.currOpenFile() + "." + fileSuffix;// m_filename.right(m_filename.size() - m_filename.lastIndexOf('/') - 1);
		//文件总大小、文件名大小、文件名
		sendOut << qint64(0) << qint64(0) << currentFileName.toUtf8();
		m_totalBytes += m_outBlock.size();
		sendOut.device()->seek(0);
		sendOut << m_totalBytes;
		sendOut << qint64(m_outBlock.size() - sizeof(qint64) * 2);
		m_bytesToWrite = m_totalBytes - m_tcpSocket->write(m_outBlock);
		m_outBlock.resize(0);
		//    m_tcpSocket->disconnectFromHost();
		//    bool ok = m_tcpSocket->waitForDisconnected();
		//    if (!ok) {
		//        qDebug() << "m_tcpSocket disconnected error";
		//    }

		   // m_tcpSocket->close();
	}
	void PrinterNetMng::slotSelectMacAndRow(QMap<int, QString> mapRowAndMac)
	{
		m_needPrinter.clear();
		m_connectedPrinter.clear();

		QString connectline = QString::number(PrinterCommand::CmdConnectAsk);
		QByteArray datagram = connectline.toUtf8();
		m_mapMacAndRow.clear();
		m_mapMacAndRow = mapRowAndMac;
		startTcpSocketListen();
	}
	//add  printer data
	void PrinterNetMng::slotAddPrintnetData(QString name, QString ipAddr, QString macAddr)
	{
		PrinterData pd;
		pd.m_mac = macAddr;
		pd.m_printerName = name;
		pd.m_hostAddr = QHostAddress(ipAddr);
		m_printerDatas[pd.m_mac] = pd;
	}
	//delete select printer data
	void PrinterNetMng::slotDeletePrinterByMac(QList<QString> listMac)
	{
		for (QString mac : listMac)
		{
			m_printerDatas.remove(mac);
		}
	}
	void PrinterNetMng::startTcpSocketListen()
	{
		m_needPrinter.clear();
		m_connectedPrinter.clear();
		if (m_mapMacAndRow.size() == 0)return;
		QString connectline = QString::number(PrinterCommand::CmdConnectAsk);
		QByteArray datagram = connectline.toUtf8();
		QMap<int, QString>::iterator itMac = m_mapMacAndRow.begin();
		if (itMac != m_mapMacAndRow.end())
		{
			qDebug() << itMac.key() << "\t" << itMac.value();
			QMap<QString, PrinterData>::iterator it = m_printerDatas.find(itMac.value());
			m_currentRow = itMac.key();
			if (it != m_printerDatas.end())
			{
				const PrinterData& pd = it.value();
				qDebug() << "pd.m_hostAddr =" << pd.m_hostAddr.toString() << "  m_currentRow ="<< m_currentRow;
				m_udpSocket->writeDatagram(datagram, pd.m_hostAddr, PrinterCommand::PortDeviceUdp);
				m_needPrinter.append(pd);
			}
		}
	}

	void PrinterNetMng::updateClientProgress(qint64 numBytes) {
		m_bytesWritten += (int)numBytes;
		if (m_bytesToWrite > 0) {
			m_outBlock = m_localFile->read(qMin(m_bytesToWrite, m_payloadSize));
			m_bytesToWrite -= (int)m_tcpSocket->write(m_outBlock);
			m_outBlock.resize(0);
		}
		else {
			m_localFile->close();
		}
		//qDebug() << "m_bytesWritten / m_totalBytes =" << (int)(m_bytesWritten * 100 / m_totalBytes );
		emit signalProgressValue((int)(m_bytesWritten * 100 / m_totalBytes), m_currentRow);
		if (m_bytesWritten == m_totalBytes) {

			m_tcpSocket->disconnectFromHost();
			/*bool ok = m_tcpSocket->waitForDisconnected();
			if (!ok) {
			    qDebug() << "m_tcpSocket disconnected error";
			}*/
			m_tcpSocket->close();
			m_localFile->close();
			m_mapMacAndRow.remove(m_currentRow);
			startTcpSocketListen();
		}
	}
}


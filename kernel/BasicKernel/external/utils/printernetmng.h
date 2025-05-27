#ifndef CREATIVE_KERNEL_PRINTERNETMNG_1594459630592_H
#define CREATIVE_KERNEL_PRINTERNETMNG_1594459630592_H

#include "basickernelexport.h"
#include "data/printerdata.h"
#include <QtCore>
#include <QtNetwork>

namespace creative_kernel
{
	class BASIC_KERNEL_API PrinterNetMng : public QObject
	{
		Q_OBJECT

	protected:

		class ScanPrinterThread : public QThread
		{
		protected:
			QUdpSocket* m_socket;
			PrinterNetMng* m_printerNetMng;
			volatile bool m_stop;

		public:
			void run();

			ScanPrinterThread(PrinterNetMng* printerNetMng);

			void stopFind();
			void startFind();
			
		};

	public:
		PrinterNetMng(QObject* parent = nullptr);
		virtual ~PrinterNetMng();

		void scanPrinter();

		void clearPrinterDatas();

		int numPrinters();
		int getUdpPort();
		void setFileName(QString filename);
		void stopScanPrinter();
		void checkPrinters(QList<QString> devicesList);
		void startTcpSocketListen();
	protected:
		void notifyFindThreadStatus(bool isAlive);
		

		void receivePrinterMsg(QByteArray datagram, QHostAddress targetaddr);

	signals:
		void signalPrinterMsg(PrinterData pd);
		void signalTcpConnect(QHostAddress targetaddr);

		void signalProgressValue(int value,int row);
	public slots:
		void sendError(QAbstractSocket::SocketError socketError);
		void connectPrinter();
		void slotTcpConnect(QHostAddress targetaddr);
		void slotFindPrinter();
		void updateClientProgress(qint64 numBytes);

		void slotSelectMacAndRow(QMap<int, QString> mapRowAndMac);
		void slotAddPrintnetData(QString name, QString ipAddr, QString macAddr);
		void slotDeletePrinterByMac(QList<QString> listMac);
	protected:
		QUdpSocket* m_udpSocket;
		QUdpSocket* m_findPrinterSocket;
		QTcpSocket* m_tcpSocket;

		ScanPrinterThread* m_scanPrinterThread;

		int m_udpPort;

		QMap<QString, PrinterData> m_printerDatas;

		QList<PrinterData> m_needPrinter;
		QList<PrinterData> m_connectedPrinter;

		QString m_filename;
		QString m_realName;

		bool m_findThreadIsAlive;
		bool m_finishSend = false;
		int m_currentRow = 0;

		QMap<int, QString> m_mapMacAndRow;
	private:
		QFile* m_localFile;
		qint64 m_totalBytes;
		qint64 m_bytesWritten;
		qint64 m_bytesToWrite;
		qint64 m_payloadSize;
		QByteArray m_outBlock;

	};
}
#endif // CREATIVE_KERNEL_PRINTERNETMNG_1594459630592_H

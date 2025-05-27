#include <QDebug>
#include "qrcodeimageprovider.h"
//#include "crealitycloud/crealitycloudinterface.h"
#include "qjsondocument.h"
#include "interface/jobsinterface.h"
#include "loginjob.h"
#include "qrencode.h"

using namespace creative_kernel;
QPixmap QrCodeImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
{
	int width = 150;
	int height = 150;

	//QString identical = "";
	//QString expireTime = "";
	QString url = "";

	//if (!m_job)
	//{
	//	m_job = new LoginJob(m_parameterUI);
	//	m_jobPtr.reset(m_job);
	//}

	//bool state = getQrinfoFromCloud(identical, expireTime);

	url = "https://share.creality.com/scan-code?i=" + m_identical;

	//m_job->SetIdenticalStr(m_identical);
	//m_job->SetExpireTimeStr(m_expireTime);
	//m_isStop = false;
	//m_job->SetStopState(false);
	//if (creative_kernel::jobExecutorAvaillable())
	//{
	//	creative_kernel::executeJob(m_jobPtr);
	//}

	return Url2QrCodeImage(url, width, height);

	//if (state)
	//{
	//	
	//}
	//else
	//{
	//	qDebug() << "qrcodeImage Error";
	//	QPixmap err;
	//	return err;
	//}
}

QPixmap QrCodeImageProvider::Url2QrCodeImage(QString urlStr, int imageWidth, int imageHeight)
{
	QRcode* qrcode; //��ά������
	//QR_ECLEVEL_Q �ݴ��ȼ�
	qrcode = QRcode_encodeString(urlStr.toStdString().c_str(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);
	qint32 temp_width = imageWidth; //��ά��ͼƬ�Ĵ�С
	qint32 temp_height = imageHeight;
	qint32 qrcode_width = qrcode->width > 0 ? qrcode->width : 1;
	double scale_x = (double)temp_width / (double)qrcode_width; //��ά��ͼƬ�����ű���
	double scale_y = (double)temp_height / (double)qrcode_width;
	QImage mainimg = QImage(temp_width, temp_height, QImage::Format_ARGB32);
	QPainter painter(&mainimg);
	QColor background(Qt::white);
	painter.setBrush(background);
	painter.setPen(Qt::NoPen);
	painter.drawRect(0, 0, temp_width, temp_height);
	QColor foreground(Qt::black);
	painter.setBrush(foreground);
	for (qint32 y = 0; y < qrcode_width; y++)
	{
		for (qint32 x = 0; x < qrcode_width; x++)
		{
			unsigned char b = qrcode->data[y * qrcode_width + x];
			if (b & 0x01)
			{
				QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
				painter.drawRects(&r, 1);
			}
		}
	}
	QPixmap mainmap = QPixmap::fromImage(mainimg);



	return mainmap;
}

void QrCodeImageProvider::setparameterUI(CrealityCloudUI* param)
{
	m_parameterUI = param;
}

void QrCodeImageProvider::setDlgClosed(bool isClosed)
{
	m_isStop = isClosed;
	if (m_job != NULL)
	{
		m_job->SetStopState(isClosed);
	}
}

void QrCodeImageProvider::setIdentical(QString identical)
{
	m_identical = identical;
}

void QrCodeImageProvider::setExpireTime(QString expireTime)
{
	m_expireTime = expireTime;
}
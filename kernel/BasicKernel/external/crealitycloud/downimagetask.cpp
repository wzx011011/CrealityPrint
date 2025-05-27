#include "downimagetask.h"
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QDebug>

DownImageTask::DownImageTask(const QString imageUrl, const QString fileName, const QString filePath, const std::function<void(QString)>& call_back, bool isImage)
{
	m_imageUrl = imageUrl;
	m_fileName = fileName;
	m_filePath = filePath;
	call_back_ = call_back;
	m_IsImage = isImage;
}


DownImageTask::~DownImageTask()
{
}

void DownImageTask::run()
{
	//ѭ��ƴ��
	QString baseUrl = m_imageUrl;
	//��������
	QNetworkRequest request;
	request.setUrl(QUrl(baseUrl));
	QNetworkAccessManager* manager = new QNetworkAccessManager();
	// ��������
	QNetworkReply* pReplay = manager->get(request);
	//����һ���ֲ����¼�ѭ�����ȴ���Ӧ�������˳�
	QEventLoop eventLoop;
	QObject::connect(pReplay, SIGNAL(finished()), &eventLoop, SLOT(quit()));
	eventLoop.exec();

	if (pReplay->error() == QNetworkReply::NoError)
	{
		qInfo() << QString("request imgFile NoError");
		//��ȡ��Ӧ��Ϣ
		QByteArray bytes = pReplay->readAll();
		QFile file;
		file.setFileName(m_filePath);
		if (file.open(QIODevice::WriteOnly))
		{
			file.write(bytes);
			file.close();
		}
		else
		{
			qDebug() << "file open failed";
		}
		qDebug() << "DownImageTask success m_IsImage:"<< m_IsImage << " filePath: " << m_filePath;
		if (m_IsImage)
		{
			call_back_(m_fileName);
		}
		else
		{
			call_back_(m_filePath);
		}
		
	}
	else
	{
		qDebug() << "DownImageTask error";
	}
}
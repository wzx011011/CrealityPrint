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
	//循环拼接
	QString baseUrl = m_imageUrl;
	//构造请求
	QNetworkRequest request;
	request.setUrl(QUrl(baseUrl));
	QNetworkAccessManager* manager = new QNetworkAccessManager();
	// 发送请求
	QNetworkReply* pReplay = manager->get(request);
	//开启一个局部的事件循环，等待响应结束，退出
	QEventLoop eventLoop;
	QObject::connect(pReplay, SIGNAL(finished()), &eventLoop, SLOT(quit()));
	eventLoop.exec();

	if (pReplay->error() == QNetworkReply::NoError)
	{
		qInfo() << QString("request imgFile NoError");
		//获取响应信息
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
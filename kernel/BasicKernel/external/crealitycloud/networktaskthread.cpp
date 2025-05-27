#include "networktaskthread.h"
#include <QDebug>
#include<fstream>
#include <string>
#include<sstream>
#include <QFile>
#include <QTextCodec>
#include <QSettings>
#include <QUuid>
#include <QtNetwork>


NetworkTaskThread::NetworkTaskThread(QObject* parent)
{
	m_app_version = "4.0.0";
	m_url = "";
	m_body = "";
	m_request_type = 1;
}

void NetworkTaskThread::setCallbackFun(const std::function<void(QByteArray)>& call_back)
{
	m_call_back = call_back;
}

void NetworkTaskThread::setRequestData(const QMap<QString, QString>& data)
{
	if (data.find("__REQUEST_TYPE__") != data.end())
	{
		m_request_type = data.value("__REQUEST_TYPE__").toInt();
	}

	QString request_url = "";
	if (data.find("__REQUEST_URl__") != data.end())
	{
		request_url = data.value("__REQUEST_URl__");
	}
	if (m_request_type == 3 || m_request_type == 4)
	{
		setRequestUrl(request_url);
	}
	else 
	{
		setRequestUrl(getCloudUrl() + request_url);
	}

	QString request_body = "";
	if (data.find("__REQUEST_BODY__") != data.end())
	{
		request_body = data.value("__REQUEST_BODY__");
	}
	setRequestBody(request_body);

	QString request_token = "";
	if (data.find("__REQUEST_TOKEN__") != data.end())
	{
		request_token = data.value("__REQUEST_TOKEN__");
	}
	QString request_user_id = "";
	if (data.find("__REQUEST_USER_ID__") != data.end())
	{
		request_user_id = data.value("__REQUEST_USER_ID__");
	}

	QString request_timeOut = "10000";
	if (data.find("__REQUEST_TIME_OUT__") != data.end())
	{
		request_timeOut = data.value("__REQUEST_TIME_OUT__");
	}
	setRequestTimeOut(request_timeOut.toInt());
	
	QUuid uuid = QUuid::createUuid();
	QString requestID = uuid.toString();
	setRequestHeader(getOSVersion(), getDUID(), getCurrentLanguage(), "creality_model", "11", requestID, m_app_version, request_token, request_user_id);
}

QString NetworkTaskThread::getOSVersion()
{
	QString os_version = "Win";
#ifdef __APPLE__
	os_version = "Mac";
#endif
	return os_version;
}

QString NetworkTaskThread::getDUID()
{
	QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();
	int nCnt = nets.count();
	QString strMacAddr = "";
	for (int i = 0; i < nCnt; i++)
	{
		if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
		{
			strMacAddr = nets[i].hardwareAddress();
			break;
		}
	}
	return strMacAddr;
}

QString NetworkTaskThread::getCurrentLanguage()
{
	QSettings setting;
	setting.beginGroup("language_perfer_config");
	QString lang = setting.value("language_type", "zh_cn").toString();
	setting.endGroup();

	//please refer http://yapi.crealitygroup.com/project/8/wiki
	if (lang == "en.ts")
	{
		return "0";
	}
	else if (lang == "zh_CN.ts")
	{
		return "1";
	}
	else if (lang == "zh_TW.ts")
	{
		return "2";
	}
	else
	{
		return "0";
	}
}

QString NetworkTaskThread::getCloudUrl()
{
	//����
	QString urlStr = "https://model-admin.crealitygroup.com";
#ifdef CLOUD_BETA_URL
	QSettings setting;
	setting.beginGroup("profile_setting");
	QString strStartType = setting.value("service_type", "-1").toString();
	if (strStartType == "-1")
	{
		strStartType = "0";
		setting.setValue("service_type", "0");
	}
	setting.endGroup();

	if (strStartType == "0")
	{
		//����
		urlStr = "https://model-admin.crealitygroup.com";
	}
	else
	{
		//����
		urlStr = "https://model-admin2.creality.com";
	}
#endif
	return urlStr;
}

void NetworkTaskThread::setRequestHeader(QString osVer, QString duid, QString osLang, QString appId, QString platform, QString requestId, QString appVer, QString token, QString uid)
{

	m_head.insert(std::make_pair("__CXY_OS_VER_", osVer.toStdString().c_str()));
	m_head.insert(std::make_pair("__CXY_DUID_", duid.toStdString().c_str()));
	m_head.insert(std::make_pair("__CXY_OS_LANG_", osLang.toStdString().c_str()));
	m_head.insert(std::make_pair("__CXY_APP_ID_", appId.toStdString().c_str()));
	m_head.insert(std::make_pair("__CXY_PLATFORM_", platform.toStdString().c_str()));
	m_head.insert(std::make_pair("__CXY_REQUESTID_", requestId.toStdString().c_str()));
	m_head.insert(std::make_pair("__CXY_APP_VER_", appVer.toStdString().c_str()));

	if (token != "")
	{
		m_head.insert(std::make_pair("__CXY_TOKEN_", token.toStdString().c_str()));
	}
	if (uid != "")
	{
		m_head.insert(std::make_pair("__CXY_UID_", uid.toStdString().c_str()));
	}
}

void NetworkTaskThread::run()
{
	if (m_request_type == 1 || m_request_type == 4)
	{
		m_httpnet.setContentTypeHeader();
		m_httpnet.setUrl(m_url.toStdString());
		m_httpnet.setRawHeader(m_head);
		m_httpnet.setBody(m_body.toStdString());
		m_httpnet.setTimeOut(m_timeOut);

		m_call_back(m_httpnet.post());
	}
	else if (m_request_type == 2)
	{
		m_httpnet.setContentTypeHeader();
		m_httpnet.setUrl(m_url.toStdString());
		m_httpnet.setRawHeader(m_head);
		m_httpnet.setTimeOut(m_timeOut);
		m_call_back(m_httpnet.get_string());
	}
	else if (m_request_type == 3)
	{
		m_httpnet.setUrl(m_url.toStdString());
		m_httpnet.setTimeOut(m_timeOut);
		m_call_back(m_httpnet.getNoBaseUrl());
	}

	return;
}

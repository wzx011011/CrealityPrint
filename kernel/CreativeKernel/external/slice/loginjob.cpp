#include "loginjob.h"
#include <QDebug>
#include <QDateTime>
#include <QtCore/QSettings>

LoginJob::LoginJob(CrealityCloudUI* theOp,QObject* parent):Job(parent)
{
	m_Op = theOp;
	m_stop = false;
	m_strIdentical = "";
	m_nExpireTime = 0;
	m_rst = -1;
}

LoginJob::~LoginJob()
{

}

void LoginJob::SetIdenticalStr(QString identicalStr)
{
	m_strIdentical = identicalStr;
}

void LoginJob::SetExpireTimeStr(QString expireTimeStr)
{
	m_nExpireTime = expireTimeStr.toInt();
}

void LoginJob::SetStopState(bool isStop)
{
	m_stop = isStop;
}

QString LoginJob::name()
{
	return "LoginJob";
}

QString LoginJob::description()
{
	return "get userinfo from creative cloud";
}

void LoginJob::work(qtuser_core::Progressor* progressor)
{
	int i = 0;
	while (1)
	{
		//int rst = creative_kernel::getQrLoginStateFromCloud(m_strIdentical, m_userinfo);
		if (m_rst == 3)
		{
			qDebug() << "login success by qrcode";
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			break;
		}

		QString strContent = QString::fromLatin1("{\"identical\": \"%1\"}").arg(m_strIdentical);
		QString strurl = "/api/cxy/account/v2/qrQuery";

		std::function<void(QByteArray)> call_back = std::bind(
			&LoginJob::funcQrLoginStateFromCloud, this, std::placeholders::_1);

		QMap<QString, QString> data;
		data.insert("__REQUEST_URl__", strurl);
		data.insert("__REQUEST_TYPE__", "1");
		data.insert("__REQUEST_BODY__", strContent);
		creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);

		QDateTime time = QDateTime::currentDateTime();   //获取当前时间  
		int timeT = time.toTime_t();   //将当前时间转为时间戳

		if (m_rst == 4 || m_rst == 5)
		{
			if (m_Op)
			{
				m_Op->loginUpdate();
			}
		}

		if (m_stop)
		{
			progressor->failed("login dialog closed");
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

}

void LoginJob::failed()
{
	qDebug() << "login failed by qrcode";
}

void LoginJob::successed(qtuser_core::Progressor* progressor)
{
	if (m_Op)
	{
		QSettings setting;
		setting.beginGroup("profile_setting");
		setting.setValue("login_auto", "1");
		setting.setValue("login_token", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
		setting.setValue("login_userid", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
		setting.endGroup();

		m_Op->loginSuccess(creative_kernel::getcxNetworkManager()->getLoginInfoToken(), creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
	}
}

void LoginJob::funcQrLoginStateFromCloud(const QByteArray& response)
{
	QJsonParseError error;

	QByteArray arrayJson = response;
	const QJsonDocument document = QJsonDocument::fromJson(arrayJson, &error);
	if (error.error != QJsonParseError::NoError)
	{
		qWarning() << "[funcQrLoginStateFromCloud QJsonDocument]" << error.errorString() << "\n";
		return;
	}
	QString strJson(document.toJson(QJsonDocument::Compact));

	QJsonObject object = document.object();
	const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
	int state = result.value(QString::fromLatin1("state")).toInt();
	QVariant nUserID = result.value(QString::fromLatin1("userId")).toVariant();
	qDebug() << "funcQrLoginStateFromCloud state: " << state;
	if (state == 3)
	{
		creative_kernel::getcxNetworkManager()->setloginInfo(1, result.value(QString::fromLatin1("token")).toString(), nUserID.toString());
	}
	m_rst = state;
}

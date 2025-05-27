#include "deviceinfotaskoldiot.h"
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include "queryequipmentinfotask.h"
#include "crealitycloud/cxnetworkmanager.h"


//iot sign begin
QString DeviceInfoTaskOldIot::hmacSha1(QByteArray key, QByteArray baseString)
{
	int blockSize = 64; // HMAC-SHA-1 block size, defined in SHA-1 standard
	if (key.length() > blockSize) { // if key is longer than block size (64), reduce key length with SHA-1 compression
		key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
	}
	QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char"6"
	QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char"/"
	// ascii characters 0x36 ("6") and 0x5c ("/") are selected because they have large
	// Hamming distance (http://en.wikipedia.org/wiki/Hamming_distance)
	for (int i = 0; i < key.length(); i++) {
		innerPadding[i] = innerPadding[i] ^ key.at(i); // XOR operation between every byte in key and innerpadding, of key length
		outerPadding[i] = outerPadding[i] ^ key.at(i); // XOR operation between every byte in key and outerpadding, of key length
	}
	// result = hash ( outerPadding CONCAT hash ( innerPadding CONCAT baseString ) ).toBase64
	QByteArray total = outerPadding;
	QByteArray part = innerPadding;
	part.append(baseString);
	total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));
	QByteArray hashed = QCryptographicHash::hash(total, QCryptographicHash::Sha1);
	return hashed.toBase64();
}

QString DeviceInfoTaskOldIot::currentTimeToUTC()
{
	QDateTime time = QDateTime::currentDateTime().toUTC();
	return time.toString("yyyy-MM-ddThh:mm:ssZ");
}

int DeviceInfoTaskOldIot::randTime()
{
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
	return qrand();
}

QString DeviceInfoTaskOldIot::percentEncodeUrl(QString value)
{
	return value.toUtf8().replace("&", "%26")
		.replace("=", "%3D")
		.replace(":", "%3A")
		.replace("/", "%2F")
		.replace("+", "%2B")
		.replace("*", "%2A")
		.replace("{", "%7B")
		.replace("}", "%7D")
		.replace("\"", "%22")
		.replace(",", "%2C")
		.replace(" ", "%20");
}

QString DeviceInfoTaskOldIot::percentEncodeToken(QString value)
{
	return value.toUtf8().replace("%", "%25");
}


QString DeviceInfoTaskOldIot::generateQueryString(QMap<QString, QString> parameter, bool isEncodeKV)
{
	QMap<QString, QString>::iterator iter = parameter.begin();
	QString tmpStr = "";
	while (iter != parameter.end())
	{
		if (isEncodeKV)
		{
			tmpStr.append(percentEncodeUrl(iter.key())).append("=").append(percentEncodeUrl(iter.value())).append("&");
		}
		else {
			tmpStr.append(iter.key()).append("=").append(iter.value()).append("&");
		}

		iter++;

	}

	tmpStr = tmpStr.left(tmpStr.length() - 1);

	return tmpStr;
}

QString DeviceInfoTaskOldIot::generateSignString(QString method, QMap<QString, QString> parameter)
{
	if (method == nullptr)
	{
		return "";
	}
	QString queryStr = generateQueryString(parameter, true);
	QString stringTosign = "";
	stringTosign.append(method).append("&");
	stringTosign.append(percentEncodeUrl("/")).append("&");
	stringTosign.append(percentEncodeUrl(queryStr));

	return stringTosign;
}

QString DeviceInfoTaskOldIot::generate(QString method, QMap<QString, QString> parameter, QString accessKeySecret)
{
	QString signString = generateSignString(method, parameter);
	return hmacSha1((accessKeySecret + "&").toUtf8(), signString.toUtf8());
}
//end


DeviceInfoTaskOldIot::DeviceInfoTaskOldIot(QueryEquipmentInfoTask* parent)
{
	m_op = parent;
}


DeviceInfoTaskOldIot::~DeviceInfoTaskOldIot()
{
}

void DeviceInfoTaskOldIot::run()
{
	getAliyunInfo();
	getAliyunInfoStatus();

	while (m_reply < 2)
	{
		QThread::msleep(1000);
	}
}

void DeviceInfoTaskOldIot::setProductKey(QString pn)
{
	m_productKey = pn;
}

void DeviceInfoTaskOldIot::setDeviceName(QString dn)
{
	m_deviceName = dn;
}

void DeviceInfoTaskOldIot::setIotId(QString iotId)
{
	m_iotId = iotId;
}

void DeviceInfoTaskOldIot::getAliyunInfo()
{
	QString strContent = "{}";
	QString strurl = "/api/cxy/account/v2/getAliyunInfo";
	std::function<void(QByteArray)> call_back = std::bind(
		&DeviceInfoTaskOldIot::funcGetAliyunInfo, this, std::placeholders::_1);
	QMap<QString, QString> data;
	data.clear();
	data.insert("__REQUEST_URl__", strurl);
	data.insert("__REQUEST_TYPE__", "1");
	data.insert("__REQUEST_BODY__", strContent.toUtf8());
	data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
	data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
	creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
}

void DeviceInfoTaskOldIot::getAliyunInfoStatus()
{
	QString strContent = "{}";
	QString strurl = "/api/cxy/account/v2/getAliyunInfo";
	std::function<void(QByteArray)> call_back = std::bind(
		&DeviceInfoTaskOldIot::funcGetAliyunInfoStatus, this, std::placeholders::_1);
	QMap<QString, QString> data;
	data.clear();
	data.insert("__REQUEST_URl__", strurl);
	data.insert("__REQUEST_TYPE__", "1");
	data.insert("__REQUEST_BODY__", strContent.toUtf8());
	data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
	data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
	creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
}

void DeviceInfoTaskOldIot::funcGetAliyunInfo(const QByteArray& response)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));

		QJsonObject object = document.object();
		const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
		const QJsonObject aliyunInfo = result.value(QString::fromLatin1("aliyunInfo")).toObject();
		int state = object.value(QString::fromLatin1("code")).toInt();

		if (state == 0)
		{
			QString accessKeyId = aliyunInfo.value(QString::fromLatin1("accessKeyId")).toString();
			QString secretAccessKey = aliyunInfo.value(QString::fromLatin1("secretAccessKey")).toString();
			QString sessionToken = aliyunInfo.value(QString::fromLatin1("sessionToken")).toString();

			QMap<QString, QString> para;
			//public
			QString timeTmp = currentTimeToUTC();
			para.insert("Action", "QueryDevicePropertyStatus");
			para.insert("Format", "JSON");
			para.insert("Version", "2018-01-20");
			para.insert("SignatureMethod", "HMAC-SHA1");
			para.insert("SignatureNonce", QString::number(randTime()));
			para.insert("SignatureVersion", "1.0");
			para.insert("AccessKeyId", accessKeyId);
			para.insert("SecurityToken", percentEncodeToken(percentEncodeUrl(sessionToken)));
			para.insert("Timestamp", percentEncodeToken(percentEncodeUrl(timeTmp)));
			para.insert("RegionId", "cn-shanghai");

			//request
			para.insert("ProductKey", m_productKey);
			para.insert("DeviceName", m_deviceName);
			para.insert("IotId", m_iotId);

			QString signString = generate("GET", para, secretAccessKey);

			para.insert("Signature", signString);
			para.insert("SecurityToken", sessionToken);
			para.insert("Timestamp", timeTmp);

			QString strurl = "https://iot.cn-shanghai.aliyuncs.com/?" + generateQueryString(para, true);

			std::function<void(QByteArray)> call_back = std::bind(
				&DeviceInfoTaskOldIot::funcQueryDevicePropertyStatus, this, std::placeholders::_1);
			QMap<QString, QString> data;
			data.clear();
			data.insert("__REQUEST_URl__", strurl);
			data.insert("__REQUEST_TYPE__", "3");
			creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
		}
	}
}

void DeviceInfoTaskOldIot::funcGetAliyunInfoStatus(const QByteArray& response)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));

		QJsonObject object = document.object();
		const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
		const QJsonObject aliyunInfo = result.value(QString::fromLatin1("aliyunInfo")).toObject();
		int state = object.value(QString::fromLatin1("code")).toInt();

		if (state == 0)
		{
			QString accessKeyId = aliyunInfo.value(QString::fromLatin1("accessKeyId")).toString();
			QString secretAccessKey = aliyunInfo.value(QString::fromLatin1("secretAccessKey")).toString();
			QString sessionToken = aliyunInfo.value(QString::fromLatin1("sessionToken")).toString();

			QMap<QString, QString> para;
			//public
			QString timeTmp = currentTimeToUTC();
			para.insert("Action", "GetDeviceStatus");
			para.insert("Format", "JSON");
			para.insert("Version", "2018-01-20");
			para.insert("SignatureMethod", "HMAC-SHA1");
			para.insert("SignatureNonce", QString::number(randTime()));
			para.insert("SignatureVersion", "1.0");
			para.insert("AccessKeyId", accessKeyId);
			para.insert("SecurityToken", percentEncodeToken(percentEncodeUrl(sessionToken)));
			para.insert("Timestamp", percentEncodeToken(percentEncodeUrl(timeTmp)));
			para.insert("RegionId", "cn-shanghai");

			//request
			para.insert("ProductKey", m_productKey);
			para.insert("DeviceName", m_deviceName);
			para.insert("IotId", m_iotId);

			QString signString = generate("GET", para, secretAccessKey);

			para.insert("Signature", signString);
			para.insert("SecurityToken", sessionToken);
			para.insert("Timestamp", timeTmp);

			QString strurl = "https://iot.cn-shanghai.aliyuncs.com/?" + generateQueryString(para, true);

			std::function<void(QByteArray)> call_back = std::bind(
				&DeviceInfoTaskOldIot::funcGetDeviceStatus, this, std::placeholders::_1);
			QMap<QString, QString> data;
			data.clear();
			data.insert("__REQUEST_URl__", strurl);
			data.insert("__REQUEST_TYPE__", "3");
			creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);

		}
	}
}

void DeviceInfoTaskOldIot::funcQueryDevicePropertyStatus(const QByteArray& response)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));
		m_op->setDeviceInformation(strJson.toStdString(), m_deviceName.toStdString(), false);
	}
	m_reply++;
}

void DeviceInfoTaskOldIot::funcGetDeviceStatus(const QByteArray& response)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));
		m_op->setDeviceStatus(strJson.toStdString(), m_deviceName.toStdString());
	}
	m_reply++;
}

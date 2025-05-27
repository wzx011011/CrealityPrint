#include "queryequipmentinfotask.h"
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
#include "crealitycloud/cxnetworkmanager.h"
#include "rapidjson/document.h"

QueryEquipmentInfoTask::QueryEquipmentInfoTask(CrealityCloudUI* parent)
	:m_op(parent)
{
}


QueryEquipmentInfoTask::~QueryEquipmentInfoTask()
{
}

void QueryEquipmentInfoTask::run()
{
	qDebug() << "QueryEquipmentInfoTask run() begin ";
	while (1)
	{
		if (is_stop)
		{
			qDebug() << "QueryEquipmentInfoTask break one ";
			break;
		}

		m_mtxList.lock();
		if (m_info.empty())
		{
			m_mtxList.unlock();
			QThread::msleep(100);
			continue;
		}
		for (const auto& deviceInfo : m_info)
		{
			const auto& iotType = std::get<0>(deviceInfo);
			if (iotType == 1)
			{
				const auto& productKey = std::get<1>(deviceInfo);
				const auto& deviceName = std::get<2>(deviceInfo);
				const auto& iotId = std::get<3>(deviceInfo);
				creative_kernel::getcxNetworkManager()->queryDevicePropertyStatus(productKey, deviceName, iotId,
					std::bind(&QueryEquipmentInfoTask::setDeviceInformation, this, std::placeholders::_1, deviceName, true));

				creative_kernel::getcxNetworkManager()->getDeviceStatus(productKey, deviceName, iotId,
					std::bind(&QueryEquipmentInfoTask::setDeviceStatus, this, std::placeholders::_1, deviceName));
			}
			if (iotType == 2)
			{
				const auto& tbId = std::get<4>(deviceInfo);
				const auto& deviceName = std::get<2>(deviceInfo);

				creative_kernel::getcxNetworkManager()->queryDevicePropertyStatus_NewIOT(tbId,
					std::bind(&QueryEquipmentInfoTask::setDeviceInformation, this, std::placeholders::_1, deviceName, true));

				creative_kernel::getcxNetworkManager()->queryDevicePropertyStatus_NewIOT_RealTime(tbId,
					std::bind(&QueryEquipmentInfoTask::setDeviceInformationRealTime, this, std::placeholders::_1, deviceName));

				QString strContent = QString("{\"method\":\"get\",\"params\":{\"ReqPrinterPara\":1}}");
				creative_kernel::getcxNetworkManager()->queryDevicePropertyStatus_NewIOT_Position(strContent.toStdString(), tbId,
					std::bind(&QueryEquipmentInfoTask::setDeviceInformationPosition, this, std::placeholders::_1, deviceName));
			}
		}
		m_mtxList.unlock();
		if (is_stop)
		{
			qDebug() << "QueryEquipmentInfoTask break three ";
			break;
		}
		QThread::msleep(10*1000);
	}
	qDebug() << "QueryEquipmentInfoTask run() end ";
}

void QueryEquipmentInfoTask::setEquipmentInfo(const std::string& infomation)
{
	rapidjson::Document doc;
	doc.Parse(infomation.c_str());
	if (doc.HasParseError())
		return;

	int state = -1;
	if (doc.HasMember("code") && doc["code"].IsInt())
	{
		state = doc["code"].GetInt();
	}
	if (state != 0)
	{
		return;
	}
	int count = 0;

	if (doc.HasMember("result") && doc["result"].IsObject())
	{
		const auto& resultObj = doc["result"];
		if (resultObj.HasMember("count") && resultObj["count"].IsInt())
		{
			count = resultObj["count"].GetInt();
			if (count == 0)
			{
				return;
			}
			m_mtxList.lock();
			m_info.clear();
			m_mtxList.unlock();
			if (resultObj.HasMember("list") && resultObj["list"].IsArray())
			{
				const auto& array = resultObj["list"];
				for (size_t i = 0; i < array.Size(); i++)
				{
					std::string productKey;
					std::string deviceName;
					std::string iotId;
					std::string tbId;
					int iotType;
					const auto& device = array[i];
					if (device.HasMember("productKey") && device["productKey"].IsString())
					{
						productKey = device["productKey"].GetString();
					}
					if (device.HasMember("deviceName") && device["deviceName"].IsString())
					{
						deviceName = device["deviceName"].GetString();
					}
					if (device.HasMember("iotId") && device["iotId"].IsString())
					{
						iotId = device["iotId"].GetString();
					}
					if (device.HasMember("tbId") && device["tbId"].IsString())
					{
						tbId = device["tbId"].GetString();
					}
					if (device.HasMember("iotType") && device["iotType"].IsInt())
					{
						iotType = device["iotType"].GetInt();
					}
					m_mtxList.lock();
					m_info.push_back(std::make_tuple(iotType, productKey, deviceName, iotId, tbId));
					m_mtxList.unlock();
				}
			}
		}
	}
}

void QueryEquipmentInfoTask::setStop(bool isStop)
{
	is_stop = isStop;
}

void QueryEquipmentInfoTask::setDeviceStatus(const std::string& status, const std::string& id)
{
	m_op->setDeviceInformation(status, id);
}

void QueryEquipmentInfoTask::setDeviceInformation(const std::string& date, const std::string& id, bool isNewIOT)
{
	m_op->setDeviceInformation(date, id, isNewIOT);
}

void QueryEquipmentInfoTask::setDeviceInformationRealTime(const std::string& date, const std::string& id)
{
	m_op->setDeviceInformationRealTime(date, id);
}

void QueryEquipmentInfoTask::setDeviceInformationPosition(const std::string& date, const std::string& id)
{
	m_op->setDeviceInformationPosition(date, id);
}
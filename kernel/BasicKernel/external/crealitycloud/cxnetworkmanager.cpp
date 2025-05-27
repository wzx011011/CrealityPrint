#include "cxnetworkmanager.h"
#include "rapidjson/document.h"

namespace creative_kernel 
{
	SINGLETON_IMPL(cxNetworkManager)
	cxNetworkManager::cxNetworkManager()
	{
		isInited = false;
	}

	cxNetworkManager::~cxNetworkManager()
	{
	}

	void cxNetworkManager::init()
	{
		if (isInited)
		{
			return;
		}
		managerThread.setRunningState(true);
		managerThread.start();

		auto t = std::thread(&cxNetworkManager::refreshStateThread, this);
		t.detach();
		isInited = true;
	}

	void cxNetworkManager::setThreadPool(QThreadPool* pool)
	{
		managerThread.setThreadPool(pool);
	}

	void cxNetworkManager::uninit()
	{
		managerThread.setRunningState(false);
		isInited = false;
		m_bExit = true;
	}

	bool cxNetworkManager::addRequestTask(int type, const QMap<QString, QString>& data, const std::function<void(QByteArray)>& call_back, const std::function<void(QByteArray)>& call_back1)
	{
		if (isInited)
		{
			managerThread.addRequestInfoList(type, data, call_back, call_back1);
			return true;
		}
		else
		{
			return false;
		}
	}

	std::string cxNetworkManager::getCloudUrl()
	{
		QString urlStr = "https://www.crealitycloud.cn";
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
			urlStr = "https://www.crealitycloud.cn";
		}
		else
		{
			urlStr = "https://www.crealitycloud.com";
		}
#endif
		return urlStr.toStdString();
	}

	std::string cxNetworkManager::getDUID()
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
		return strMacAddr.toStdString();
	}

	cpr::Header cxNetworkManager::getHeader()
	{
		cpr::Header header;
		header.insert({ "__CXY_OS_VER_", "Win" });
		header.insert({ "__CXY_DUID_", getDUID() });
		header.insert({ "__CXY_OS_LANG_", "1" });
		header.insert({ "__CXY_APP_ID_", "creality_model" });
		header.insert({ "__CXY_PLATFORM_", "11" });
		header.insert({ "__CXY_REQUESTID_", QUuid::createUuid().toString().toStdString() });
		header.insert({ "__CXY_APP_VER_", "4.0.0" });
		if (!getLoginInfoToken().isEmpty())
		{
			header.insert({ "__CXY_TOKEN_", getLoginInfoToken().toStdString() });
		}
		if (!getLoginInfoUserID().isEmpty())
		{
			header.insert({ "__CXY_UID_", getLoginInfoUserID().toStdString() });
		}
		header.insert({ "Content-Type", "application/json" });
		return header;
	}

	void cxNetworkManager::refreshStateThread()
	{
		while (!m_bExit)
		{
			if (!m_loginInfo.loginState)//����û����
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				continue;
			}
			time_t tmNow = time(nullptr);
			m_mutex.lock();
			if (tmLastActive == 0 || abs(tmNow - tmLastActive) > 3)
			{
				tmLastActive = tmNow;
				m_mutex.unlock();
				getDeviceList(funcDeviceListCb);
				getTaskList(funcTaskListCb);
			}
			else
			{
				m_mutex.unlock();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

	void cxNetworkManager::updateActiveTime(const time_t& tm)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		tmLastActive = tm;
	}

	void cxNetworkManager::getDeviceList(std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cDeviceListSuffix;
		cpr::Header header = getHeader();

		auto future_text = cpr::GetCallback([](cpr::Response r) {
			return r.text;
			}, cpr::Url{ strUrl },
				header);

		if (future_text.wait_for(std::chrono::seconds(10)) == std::future_status::ready)
		{
			auto ret = future_text.get();
			if (ret.empty())
			{
				return;
			}
			if (call_back)
			{
				call_back(ret);
			}
		}
	}

	void cxNetworkManager::getTaskList(std::function<void(std::string)> call_back)
	{
		std::string strContent = "{\"status\": \"\"}";
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cTaskListSuffix;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		auto future_text = cpr::PostCallback([](cpr::Response r) {
			return r.text;
			}, cpr::Url{ strUrl }, payload,
				header);

		if (future_text.wait_for(std::chrono::seconds(10)) == std::future_status::ready)
		{
			auto ret = future_text.get();
			if (ret.empty())
			{
				return;
			}
			if (call_back)
			{
				call_back(ret);
			}
		}
	}

	void cxNetworkManager::confirmDeviceIdle(const std::string& deviceName, std::function<void(std::string)> call_back)
	{
		std::string strUrl = getCloudUrl() + cConfirmDeviceIdleSuffix + deviceName;
		cpr::Header header = getHeader();

		auto future_text = cpr::GetCallback([](cpr::Response r) {
			return r.text;
			}, cpr::Url{ strUrl },
				header);
		updateActiveTime(0);//ˢ������
		if (future_text.wait_for(std::chrono::seconds(10)) == std::future_status::ready)
		{
			auto ret = future_text.get();
			if (ret.empty())
			{
				return;
			}
			if (call_back)
			{
				call_back(ret);
			}
		}
	}

	void cxNetworkManager::cancelTask(const std::string& deviceName, std::function<void(std::string)> call_back)
	{
		std::string strUrl = getCloudUrl() + cCancelTaskSuffix + deviceName;
		cpr::Header header = getHeader();

		auto future_text = cpr::GetCallback([](cpr::Response r) {
			return r.text;
			}, cpr::Url{ strUrl },
				header);
		updateActiveTime(0);//ˢ������
		if (future_text.wait_for(std::chrono::seconds(10)) == std::future_status::ready)
		{
			auto ret = future_text.get();
			if (ret.empty())
			{
				return;
			}
			if (call_back)
			{
				call_back(ret);
			}
		}
	}

	void cxNetworkManager::addTask(const std::string& strContent, std::function<void(bool)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cAddTaskSuffix;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		auto future_text = cpr::PostCallback([](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			if (doc.HasParseError())
				return false;

			if (doc.HasMember("code") && doc["code"].IsInt())
			{
				if(doc["code"].GetInt() == 0)
					return true;
			}
			return false;
			}, cpr::Url{ strUrl }, payload,
				header);
		updateActiveTime(0);//ˢ������

		if (future_text.wait_for(std::chrono::seconds(10)) == std::future_status::ready)
		{
			auto ret = future_text.get();
			if (call_back)
			{
				call_back(ret);
			}
		}
	}
	void cxNetworkManager::getGcodeList(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetGcodeListSuffix;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		auto future_text = cpr::PostCallback([](cpr::Response r) {
			return r.text;
			}, cpr::Url{ strUrl }, payload,
				header);
		updateActiveTime(time(nullptr));

		if (future_text.wait_for(std::chrono::seconds(10)) == std::future_status::ready)
		{
			auto ret = future_text.get();
			if (ret.empty())
			{
				return;
			}
			if (call_back)
			{
				call_back(ret);
			}
		}
	}
	void cxNetworkManager::getPrintReport(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetPrintReportSuffix + strContent;

		cpr::Header header = getHeader();

		auto future_text = cpr::GetCallback([](cpr::Response r) {
			return r.text;
			}, cpr::Url{ strUrl }, header);
		updateActiveTime(time(nullptr));

		if (future_text.wait_for(std::chrono::seconds(10)) == std::future_status::ready)
		{
			auto ret = future_text.get();
			if (ret.empty())
			{
				return;
			}
			if (call_back)
			{
				call_back(ret);
			}
		}
	}
	void cxNetworkManager::getGcodeSliceTypeListSuffix(std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetGcodeSliceTypeListSuffix;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ "{}" };
		auto future_text = cpr::PostCallback([=](cpr::Response r) {
			if (call_back)
			{
				call_back(r.text);
			}
			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::setDeviceDesiredProperty_NewIOTControlPrintGcode(const std::string& urlSuffix, const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cSetDeviceDesiredProperty_NewIOTControlPrintGcode + urlSuffix;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		auto future_text = cpr::PostCallback([](cpr::Response r) {
			return r.text;
			}, cpr::Url{ strUrl }, payload,
				header);
		updateActiveTime(0);

		if (future_text.wait_for(std::chrono::seconds(10)) == std::future_status::ready)
		{
			auto ret = future_text.get();
			if (ret.empty())
			{
				return;
			}
			if (call_back)
			{
				call_back(ret);
			}
		}
	}
	void cxNetworkManager::startPrintGcode(const std::string& strContent, std::function<void(int, std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cStartPrintGcodeSuffix;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		auto future_text = cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			if (doc.HasParseError())
			{
				return;
			}
			int state = -1;
			std::string id;
			if (doc.HasMember("result") && doc["result"].IsObject())
			{
				const auto& resultObj = doc["result"];

				if (resultObj.HasMember("printInfo") && resultObj["printInfo"].IsObject())
				{
					const auto& printObj = resultObj["printInfo"];
					if (printObj.HasMember("id") && printObj["id"].IsString())
					{
						id = printObj["id"].GetString();
					}
				}
			}
			if (doc.HasMember("code") && doc["code"].IsInt())
			{
				state = doc["code"].GetInt();
			}
			if (call_back)
			{
				call_back(state, id);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
		updateActiveTime(0);
	}

	void cxNetworkManager::getPrinterTypeList(std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetGcodeSliceTypeListSuffix;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ "{}" };
		auto future_text = cpr::PostCallback([=](cpr::Response r) {
			if (call_back)
			{
				call_back(r.text);
			}
			}, cpr::Url{ strUrl }, payload,
				header);
	}
	void cxNetworkManager::getOnwerDeviceList(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetOnwerDeviceList;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		auto future_text = cpr::PostCallback([=](cpr::Response r) {
			if (call_back)
			{
				call_back(r.text);
			}
			}, cpr::Url{ strUrl }, payload,
				header);
	}
	void cxNetworkManager::queryDevicePropertyStatus_NewIOT(const std::string& tbid, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cQueryDevicePropertyStatus_NewIOT + tbid + "/values/attributes";
		cpr::Header header = getHeader();

		cpr::GetCallback([=](cpr::Response r) {
			if (r.text.empty())
			{
				return;
			}
			if (call_back)
			{
				call_back(r.text);
			}
			}, cpr::Url{ strUrl }, cpr::Parameters{ {"keys", "netIP,tfCard,state,active,model,connect,printId,boxVersion,upgradeStatus,upgrade,bedTemp2,nozzleTemp2,fan,layer,setPosition,video"} },
				header);
	}

	void cxNetworkManager::queryDevicePropertyStatus_NewIOT_RealTime(const std::string& tbid, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cQueryDevicePropertyStatus_NewIOT_RealTime + tbid + "/values/timeseries";
		cpr::Header header = getHeader();

		cpr::GetCallback([=](cpr::Response r) {
			if (r.text.empty())
			{
				return;
			}
			if (call_back)
			{
				call_back(r.text);
			}
			}, cpr::Url{ strUrl }, cpr::Parameters{ {"keys", "nozzleTemp,bedTemp,curFeedratePct,dProgress,printProgress,printJobTime,printLeftTime"},{"useStrictDataTypes", "true"} },
				header);
	}

	void cxNetworkManager::queryDevicePropertyStatus_NewIOT_Position(const std::string& strContent, const std::string& tbid, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cQueryDevicePropertyStatus_NewIOT_Position + tbid;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		auto future_text = cpr::PostCallback([=](cpr::Response r) {
			if (r.text.empty())
			{
				return;
			}
			if (call_back)
			{
				call_back(r.text);
			}
			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::queryDevicePropertyStatus(const std::string& productKey, const std::string& deviceName, const std::string& iotId, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetAliyunInfo;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ "{}" };
		auto futureAliyunInfo = cpr::PostCallback([](cpr::Response r) {
			return r.text;
			}, cpr::Url{ strUrl }, payload,
				header);

		if (futureAliyunInfo.wait_for(std::chrono::seconds(10)) == std::future_status::ready)
		{
			const auto& ret = futureAliyunInfo.get();
			rapidjson::Document doc;
			doc.Parse(ret.c_str());
			if (doc.HasParseError())
				return;

			int state = -1;
			std::string accessKeyId;
			std::string secretAccessKey;
			std::string sessionToken;
			if (doc.HasMember("code") && doc["code"].IsInt())
			{
				state = doc["code"].GetInt();
			}
			if (state != 0)
			{
				return;
			}
			if (doc.HasMember("result") && doc["result"].IsObject())
			{
				const auto& resultObj = doc["result"];
				if (resultObj.HasMember("aliyunInfo") && resultObj["aliyunInfo"].IsObject())
				{
					const auto& aliyunInfo = resultObj["aliyunInfo"];
					if (aliyunInfo.HasMember("accessKeyId") && aliyunInfo["accessKeyId"].IsString())
					{
						accessKeyId = aliyunInfo["accessKeyId"].GetString();
					}
					if (aliyunInfo.HasMember("secretAccessKey") && aliyunInfo["secretAccessKey"].IsString())
					{
						secretAccessKey = aliyunInfo["secretAccessKey"].GetString();
					}
					if (aliyunInfo.HasMember("sessionToken") && aliyunInfo["sessionToken"].IsString())
					{
						sessionToken = aliyunInfo["sessionToken"].GetString();
					}
				}
			}

			qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

			cpr::Parameters parameters = cpr::Parameters{ };
			parameters.Add(cpr::Parameter{ "Action", "QueryDevicePropertyStatus" });
			parameters.Add(cpr::Parameter{ "Format", "JSON" });
			parameters.Add(cpr::Parameter{ "Version", "2018-01-20" });
			parameters.Add(cpr::Parameter{ "SignatureMethod", "HMAC-SHA1" });
			parameters.Add(cpr::Parameter{ "SignatureNonce", QString::number(qrand()).toStdString() });
			parameters.Add(cpr::Parameter{ "AccessKeyId", accessKeyId });
			parameters.Add(cpr::Parameter{ "SecurityToken", sessionToken });
			parameters.Add(cpr::Parameter{ "Timestamp", QDateTime::currentDateTime().toUTC().toString("yyyy-MM-ddThh:mm:ssZ").toStdString() });
			parameters.Add(cpr::Parameter{ "RegionId", "cn-shanghai" });
			parameters.Add(cpr::Parameter{ "ProductKey", productKey });
			parameters.Add(cpr::Parameter{ "DeviceName", deviceName });
			parameters.Add(cpr::Parameter{ "IotId", iotId });
			parameters.Add(cpr::Parameter{ "Signature", iotId });
			parameters.Add(cpr::Parameter{ "SecurityToken", iotId });
			parameters.Add(cpr::Parameter{ "Timestamp", iotId });

			QString strurl = "https://iot.cn-shanghai.aliyuncs.com/";

			auto r = cpr::GetCallback([=](cpr::Response r) {
				if (call_back)
				{
					call_back(r.text);
				}
				}, cpr::Url{ strUrl }, parameters);
		}
	}

	void cxNetworkManager::getDeviceStatus(const std::string& productKey, const std::string& deviceName, const std::string& iotId, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetAliyunInfo;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ "{}" };
		auto futureAliyunInfo = cpr::PostCallback([](cpr::Response r) {
			return r.text;
			}, cpr::Url{ strUrl }, payload,
				header);

		if (futureAliyunInfo.wait_for(std::chrono::seconds(10)) == std::future_status::ready)
		{
			const auto& ret = futureAliyunInfo.get();
			rapidjson::Document doc;
			doc.Parse(ret.c_str());
			if (doc.HasParseError())
				return;

			int state = -1;
			std::string accessKeyId;
			std::string secretAccessKey;
			std::string sessionToken;
			if (doc.HasMember("code") && doc["code"].IsInt())
			{
				state = doc["code"].GetInt();
			}
			if (state != 0)
			{
				return;
			}
			if (doc.HasMember("result") && doc["result"].IsObject())
			{
				const auto& resultObj = doc["result"];
				if (resultObj.HasMember("aliyunInfo") && resultObj["aliyunInfo"].IsObject())
				{
					const auto& aliyunInfo = resultObj["aliyunInfo"];
					if (aliyunInfo.HasMember("accessKeyId") && aliyunInfo["accessKeyId"].IsString())
					{
						accessKeyId = aliyunInfo["accessKeyId"].GetString();
					}
					if (aliyunInfo.HasMember("secretAccessKey") && aliyunInfo["secretAccessKey"].IsString())
					{
						secretAccessKey = aliyunInfo["secretAccessKey"].GetString();
					}
					if (aliyunInfo.HasMember("sessionToken") && aliyunInfo["sessionToken"].IsString())
					{
						sessionToken = aliyunInfo["sessionToken"].GetString();
					}
				}
			}

			qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
			cpr::Parameters parameters = cpr::Parameters{ };
			parameters.Add(cpr::Parameter{ "Action", "GetDeviceStatus" });
			parameters.Add(cpr::Parameter{ "Format", "JSON" });
			parameters.Add(cpr::Parameter{ "Version", "2018-01-20" });
			parameters.Add(cpr::Parameter{ "SignatureMethod", "HMAC-SHA1" });
			parameters.Add(cpr::Parameter{ "SignatureNonce", QString::number(qrand()).toStdString() });
			parameters.Add(cpr::Parameter{ "AccessKeyId", accessKeyId });
			parameters.Add(cpr::Parameter{ "SecurityToken", sessionToken });
			parameters.Add(cpr::Parameter{ "Timestamp", QDateTime::currentDateTime().toUTC().toString("yyyy-MM-ddThh:mm:ssZ").toStdString() });
			parameters.Add(cpr::Parameter{ "RegionId", "cn-shanghai" });
			parameters.Add(cpr::Parameter{ "ProductKey", productKey });
			parameters.Add(cpr::Parameter{ "DeviceName", deviceName });
			parameters.Add(cpr::Parameter{ "IotId", iotId });
			parameters.Add(cpr::Parameter{ "Signature", iotId });
			parameters.Add(cpr::Parameter{ "SecurityToken", iotId });
			parameters.Add(cpr::Parameter{ "Timestamp", iotId });

			QString strurl = "https://iot.cn-shanghai.aliyuncs.com/";

			auto r = cpr::GetCallback([=](cpr::Response r) {
				if (call_back)
				{
					call_back(r.text);
				}
				}, cpr::Url{ strUrl }, parameters);
		}
	}

	void cxNetworkManager::setDeviceDesiredProperty_NewIOT(const std::string& strContent, const std::string& tbid, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cSetDeviceDesiredProperty_NewIOTControlPrintGcode + tbid;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		auto future_text = cpr::PostCallback([=](cpr::Response r) {
			if (call_back)
			{
				call_back(r.text);
			}
			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::setDeviceDesiredProperty_NewIOTControlPrintTempSpeed(const std::string& strContent, const std::string& tbid, std::function<void(std::string)> call_back)
	{
		setDeviceDesiredProperty_NewIOT(strContent, tbid, call_back);
	}

	void cxNetworkManager::getQrLoginInfo(std::function<void(std::string, time_t)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetQrLoginInfo;

		cpr::Header header = cpr::Header{};
		header.insert({ "Content-Type", "application/json" });

		cpr::Body payload = cpr::Body{ "{}" };
		auto future_text = cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			if (doc.HasParseError())
				return std::make_pair(std::string(), 0LL);

			if (doc.HasMember("result") && doc["result"].IsObject())
			{
				const auto& resultObj = doc["result"];

				if (resultObj.HasMember("identical") && resultObj["identical"].IsString())
				{
					if (resultObj.HasMember("expireTime") && resultObj["expireTime"].IsInt64())
					{
						if (call_back)
						{
							call_back(std::string(resultObj["identical"].GetString()), resultObj["expireTime"].GetInt64());
						}
						return std::make_pair(std::string(resultObj["identical"].GetString()), (long long)resultObj["expireTime"].GetInt64());
					}
				}
			}
			return std::make_pair(std::string(), 0LL);
			}, cpr::Url{ strUrl }, payload,
				header);

		//if (future_text.wait_for(std::chrono::seconds(10)) == std::future_status::ready)
		//{
		//	auto ret = future_text.get();
		//	if (call_back)
		//	{
		//		call_back(ret.first, ret.second);
		//	}
		//}
	}

	void cxNetworkManager::startQueryQrLoginInfo(const std::string& strContent, std::function<void(int, std::string, std::string)> call_back)
	{
		if (!m_isQueryingQr)
		{
			setQueryQrData(strContent);
			auto t = std::thread(&cxNetworkManager::queryQrLoginInfoThread, this, call_back);
			t.detach();
			m_isQueryingQr = true;
		}
		else
		{
			setQueryQrData(strContent);
		}
	}

	void cxNetworkManager::queryQrLoginInfoThread(std::function<void(int, std::string, std::string)> call_back)
	{
		while (!m_bExit)
		{
			std::unordered_map<std::string, std::string> retKvs;
			std::string strUrl = getCloudUrl() + cQueryQrLoginInfo;
			std::string content = getQueryQrData();
			if (content.empty())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				continue;
			}

			cpr::Header header = getHeader();

			cpr::Body payload = cpr::Body{ content };
			auto future_text = cpr::PostCallback([](cpr::Response r) {
				rapidjson::Document doc;
				doc.Parse(r.text.c_str());
				if (doc.HasParseError())
					return std::tuple<int, std::string, std::string>();

				int state = -1;
				std::string userId;
				std::string token;
				if (doc.HasMember("result") && doc["result"].IsObject())
				{
					const auto& resultObj = doc["result"];
					if (resultObj.HasMember("state") && resultObj["state"].IsInt())
					{
						state = resultObj["state"].GetInt();
						if (state == 3)
						{
							if (resultObj.HasMember("userId") && resultObj["userId"].IsInt64())
							{
								userId = std::to_string(resultObj["userId"].GetInt64());
							}
							if (resultObj.HasMember("token") && resultObj["token"].IsString())
							{
								token = resultObj["token"].GetString();
							}
						}
					}
				}
				return std::make_tuple(state, userId, token);
				}, cpr::Url{ strUrl }, payload,
					header);

			if (future_text.wait_for(std::chrono::seconds(10)) == std::future_status::ready)
			{
				auto ret = future_text.get();
				auto state = std::get<0>(ret);
				auto userId = std::get<1>(ret);
				auto token = std::get<2>(ret);
				if (state == 3)
				{
					setloginInfo(1, token.c_str(), userId.c_str());
				}
				if (call_back)
				{
					call_back(state, token, userId);
				}
				if (state == 3)
				{
					break;
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		m_isQueryingQr = false;
	}

	void cxNetworkManager::importMyModel(const std::string& strContent, std::function<void(int)> call_back, std::string downloadFilePath)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetModelFileList;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			int state = -1;
			std::string url;
			rapidjson::Document doc;
			QString str = r.text.c_str();
			doc.Parse(r.text.c_str());
			if (doc.HasParseError()) 
			{
				if (call_back)
				{
					call_back(state);
				}
				return;
			}

			if (doc.HasMember("code") && doc["code"].IsInt())
			{
				state = doc["code"].GetInt();
			}
			if (state != 0)
			{
				if (call_back)
				{
					call_back(state);
				}
				return;
			}
			if (doc.HasMember("result") && doc["result"].IsObject())
			{
				const auto& resultObj = doc["result"];
				if (resultObj.HasMember("signUrl") && resultObj["signUrl"].IsString())
				{
					url = resultObj["signUrl"].GetString();
				}
			}
			if (!url.empty())
			{
				std::ofstream of(downloadFilePath, std::ios::binary | std::ios::trunc);
				cpr::Response r = cpr::Download(of, cpr::Url{ url });
				if (r.error)
				{
					state = -1;
				}
			}
			else
			{
				state = -1;
			}
			if (call_back)
			{
				call_back(state);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::importDetailItem(const std::string& strContent, std::function<void(int)> call_back, std::string downloadFilePath)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cImportDetailItem;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			int state = -1;
			std::string url;
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			if (doc.HasParseError())
			{
				if (call_back)
				{
					call_back(state);
				}
				return;
			}

			if (doc.HasMember("code") && doc["code"].IsInt())
			{
				state = doc["code"].GetInt();
			}
			if (state != 0)
			{
				if (call_back)
				{
					call_back(state);
				}
				return;
			}
			if (doc.HasMember("result") && doc["result"].IsObject())
			{
				const auto& resultObj = doc["result"];
				if (resultObj.HasMember("signUrl") && resultObj["signUrl"].IsString())
				{
					url = resultObj["signUrl"].GetString();
				}
			}
			if (!url.empty())
			{
				cpr::Response r = cpr::Download(downloadFilePath, cpr::Url{ url }, cpr::VerifySsl(false));
				if (r.error)
				{
					state = -1;
				}
			}
			else
			{
				state = -1;
			}
			if (call_back)
			{
				call_back(state);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::getModelGroupDetail(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetModelGroupDetail;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	static int sucessCount = 0;
	static int dealedCount = 0;
	void cxNetworkManager::downloadAllModel(const std::string& strContent, std::function<void(int)> call_back, const std::string& filepath)
	{
		rapidjson::Document doc;
		doc.Parse(strContent.c_str());
		auto ret = strContent;
		int state = -1;
		if (doc.HasParseError())
		{
			ret = "";
		}
		if (doc.HasMember("code") && doc["code"].IsInt())
		{
			state = doc["code"].GetInt();
		}
		if (state != 0)
		{
			return;
		}
		int totalCount = 0;
		sucessCount = 0;
		dealedCount = 0;

		if (doc.HasMember("result") && doc["result"].IsObject())
		{
			const auto& resultObj = doc["result"];
			if (resultObj.HasMember("list") && resultObj["list"].IsArray())
			{
				const auto& array = resultObj["list"];
				totalCount = array.Size();
				for (size_t i = 0; i < array.Size(); i++)
				{
					std::string fileFormat;
					std::string fileName;
					std::string id;
					const auto& modelInfo = array[i];
					if (modelInfo.HasMember("fileFormat") && modelInfo["fileFormat"].IsString())
					{
						fileFormat = modelInfo["fileFormat"].GetString();
					}
					if (modelInfo.HasMember("fileName") && modelInfo["fileName"].IsString())
					{
						fileName = modelInfo["fileName"].GetString();
					}
					if (modelInfo.HasMember("id") && modelInfo["id"].IsString())
					{
						id = modelInfo["id"].GetString();
					}
					std::string requestContent = "{\"id\":\"";
					requestContent = requestContent + id + "\"}";
					importDetailItem(requestContent, [=](int state) {
						if (state == 0)
						{
							sucessCount++;
						}
						dealedCount++;
						if (dealedCount == totalCount)
						{
							call_back(sucessCount < totalCount);
						}
						}, filepath + fileName + fileFormat);
				}
			}
		}
	}

	void cxNetworkManager::getRecommendModel(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetRecommendModel;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::getCategoryList(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetCategoryList;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::getModelLibraryList(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetModelLibraryList;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::searchModelList(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cSearchModelList;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::getModelGroupInfomation(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetModelGroupInfomation;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::getSSOToken(std::function<void(std::string)> call_back)
	{
		std::string strUrl = getCloudUrl() + cGetSSOToken;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ "{}"};
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (doc.HasMember("result") && doc["result"].IsObject())
			{
				const auto& resultObj = doc["result"];
				if (resultObj.HasMember("ssoToken") && resultObj["ssoToken"].IsString())
				{
					ret = resultObj["ssoToken"].GetString();
				}
			}

			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::getInfo(std::function<void(std::string)> call_back)
	{
		std::string strUrl = getCloudUrl() + cGetInfo;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ "{}" };
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			QString test = r.text.c_str();
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}
	void cxNetworkManager::quickLoginByAccont(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cQuickLoginByAccont;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		auto future_text = cpr::PostCallback([=](cpr::Response r) {
			if (call_back)
			{
				call_back(r.text);
			}
			}, cpr::Url{ strUrl }, payload, /*cpr::VerifySsl(false),*/
				header);
	}

	void cxNetworkManager::getVerificationCode(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetVerificationCode;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		auto future_text = cpr::PostCallback([=](cpr::Response r) {
			if (call_back)
			{
				call_back(r.text);
			}
			}, cpr::Url{ strUrl }, payload, /*cpr::VerifySsl(false),*/
				header);
	}

	void cxNetworkManager::loginSuccess(std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cLoginSuccess;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ "{}" };
		auto future_text = cpr::PostCallback([=](cpr::Response r) {
			if (call_back)
			{
				call_back(r.text);
			}
			}, cpr::Url{ strUrl }, payload, /*cpr::VerifySsl(false),*/
				header);
	}

	void cxNetworkManager::loginByAccont(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cLoginByAccont;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		auto future_text = cpr::PostCallback([=](cpr::Response r) {
			if (call_back)
			{
				call_back(r.text);
			}
			}, cpr::Url{ strUrl }, payload, /*cpr::VerifySsl(false),*/
				header);
	}

	void cxNetworkManager::uploadGcode(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cUploadGcode;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::modelGroupCreate(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cModelGroupCreate;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::getAliyunOssKeyAndBucket(std::function<void(bool, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetAliyunInfo;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ "{}" };
		auto futureAliyunInfo = cpr::PostCallback([](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			if (doc.HasParseError())
				return std::tuple<int, std::string, std::string, std::string>();

			int state = -1;
			std::string accessKeyId;
			std::string secretAccessKey;
			std::string sessionToken;
			if (doc.HasMember("code") && doc["code"].IsInt())
			{
				state = doc["code"].GetInt();
			}
			if (state != 0)
			{
				return std::make_tuple(state, accessKeyId, secretAccessKey, sessionToken);
			}
			if (doc.HasMember("result") && doc["result"].IsObject())
			{
				const auto& resultObj = doc["result"];
				if (resultObj.HasMember("aliyunInfo") && resultObj["aliyunInfo"].IsObject())
				{
					const auto& aliyunInfo = resultObj["aliyunInfo"];
					if (aliyunInfo.HasMember("accessKeyId") && aliyunInfo["accessKeyId"].IsString())
					{
						accessKeyId = aliyunInfo["accessKeyId"].GetString();
					}
					if (aliyunInfo.HasMember("secretAccessKey") && aliyunInfo["secretAccessKey"].IsString())
					{
						secretAccessKey = aliyunInfo["secretAccessKey"].GetString();
					}
					if (aliyunInfo.HasMember("sessionToken") && aliyunInfo["sessionToken"].IsString())
					{
						sessionToken = aliyunInfo["sessionToken"].GetString();
					}
				}
			}
			return std::make_tuple(state, accessKeyId, secretAccessKey, sessionToken);

			}, cpr::Url{ strUrl }, payload,
				header);

		strUrl = getCloudUrl() + cGetAliyunOssBucket;
		auto futureAliyunBucket = cpr::PostCallback([](cpr::Response r) {
			int state = -1;
			std::string endpoint;
			std::string fileBucket;
			std::string prefixPath;
			std::string internalBucket;
			std::string internalPrefixPath;

			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			if (doc.HasParseError())
				return std::make_tuple(state, endpoint, fileBucket, prefixPath, internalBucket, internalPrefixPath);

			if (doc.HasMember("code") && doc["code"].IsInt())
			{
				state = doc["code"].GetInt();
			}
			if (state != 0)
			{
				return std::make_tuple(state, endpoint, fileBucket, prefixPath, internalBucket, internalPrefixPath);
			}
			if (doc.HasMember("result") && doc["result"].IsObject())
			{
				const auto& resultObj = doc["result"];
				if (resultObj.HasMember("info") && resultObj["info"].IsObject())
				{
					const auto& aliyunInfo = resultObj["info"];
					if (aliyunInfo.HasMember("file") && aliyunInfo["file"].IsObject())
					{
						const auto& fileObject = aliyunInfo["file"];
						if (fileObject.HasMember("bucket") && fileObject["bucket"].IsString())
						{
							fileBucket = fileObject["bucket"].GetString();
						}
						if (fileObject.HasMember("prefixPath") && fileObject["prefixPath"].IsString())
						{
							prefixPath = fileObject["prefixPath"].GetString();
						}
					}
					if (aliyunInfo.HasMember("internal") && aliyunInfo["internal"].IsObject())
					{
						const auto& internalObject = aliyunInfo["internal"];
						if (internalObject.HasMember("bucket") && internalObject["bucket"].IsString())
						{
							internalBucket = internalObject["bucket"].GetString();
						}
						if (internalObject.HasMember("prefixPath") && internalObject["prefixPath"].IsString())
						{
							internalPrefixPath = internalObject["prefixPath"].GetString();
						}
					}
					if (aliyunInfo.HasMember("endpoint") && aliyunInfo["endpoint"].IsString())
					{
						endpoint = aliyunInfo["endpoint"].GetString();
					}
				}
			}
			return std::make_tuple(state, endpoint, fileBucket, prefixPath, internalBucket, internalPrefixPath);

			}, cpr::Url{ strUrl }, payload,
				header);

		if (futureAliyunInfo.wait_for(std::chrono::seconds(10)) == std::future_status::ready &&
			futureAliyunBucket.wait_for(std::chrono::seconds(10)) == std::future_status::ready)
		{
			if (call_back)
			{
				const auto& aliyunInfo = futureAliyunInfo.get();
				const auto& aliyunBucket = futureAliyunBucket.get();
				call_back(std::get<0>(aliyunInfo) && std::get<0>(aliyunBucket), std::get<1>(aliyunInfo), std::get<2>(aliyunInfo), std::get<3>(aliyunInfo),
					std::get<1>(aliyunBucket), std::get<2>(aliyunBucket), std::get<3>(aliyunBucket), std::get<4>(aliyunBucket), std::get<5>(aliyunBucket));
			}
		}

	}

	void cxNetworkManager::addOrDeleteFavoriteModel(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::string strUrl = getCloudUrl() + cAddOrDeleteFavoriteModel;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::getGCodeList(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::string strUrl = getCloudUrl() + cGetGCodeList;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::deleteGcode(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::string strUrl = getCloudUrl() + cDeleteGcode;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::getModelList(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::string strUrl = getCloudUrl() + cGetModelList;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::deleteModel(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::string strUrl = getCloudUrl() + cDeleteModel;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::getFavoritedList(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::string strUrl = getCloudUrl() + cGetFavoritedList;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			rapidjson::Document doc;
			doc.Parse(r.text.c_str());
			auto ret = r.text;
			if (doc.HasParseError())
			{
				ret = "";
			}
			if (call_back)
			{
				call_back(ret);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::getProfileList(const std::string& printerName, const std::string& downloadPath, std::function<void(std::list<std::string>)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetProfileList;

		cpr::Header header = getHeader();
		
		std::string strContent;
		strContent += "{";
		strContent += "\"printerName\":\"";
		strContent += printerName;
		strContent += "\"}";

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			std::list<std::string> fileList;
			rapidjson::Document doc;
			auto res = r.text;
			QString qres = QString::fromStdString(res);
			doc.Parse(r.text.c_str());
			if (doc.HasParseError())
			{
				if (call_back)
				{
					call_back(fileList);
					return;
				}
			}

			int state = -1;
			if (doc.HasMember("code") && doc["code"].IsInt())
			{
				state = doc["code"].GetInt();
			}
			if (state != 0)
			{
				if (call_back)
				{
					call_back(fileList);
					return;
				}
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
						if (call_back)
						{
							call_back(fileList);
							return;
						}
					}
					if (resultObj.HasMember("list") && resultObj["list"].IsArray())
					{
						const auto& array = resultObj["list"];
						for (size_t i = 0; i < array.Size(); i++)
						{
							std::string fileName;
							std::string url;
							const auto& profile = array[i];
							if (profile.HasMember("fileName") && profile["fileName"].IsString())
							{
								fileName = profile["fileName"].GetString();
							}
							if (profile.HasMember("Url") && profile["Url"].IsString())
							{
								url = profile["Url"].GetString();
							}
							std::ofstream of(downloadPath + fileName, std::ios::binary | std::ios::trunc);
							cpr::Response r = cpr::Download(of, cpr::Url{ url });
							if (!r.error)
							{
								fileList.push_back(fileName);
							}
						}
					}
					if (call_back)
					{
						call_back(fileList);
						return;
					}
				}
			}

			}, cpr::Url{ strUrl }, payload, cpr::VerifySsl(false),
				header);
	}

	void cxNetworkManager::getPrinterTypeListNewResult(const std::string& strContent, std::function<void(std::string)> call_back)
	{
		std::unordered_map<std::string, std::string> retKvs;
		std::string strUrl = getCloudUrl() + cGetPrinterTypeListNewResult;

		cpr::Header header = getHeader();

		cpr::Body payload = cpr::Body{ strContent };
		cpr::PostCallback([=](cpr::Response r) {
			if (call_back)
			{
				call_back(r.text);
			}

			}, cpr::Url{ strUrl }, payload,
				header);
	}

	void cxNetworkManager::setQueryQrData(const std::string& strContent)
	{
		std::lock_guard<std::mutex> lock(m_mtxQueryQr);
		m_queryQrData = strContent;
	}
	std::string cxNetworkManager::getQueryQrData()
	{
		std::lock_guard<std::mutex> lock(m_mtxQueryQr);
		return m_queryQrData;
	}
}



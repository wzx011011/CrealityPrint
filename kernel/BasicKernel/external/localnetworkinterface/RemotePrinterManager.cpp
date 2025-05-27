#include "RemotePrinterManager.h"
#include "RemotePrinter.h"
#include <thread>
#include "rapidjson/document.h"

namespace creative_kernel
{
	SINGLETON_IMPL(RemotePrinterManager)
	RemotePrinterManager::RemotePrinterManager()
	{
		m_pLanPrinterInterface = new LanPrinterInterface();
		m_pOctoPrinterInterface = new OctoPrintInterface();
		auto t = std::thread(&RemotePrinterManager::refreshStateThread, this);
		t.detach();
	}

	void RemotePrinterManager::Init()
	{
	}

	RemotePrinterManager::~RemotePrinterManager()
	{
		m_bExit = true;
		m_mutex.lock();
		m_mapID2Printer.clear();
		m_mutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		delete m_pLanPrinterInterface;
		delete m_pOctoPrinterInterface;
	}

	void RemotePrinterManager::searchDevices()
	{
		//Ŀǰֻ֧�־�������������,����Ŀǰ��ȫ�����أ�����ĳ��ѵ�һ������һ��
		auto t = std::thread(&RemotePrinterManager::searchLanDeviceThread, this);
		t.detach();
	}

	void RemotePrinterManager::searchLanDeviceThread()
	{
		auto devices = cxnet::syncDiscoveryService("CXSWBox");

		for (auto item : devices)
		{
			if (m_pfnSearchCb != nullptr)
			{
				m_pfnSearchCb(item.machineMac, item.machineIp);
			}
			RemotePrinter printer;
			printer.ipAddress = item.machineIp.c_str();
			printer.macAddress = item.machineMac.c_str();
			printer.type = RemotePrinerType::REMOTE_PRINTER_TYPE_LAN;
			printer.uniqueId = printer.ipAddress.isEmpty() ? printer.macAddress : printer.ipAddress;
			addPrinter(printer);
		}
	}

	void RemotePrinterManager::refreshStateThread()
	{
		while (!m_bExit)
		{
			m_mutex.lock();
			if (m_mapID2Printer.empty())
			{
				m_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				continue;
			}
			for (auto& item : m_mapID2Printer)
			{
				auto& session = item.second;
				time_t tmNow = time(nullptr);
				if (session.tmLastActive == 0 || abs(tmNow - session.tmLastActive) > 3)
				{
					switch (session.type)
					{
					case RemotePrinerType::REMOTE_PRINTER_TYPE_LAN:
					{
						m_pLanPrinterInterface->getDeviceState(session.ipAddress, [&](std::unordered_map<std::string, std::string> ret) {
							if (ret.size() > 0 && m_pfnGetInfoCb)
							{
								RemotePrinter printer;
								printer.uniqueId = session.uniqueId.c_str();
								printer.type = RemotePrinerType::REMOTE_PRINTER_TYPE_LAN;
								printer.ipAddress = session.ipAddress.c_str();

								QString ssid = QString(ret["ssid"].c_str());
								QStringList strList = ssid.split("-");
								if (strList.size() > 1)
								{
									printer.macAddress = strList[1];
									printer.deviceName = strList[0];
									if (session.macAddress.empty())
									{
										session.macAddress = printer.macAddress.toStdString();
									}
								}
								else
								{
									printer.macAddress = "";
									printer.deviceName = "";
								}
								printer.productKey = "";
								printer.printerName = QString(ret["model"].c_str());
								QString filepath = QString(ret["print"].c_str());
								QStringList list = filepath.split("/");
								bool mcu_is_print = ret["mcu_is_print"] == "1";
								if(mcu_is_print)
								{
									printer.printFileName = filepath;
									printer.printMode = 0;
								}
								else if(list.size() > 0 && filepath != "localhost")
								{
									printer.printFileName = list[list.size() - 1];
									if (!printer.printFileName.endsWith(".gcode"))
									{
										auto index = printer.printFileName.lastIndexOf(".gcode");
										printer.printFileName = printer.printFileName.left(index + std::string(".gcode").length());
										printer.printMode = 2;
									}
									else
									{
										printer.printMode = 1;
									}
								}
								printer.curPosition = QString(ret["curPosition"].c_str());
								printer.printerStatus = atoi(ret["connect"].c_str());
								printer.TFCardStatus = atoi(ret["tfCard"].c_str());
								printer.printState = atoi(ret["state"].c_str());
								printer.fanSwitchState = atoi(ret["fan"].c_str());
								printer.printProgress = atoi(ret["printProgress"].c_str());
								printer.nozzleTemp = atof(ret["nozzleTemp"].c_str());
								printer.nozzleTempTarget = atof(ret["nozzleTemp2"].c_str());
								printer.bedTemp = atof(ret["bedTemp"].c_str());
								printer.begTempTarget = atof(ret["bedTemp2"].c_str());
								printer.printSpeed = atoi(ret["curFeedratePct"].c_str());;
								printer.printJobTime = atoi(ret["printJobTime"].c_str());
								printer.printLeftTime = atoi(ret["printLeftTime"].c_str());
								printer.autoHomeState = atoi(ret["autohome"].c_str());
								printer.errorType = atoi(ret["err"].c_str());
								printer.video = atoi(ret["video"].c_str());
								if ((!mcu_is_print) && (!printer.printFileName.isEmpty()) && (session.previewImg != printer.printFileName.toStdString()))
								{
									session.previewImg = printer.printFileName.toStdString();
									getPreviewImg(printer, printer.printFileName.toStdString());
								}
								printer.ledState = atoi(ret["led_state"].c_str());
								printer.totalLayer = atoi(ret["TotalLayer"].c_str());
								printer.layer = atoi(ret["layer"].c_str());
								m_pfnGetInfoCb(printer);
							}

							session.connected = true;
							session.tmLastActive = tmNow;
							}
						);
					}
						break;
					case RemotePrinerType::REMOTE_PRINTER_TYPE_OCTOPRINT:
					{
						m_pOctoPrinterInterface->getDeviceState(session.ipAddress, session.token, [&](const std::string& ret) {
							if (ret.size() > 0 && m_pfnGetInfoCb)
							{
								RemotePrinter printer;
								printer.ipAddress = session.ipAddress.c_str();
								printer.type = RemotePrinerType::REMOTE_PRINTER_TYPE_OCTOPRINT;

								rapidjson::Document doc;
								doc.Parse(ret.c_str());
								if (doc.HasParseError())
									return;
								if (doc.HasMember("state") && doc["state"].IsObject())
								{
									const rapidjson::Value& objectState = doc["state"];
									if (objectState.HasMember("text") && objectState["text"].IsString())
									{
										objectState["name"].GetString();
									}
									if (objectState.HasMember("flags") && objectState["flags"].IsObject())
									{
										const rapidjson::Value& objectFlags = doc["flags"];
										if (objectFlags.HasMember("operational") && objectFlags["operational"].IsBool())
										{
											objectFlags["operational"].GetBool();
										}
									}
								}
								if (doc.HasMember("sd") && doc["sd"].IsObject())
								{
									const rapidjson::Value& objectSD = doc["sd"];
									if (objectSD.HasMember("ready") && objectSD["ready"].IsBool())
									{
										printer.TFCardStatus = objectSD["ready"].GetBool();
									}
								}
								if (doc.HasMember("temperature") && doc["temperature"].IsObject())
								{
									const rapidjson::Value& objectTemperature = doc["temperature"];
									if (objectTemperature.HasMember("tool0") && objectTemperature["tool0"].IsObject())
									{
										const rapidjson::Value& objectTool0 = doc["tool0"];
										if (objectTool0.HasMember("actual") && objectTool0["actual"].IsDouble())
										{
											objectTool0["actual"].GetDouble();
										}
										if (objectTool0.HasMember("target") && objectTool0["target"].IsDouble())
										{
											objectTool0["target"].GetDouble();
										}
										if (objectTool0.HasMember("offset") && objectTool0["offset"].IsDouble())
										{
											objectTool0["offset"].GetDouble();
										}
									}

									if (objectTemperature.HasMember("tool0") && objectTemperature["tool0"].IsObject())
									{
										const rapidjson::Value& objectTool0 = doc["tool0"];
										if (objectTool0.HasMember("actual") && objectTool0["actual"].IsDouble())
										{
											objectTool0["actual"].GetDouble();
										}
										if (objectTool0.HasMember("target") && objectTool0["target"].IsDouble())
										{
											objectTool0["target"].GetDouble();
										}
										if (objectTool0.HasMember("offset") && objectTool0["offset"].IsDouble())
										{
											objectTool0["offset"].GetDouble();
										}
									}

									if (objectTemperature.HasMember("tool1") && objectTemperature["tool1"].IsObject())
									{
										const rapidjson::Value& objectTool1 = doc["tool1"];
										if (objectTool1.HasMember("actual") && objectTool1["actual"].IsDouble())
										{
											objectTool1["actual"].GetDouble();
										}
										if (objectTool1.HasMember("target") && objectTool1["target"].IsDouble())
										{
											objectTool1["target"].GetDouble();
										}
										if (objectTool1.HasMember("offset") && objectTool1["offset"].IsDouble())
										{
											objectTool1["offset"].GetDouble();
										}
									}

									if (objectTemperature.HasMember("bed") && objectTemperature["bed"].IsObject())
									{
										const rapidjson::Value& objectBed = doc["bed"];
										if (objectBed.HasMember("actual") && objectBed["actual"].IsDouble())
										{
											objectBed["actual"].GetDouble();
										}
										if (objectBed.HasMember("target") && objectBed["target"].IsDouble())
										{
											objectBed["target"].GetDouble();
										}
										if (objectBed.HasMember("offset") && objectBed["offset"].IsDouble())
										{
											objectBed["offset"].GetDouble();
										}
									}
								}

								m_pfnGetInfoCb(printer);
							}

							session.connected = true;
							session.tmLastActive = tmNow;
							});
					}
						break;
					default:
						break;
					}
				}
				if (session.tmLastGetFileList == 0 || abs(tmNow - session.tmLastGetFileList) > 10)
				{
					switch (session.type)
					{
					case RemotePrinerType::REMOTE_PRINTER_TYPE_LAN:
					{
						m_pLanPrinterInterface->getFileListFromDevice(session.ipAddress, [=](std::string filelist) {
							if (m_pfnGetFileListCb)
							{
								m_pfnGetFileListCb(session.macAddress, filelist);
							}
							});
					}
					session.tmLastGetFileList = tmNow;
					break;
					default:
						break;
					}
				}
			}
			m_mutex.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		m_condition.notify_all();
	}
	void RemotePrinterManager::addPrinter(const RemotePrinter& printer)
	{
		auto key = printer.uniqueId.toStdString();
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_mapID2Printer.find(key) == m_mapID2Printer.end())
		{
			RemotePrinterSession session;
			session.ipAddress = printer.ipAddress.toStdString();
			session.macAddress = printer.macAddress.toStdString();
			session.type = printer.type;
			session.uniqueId = session.ipAddress.empty() ? session.macAddress : session.ipAddress;
			m_mapID2Printer[key] = session;
		}
	}
	void RemotePrinterManager::deletePrinter(const RemotePrinter& printer)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		auto key = printer.uniqueId.toStdString();
		if (m_mapID2Printer.find(key) != m_mapID2Printer.end())
		{
			m_mapID2Printer.erase(key);
		}
	}
	void RemotePrinterManager::getPrinterInfo(RemotePrinter& printer)
	{
	}
	void RemotePrinterManager::pushFile(const RemotePrinter& printer, const std::string& filePath, std::function<void(std::string, float)> callback, std::function<void(std::string, std::string)> errorCallback)
	{
		switch (printer.type)
		{
		case RemotePrinerType::REMOTE_PRINTER_TYPE_LAN:
		{
			QByteArray ba = printer.ipAddress.toLocal8Bit();
			m_pLanPrinterInterface->sendFileToDevice(ba.data(), filePath, [=](float progress) {
				if (callback)
				{
					callback(printer.macAddress.toStdString(), progress);
				}
				},
				[=](std::string errMsg) {
					if (errorCallback)
					{
						errorCallback(printer.macAddress.toStdString(), errMsg);
					}
				});
		}
		break;
		case RemotePrinerType::REMOTE_PRINTER_TYPE_OCTOPRINT:
		{
			m_pOctoPrinterInterface->sendFileToDevice(printer.ipAddress.toStdString(), printer.token.toStdString(), filePath);
		}
		break;
		default:
			break;
		}
	}
	void RemotePrinterManager::getPreviewImg(const RemotePrinter& printer, const std::string& filePath)
	{
		switch (printer.type)
		{
		case RemotePrinerType::REMOTE_PRINTER_TYPE_LAN:
		{
			QByteArray ba = printer.ipAddress.toLocal8Bit();
			m_pLanPrinterInterface->fetchHead(ba.data(), filePath, [=](std::string type, std::string imgData) {
				if (m_pfnGetPreviewCb)
				{
					m_pfnGetPreviewCb(printer.macAddress.toStdString(), type, imgData);
				}
				});
		}
		break;
		case RemotePrinerType::REMOTE_PRINTER_TYPE_OCTOPRINT:
		{
			m_pOctoPrinterInterface->sendFileToDevice(printer.ipAddress.toStdString(), printer.token.toStdString(), filePath);
		}
		break;
		default:
			break;
		}
	}
	void RemotePrinterManager::deleteFile(const RemotePrinter& printer, const std::string& filePath)
	{
		switch (printer.type)
		{
		case RemotePrinerType::REMOTE_PRINTER_TYPE_LAN:
		{
			QByteArray ba = printer.ipAddress.toLocal8Bit();
			m_pLanPrinterInterface->deleteFile(ba.data(), filePath);
		}
		break;
		case RemotePrinerType::REMOTE_PRINTER_TYPE_OCTOPRINT:
		{
		}
		break;
		default:
			break;
		}
	}

	void RemotePrinterManager::controlPrinter(const RemotePrinter& printer, const PrintControlType& cmdType, const std::string& value)
	{
			switch (printer.type)
			{
			case RemotePrinerType::REMOTE_PRINTER_TYPE_LAN:
			{
				auto ret = m_pLanPrinterInterface->controlPrinter(printer.ipAddress.toStdString(), cmdType, value, std::bind(&RemotePrinterManager::updateSessitonActive, this, printer.uniqueId.toStdString(),0));
			}
				break;
			case RemotePrinerType::REMOTE_PRINTER_TYPE_OCTOPRINT:
			{
				m_pOctoPrinterInterface->controlPrinter(printer.ipAddress.toStdString(), printer.token.toStdString(), cmdType, value);
			}
			break;
			default:
				break;
			}
			//updateSessitonActive(printer.uniqueId.toStdString());
	}
	void RemotePrinterManager::transparentCommand(const RemotePrinter& printer, const std::string& value)
	{
			switch (printer.type)
			{
			case RemotePrinerType::REMOTE_PRINTER_TYPE_LAN:
			{
				m_pLanPrinterInterface->transparentCommand(printer.ipAddress.toStdString(), value, std::bind(&RemotePrinterManager::updateSessitonActive, this, printer.uniqueId.toStdString(), 0));
			}
			break;
			case RemotePrinerType::REMOTE_PRINTER_TYPE_OCTOPRINT:
			{
				m_pOctoPrinterInterface->transparentCommand(printer.ipAddress.toStdString(), printer.token.toStdString(), value);
			}
			break;
			default:
				break;
			}
	}
	void RemotePrinterManager::updateSessitonActive(const std::string& uuid, const time_t& tmActive)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_mapID2Printer.find(uuid) != m_mapID2Printer.end())
		{
			m_mapID2Printer[uuid].tmLastActive = tmActive;
		}
	}
}
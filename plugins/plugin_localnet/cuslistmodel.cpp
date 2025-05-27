#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QStringList>
#include <QQmlContext>
#include <QQmlApplicationEngine>

#include "cuslistmodel.h"

using namespace creative_kernel;

/******************** CusListModel ********************/
CusListModel::CusListModel(QObject *parent) : QAbstractListModel(parent), m_previewProvider(new ImgProvider())
{
	//m_ListPrinterItem.clear();
	m_MapPrinterItem.clear();
	m_deviceGroupNames << "Group1";
	m_appDataPath = QProcessEnvironment::systemEnvironment().value("appdata");
	m_infoRegExp = QRegExp("-rwxrwxrwx[\\s]+[\\d][\\s]+[\\d][\\s]+[\\d][\\s]+([\\d]+)[\\s]([a-zA-z]{3}[\\s][\\d]{2}[\\s][\\d]{2}:[\\d]{2})[\\s](.+.gcode)");

	QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();
	engine->addImageProvider(QLatin1String("preview"), m_previewProvider);

	readConfig();
}

CusListModel::~CusListModel()
{
	getRemotePrinterManager()->setGetInfoCb(nullptr);
	getRemotePrinterManager()->setGetFileListCb(nullptr);
	getRemotePrinterManager()->setGetPreviewCb(nullptr);

	delete m_previewProvider;
	this->disconnect();
	writeConfig();
}

void CusListModel::onGetInfoSuccess(RemotePrinter curState)
{
	emit sigGetInfoSuccess(curState);
}

void CusListModel::onGetFileList(const std::string& std_macAddr, const std::string& std_list)
{
	if (std_macAddr.empty()) return;// || std_list.empty()
	QString from_std_macAddr = QString::fromStdString(std_macAddr);
	QString from_std_list = QString::fromStdString(std_list);

	emit sigGetFileList(from_std_macAddr, from_std_list);
}

void CusListModel::onGetPreviewImg(const std::string& std_macAddr, const std::string& std_imgType, const std::string& std_imgData)
{
	if (std_macAddr.empty() || std_imgType.empty() || std_imgData.empty()) return;
	QString from_std_macAddr = QString::fromStdString(std_macAddr);
	//QString from_std_imgType = QString::fromStdString(std_imgType);
	QString from_std_imgData = QString::fromStdString(std_imgData);
	from_std_imgData.remove(QRegExp("[;]|[\\s]"));

	//qDebug() << "from_std_imgData" << from_std_imgData;
	QByteArray from_base64_imgData = QByteArray::fromBase64(from_std_imgData.toUtf8());
	QImage previewImage = QImage::fromData(from_base64_imgData, std_imgType.c_str());
	
	if (!previewImage.isNull()) emit sigGetPreviewImage(from_std_macAddr, previewImage);
}

void CusListModel::refresh()
{
	getRemotePrinterManager()->searchDevices();
}

void CusListModel::findDeviceList()
{
	if (!m_bFirstFind)
	{
		m_bFirstFind = true;
		//qDebug() << "CusWifiManagerModel::findDeviceList()";
		connect(this, &CusListModel::sigGetFileList, this, &CusListModel::slotGetFileList, Qt::ConnectionType::QueuedConnection);
		connect(this, &CusListModel::sigGetInfoSuccess, this, &CusListModel::slotGetInfoSuccess, Qt::ConnectionType::QueuedConnection);
		connect(this, &CusListModel::sigGetPreviewImage, this, &CusListModel::slotGetPreviewImage, Qt::ConnectionType::QueuedConnection);

		getRemotePrinterManager()->setGetInfoCb(std::bind(&CusListModel::onGetInfoSuccess, this, std::placeholders::_1));
		getRemotePrinterManager()->setGetFileListCb(std::bind(&CusListModel::onGetFileList, this, std::placeholders::_1, std::placeholders::_2));
		getRemotePrinterManager()->setGetPreviewCb(std::bind(&CusListModel::onGetPreviewImg, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

		getRemotePrinterManager()->searchDevices();
	}
}

void CusListModel::createConnect(const QString & deviceIpAddr)
{
	//调用连接接口
	m_bIsConnecting = true;
	m_curConnectingIpAddr = deviceIpAddr;
	//qDebug() << "请求添加设备:" << deviceIpAddr;

	RemotePrinter printer;
	printer.uniqueId = deviceIpAddr;
	printer.ipAddress = deviceIpAddr;
	printer.type = RemotePrinerType::REMOTE_PRINTER_TYPE_LAN;
	getRemotePrinterManager()->addPrinter(printer);
}

void CusListModel::sendUIControlCmdToDevice(QString deviceIpAddr, int nCmdType, QString value)
{
	RemotePrinter printer;
	printer.uniqueId = deviceIpAddr;
	printer.ipAddress = deviceIpAddr;
	printer.type = RemotePrinerType::REMOTE_PRINTER_TYPE_LAN;
	getRemotePrinterManager()->controlPrinter(printer, (PrintControlType)nCmdType, value.toStdString());
	//qDebug() << "sendUIControlCmdToDevice" << deviceIpAddr << nCmdType << value;
}

//void CusListModel::batch_reset()
//{
//	foreach(ListModelItem* item, m_MapPrinterItem.values())
//	{
//		item->setGCodeTransProgress(0);
//	}
//	//Update data
//	emit dataChanged(createIndex(0, 0), createIndex(rowCount(), 0));
//}

void CusListModel::batch_pause(const QStringList& deviceMacAddrs)
{
	foreach(QString deviceMacAddr, deviceMacAddrs)
	{
		ListModelItem* item = getDeviceItem(deviceMacAddr);
		if (item) 
		{
			item->setPcHasSelect(false);
			sendUIControlCmdToDevice(item->pcIpAddr(), (int)PrintControlType::PRINT_PAUSE, " ");
		}
	}
}

void CusListModel::batch_warmUp(const QStringList& deviceMacAddrs)
{
	foreach(QString deviceMacAddr, deviceMacAddrs)
	{
		ListModelItem* item = getDeviceItem(deviceMacAddr);
		if (item) 
		{
			item->setPcHasSelect(false);
			sendUIControlCmdToDevice(item->pcIpAddr(), (int)PrintControlType::CONTROL_CMD_BEDTEMP, "60");//default
			sendUIControlCmdToDevice(item->pcIpAddr(), (int)PrintControlType::CONTROL_CMD_NOZZLETEMP, "200");//default
		}
	}
}

void CusListModel::batch_stop(const QStringList& deviceMacAddrs)
{
	foreach(QString deviceMacAddr, deviceMacAddrs)
	{
		ListModelItem* item = getDeviceItem(deviceMacAddr);
		if (item)
		{
			item->setPcHasSelect(false);
			sendUIControlCmdToDevice(item->pcIpAddr(), (int)PrintControlType::PRINT_STOP, " ");
		}
	}
}

void CusListModel::batch_start(const QStringList& deviceMacAddrs, const QString& filePath)
{
	if (filePath.isEmpty()) return;

	QFileInfo fileInfo(filePath);
	m_gcodeUploadPath = "/media/mmcblk0p1/creality/gztemp/" + fileInfo.fileName();//保存打印路径

	foreach(QString deviceMacAddr, deviceMacAddrs)
	{
		ListModelItem* item = getDeviceItem(deviceMacAddr);
		if (item)
		{
			RemotePrinter printer;
			printer.macAddress = deviceMacAddr;
			printer.ipAddress = item->pcIpAddr();
			printer.type = RemotePrinerType::REMOTE_PRINTER_TYPE_LAN;

			item->setPcHasSelect(false);
			getRemotePrinterManager()->pushFile(printer, filePath.toStdString(),
				std::bind(&CusListModel::UploadProgressCallBack, this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&CusListModel::UploadMessageCallBack, this, std::placeholders::_1, std::placeholders::_2));
		}
	}
}

Q_INVOKABLE void CusListModel::importFile(QString macAddress, QString filePath)
{
	filePath = filePath.replace("file:///", "");
	ListModelItem* item = getDeviceItem(macAddress);
	if (filePath.isEmpty() || item == nullptr) return;

	//qDebug() << "importFile" << filePath;

	RemotePrinter printer;
	printer.macAddress = macAddress;
	printer.ipAddress = item->pcIpAddr();
	printer.type = RemotePrinerType::REMOTE_PRINTER_TYPE_LAN;

	getRemotePrinterManager()->pushFile(printer, filePath.toStdString(),
		std::bind(&CusListModel::ImportProgressCallBack, this, std::placeholders::_1, std::placeholders::_2),
		std::bind(&CusListModel::ImportMessageCallBack, this, std::placeholders::_1, std::placeholders::_2));
}

Q_INVOKABLE void CusListModel::deleteFile(QString macAddress, QString filePath)
{
	ListModelItem* item = getDeviceItem(macAddress);
	if (filePath.isEmpty() || item == nullptr) return;

	//qDebug() << "deleteFile" << filePath;

	RemotePrinter printer;
	printer.macAddress = macAddress;
	printer.ipAddress = item->pcIpAddr();
	printer.type = RemotePrinerType::REMOTE_PRINTER_TYPE_LAN;

	getRemotePrinterManager()->deleteFile(printer, filePath.toStdString());
}

QStringList CusListModel::getGroups()
{
    return m_deviceGroupNames;
}

void CusListModel::addGroup(const QString& addGroupName)
{
	m_deviceGroupNames.append(addGroupName);
    qDebug()<<"新建编组"<<addGroupName;
}

void CusListModel::editGroupName(int groupIndex, const QString& newGroupName)
{
	m_deviceGroupNames[groupIndex] = newGroupName;
    qDebug()<<"修改组名"<<groupIndex<<newGroupName;
}

void CusListModel::editDeviceGroup(const QString& macAddress, int newGroupNo)
{
	if (m_MapDeviceInfo.contains(macAddress))
	{
		QPair<int, QString> infoPair = m_MapDeviceInfo[macAddress];
		infoPair.first = newGroupNo;//修改设备分组
		m_MapDeviceInfo[macAddress] = infoPair;

		//int rowIndex = getRow(macAddress);
		//if (rowIndex != -1)
		//{
		//	//Update data
		//	m_ListPrinterItem[rowIndex]->m_pcGroup = newGroupNo;
		//	QModelIndex modelIndex = createIndex(rowIndex, 0);
		//	emit dataChanged(modelIndex, modelIndex, QVector<int>() << ListModelItem::E_GroupNo);
		//}
	}
}

void CusListModel::editDeviceName(const QString& macAddress, const QString& newDeviceName)
{
	if (m_MapDeviceInfo.contains(macAddress) && !newDeviceName.isEmpty())
	{
		QPair<int, QString> infoPair = m_MapDeviceInfo[macAddress];
		infoPair.second = newDeviceName;//修改设备名称
		m_MapDeviceInfo[macAddress] = infoPair;
	}
}

qint64 CusListModel::getCurrentUtcTime()
{
	return QDateTime::currentMSecsSinceEpoch();
}

QDateTime CusListModel::getCurrentDateTime(bool start)
{
	QDateTime dateTime = QDateTime::currentDateTime();
	return (start ? dateTime : dateTime.addSecs(60));
}

ListModelItem *CusListModel::getDeviceItem(const QString &macAddress)
{
 //   ListModelItem *item = nullptr;
 //   int rowIndex = getRow(macAddress);
	//if (rowIndex != -1) item = m_ListPrinterItem[rowIndex];

	//return item;

	ListModelItem* item = m_MapPrinterItem.contains(macAddress) ? m_MapPrinterItem[macAddress] : nullptr;
	return item;
}

int CusListModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent)
	//return m_ListPrinterItem.count();
	return m_MapPrinterItem.count();
}

QVariant CusListModel::data(const QModelIndex& index, int role) const
{
	if (index.row() < 0 || index.row() >= m_MapPrinterItem.count())
		return QVariant();

	QVariant res;
	QList<ListModelItem*> listItem = m_MapPrinterItem.values();
	ListModelItem * modelInfo = listItem.at(index.row());

	switch (role)
	{
		case ListModelItem::E_PrinterID:
			res = modelInfo->pcPrinterID();
			break;
		case ListModelItem::E_PrinterModel:
			res = modelInfo->pcPrinterModel();
			break;
		case ListModelItem::E_PrinterState:
			res = modelInfo->pcPrinterState();
			break;
		case ListModelItem::E_PrinterStatus:
			res = modelInfo->pcPrinterStatus();
			break;
		case ListModelItem::E_PrinterMethod:
			res = modelInfo->pcPrinterMethod();
			break; 
		case ListModelItem::E_HasCamera:
			res = modelInfo->pcHasCamera();
			break;
		case ListModelItem::E_IpAddr:
			res = modelInfo->pcIpAddr();
			break;
		case ListModelItem::E_GCodeName:
			res = modelInfo->pcGCodeName();
			break;
		case ListModelItem::E_TotalPrintTime:
			res = modelInfo->pcTotalPrintTime();
			break;
		case ListModelItem::E_LeftTime:
			res = modelInfo->pcLeftTime();
			break;
		case ListModelItem::E_CurPrintLayer:
			res = modelInfo->pcCurPrintLayer();
			break;
		case ListModelItem::E_TotalPrintLayer:
			res = modelInfo->pcTotalPrintLayer();
			break;
		case ListModelItem::E_CurPrintProgress:
			res = modelInfo->pcCurPrintProgress();
			break;
		case ListModelItem::E_BedTemp:
			res = modelInfo->pcBedTemp();
			break;
		case ListModelItem::E_NozzleTemp:
			res = modelInfo->pcNozzleTemp();
			break;
		case ListModelItem::E_GroupNo:
			res = modelInfo->pcGroup();
			break;
		case ListModelItem::E_GCodeFiles:
			res = modelInfo->gCodeFileList();
			break;
		//需要双向交互的数据直接传递对象指针
		case ListModelItem::E_HasSelect:
		case ListModelItem::E_DeviceName:
		case ListModelItem::E_PrintSpeed:
		case ListModelItem::E_BedDstTemp:
		case ListModelItem::E_NozzleDstTemp:
		//case ListModelItem::E_X:
		//case ListModelItem::E_Y:
		//case ListModelItem::E_Z:
		//case ListModelItem::E_FanOpened:
		//case ListModelItem::E_LedOpened:
		case ListModelItem::E_TransProgress:
			res = QVariant::fromValue(modelInfo);
			break;
		default:
			break;
	}
	return res;
}

QHash<int, QByteArray> CusListModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[ListModelItem::E_PrinterID] = "printerID";
	roles[ListModelItem::E_PrinterModel] = "printerModel";
	roles[ListModelItem::E_PrinterState] = "printerState";
	roles[ListModelItem::E_PrinterStatus] = "printerStatus"; 
	roles[ListModelItem::E_PrinterMethod] = "printerMethod";
	roles[ListModelItem::E_HasCamera] = "hasCamera";
	roles[ListModelItem::E_HasSelect] = "mItem";  //select
	roles[ListModelItem::E_DeviceName] = "mItem"; //deviceName
	roles[ListModelItem::E_IpAddr] = "ipAddr";
	roles[ListModelItem::E_GCodeName] = "gCodeName";
	roles[ListModelItem::E_TotalPrintTime] = "totalPrintTime";
	roles[ListModelItem::E_LeftTime] = "leftTime";
	roles[ListModelItem::E_CurPrintLayer] = "curPrintLayer";
	roles[ListModelItem::E_TotalPrintLayer] = "totalPrintLayer";
	roles[ListModelItem::E_CurPrintProgress] = "curPrintProgress";
	roles[ListModelItem::E_PrintSpeed] = "mItem"; //printSpeed
	roles[ListModelItem::E_BedDstTemp] = "mItem"; //bedDstTemp
	roles[ListModelItem::E_NozzleDstTemp] = "mItem"; //nozzleDstTemp
	roles[ListModelItem::E_BedTemp] = "bedTemp";
	roles[ListModelItem::E_NozzleTemp] = "nozzleTemp";
	roles[ListModelItem::E_GroupNo] = "groupNo";
	//roles[ListModelItem::E_X] = "mItem";
	//roles[ListModelItem::E_Y] = "mItem";
	//roles[ListModelItem::E_Z] = "mItem";
	//roles[ListModelItem::E_FanOpened] = "mItem";
	//roles[ListModelItem::E_LedOpened] = "mItem";
    roles[ListModelItem::E_TransProgress] = "mItem";//progress
	roles[ListModelItem::E_GCodeFiles] = "gcodeFiles";
	return roles;
}

void CusListModel::slotGetInfoSuccess(RemotePrinter stateInfo)
{
	//qDebug() << "slotGetInfoSuccess ThreadID:" << QThread::currentThreadId();
	QString macAddress = stateInfo.macAddress;
	QString printerModel = stateInfo.printerName.simplified();

	bool notExist = !m_MapPrinterItem.contains(macAddress);
	ListModelItem* item = notExist ? new ListModelItem(this) : m_MapPrinterItem[macAddress];

	//int rowIndex = getRow(macAddress);
	//ListModelItem* item = (rowIndex < 0) ? new ListModelItem() : m_ListPrinterItem[rowIndex];

	//手动添加设备
	if (m_bIsConnecting && stateInfo.ipAddress == m_curConnectingIpAddr)
	{
		m_bIsConnecting = false;
		m_curConnectingIpAddr.clear();
		emit sigConnectedIpSuccessed();
	}
	//记录设备信息
	QPair<int, QString> infoDef(1, printerModel);
	QPair<int, QString> infoPair = m_MapDeviceInfo.value(macAddress, infoDef);

	int currentItemGroup = (infoPair.first != 1) ? infoPair.first : 1; 
	QString curItemDeviceName = (infoPair.second != printerModel) ? infoPair.second : printerModel;
	m_MapDeviceInfo[macAddress] = QPair<int, QString>(currentItemGroup, curItemDeviceName);

	item->setPcPrinterID(macAddress);
	item->setPcPrinterModel(printerModel);
	item->setPcPrinterState(stateInfo.printState);
	item->setPcPrinterStatus(stateInfo.printerStatus);
	item->setPcPrinterMethod(stateInfo.printMode);
	item->setPcHasCamera(stateInfo.video);
	item->setPcIpAddr(stateInfo.ipAddress);
	item->setPcGCodeName(stateInfo.printFileName);
	item->setPcTotalPrintTime(stateInfo.printJobTime);
	item->setPcLeftTime(stateInfo.printLeftTime);
	item->setPcCurPrintLayer(stateInfo.layer);
	item->setPcTotalPrintLayer(stateInfo.totalLayer);
	item->setPcCurPrintProgress(stateInfo.printProgress);
	item->setPcPrintSpeed(stateInfo.printSpeed);
	item->setPcBedDstTemp(stateInfo.begTempTarget);
	item->setPcNozzleDstTemp(stateInfo.nozzleTempTarget);
	item->setPcBedTemp(stateInfo.bedTemp);
	item->setPcNozzleTemp(stateInfo.nozzleTemp);
	item->setFanOpened(stateInfo.fanSwitchState);
	item->setLedOpened(stateInfo.ledState);
	//item->setGCodeTransFinished(false);
	item->setPcGroup(currentItemGroup);
	item->setPcDeviceName(curItemDeviceName);

	QStringList postion = stateInfo.curPosition.split(" ");
	if (postion.size() == 3)
	{
		item->setPcX(QString(postion[0]).mid(2).toFloat());
		item->setPcY(QString(postion[1]).mid(2).toFloat());
		item->setPcZ(QString(postion[2]).mid(2).toFloat());
	}

	if (notExist) {
		//Add item
		beginInsertRows(QModelIndex(), rowCount(), rowCount());
		//m_ListPrinterItem.append(item);
		m_MapPrinterItem.insert(macAddress, item);
		endInsertRows();
	}

	//Update data
	emit dataChanged(createIndex(0, 0), createIndex(rowCount(), 0));
	//qDebug() << "ipAddress" << stateInfo.ipAddress << "printerModel" << printerModel;
	//qDebug() << "count" << m_MapPrinterItem.count() << "printMode" << stateInfo.printMode;
}

void CusListModel::slotGetFileList(const QString& from_std_string, const QString& from_std_list)
{
	//qDebug() << "slotGetFileList ThreadID:" << QThread::currentThreadId();
	//int rowIndex = getRow(from_std_string);
	//if (rowIndex == -1) return;
	if (!m_MapPrinterItem.contains(from_std_string)) return;

	QStringList fileList;
	if (!from_std_list.isEmpty())
	{
		QStringList splitList = from_std_list.split("\r\n", QString::SkipEmptyParts);
		foreach(QString fullInfo, splitList)
		{
			if (m_infoRegExp.exactMatch(fullInfo))
			{
				double fileSize = m_infoRegExp.cap(1).toDouble();
				QString fileTime = m_infoRegExp.cap(2); QString fileName = m_infoRegExp.cap(3);
				QString fileInfo = QString("%1#%2MB#%3").arg(fileName).arg(fileSize / 1024.0 / 1024.0, 2, 'f', 2, QChar('0')).arg(fileTime);

				fileList.append(fileInfo);
			}
		}
	}

	//qDebug() << "from_std_string" << from_std_string << fileList;
	m_MapPrinterItem[from_std_string]->setGCodeFileList(fileList);
}

void CusListModel::slotGetPreviewImage(const QString& from_std_macAddr, const QImage& previewImage)
{
	if (!m_MapPrinterItem.contains(from_std_macAddr)) return;

	m_previewProvider->setImage(previewImage, from_std_macAddr);
}

void CusListModel::readConfig()
{
	QSettings setting(m_appDataPath + "/Creality/Creative3D/deviceInfo.ini", QSettings::IniFormat);

	//分组名称
	setting.beginGroup("deviceGroupNames");
		QStringList groupNames;
		foreach(QString keyName, setting.allKeys())
		{
			QString groupName = setting.value(keyName, QString()).toString();
			groupNames.append(groupName.isEmpty() ? "---" : groupName);
		}
		if (!groupNames.isEmpty()) m_deviceGroupNames = groupNames;
	setting.endGroup();

	//设备信息
	QRegExp regExp("[\\d]_.{0,16}");
	setting.beginGroup("deviceInfomation");
		foreach(QString macAddress, setting.allKeys())
		{
			QString strInfo = setting.value(macAddress, "1_Printer").toString();
			if (regExp.exactMatch(strInfo))
			{
				QStringList listInfo = strInfo.split("_");
				QPair<int, QString> pairInfo(listInfo[0].toInt(), listInfo[1]);
				m_MapDeviceInfo.insert(macAddress, pairInfo);
			}
		}
	setting.endGroup();
}

void CusListModel::writeConfig()
{
	QSettings setting(m_appDataPath + "/Creality/Creative3D/deviceInfo.ini", QSettings::IniFormat);

	//分组名称
	setting.beginGroup("deviceGroupNames");
		for (int i = 0; i < m_deviceGroupNames.count(); ++i)
		{
			QString keyName = QString("groupName%1").arg(i);
			QString groupName = m_deviceGroupNames[i];
			setting.setValue(keyName, groupName);
		}
	setting.endGroup();

	//设备信息
	setting.beginGroup("deviceInfomation");
		foreach(QString macAddress, m_MapDeviceInfo.keys())
		{
			QPair<int, QString> pairInfo = m_MapDeviceInfo[macAddress];
			if (pairInfo.second.isEmpty()) continue;

			QString strInfo = QString("%1_%2").arg(pairInfo.first).arg(pairInfo.second);
			setting.setValue(macAddress, strInfo);
		}
	setting.endGroup();
}

//int CusListModel::getRow(QString macAddr, bool hasMac)
//{
//	if (macAddr.isEmpty() || m_ListPrinterItem.isEmpty()) return -1;
//
//    auto it = std::find_if(m_ListPrinterItem.begin(), m_ListPrinterItem.end(), [=](const ListModelItem* item) {
//		QString compare = item ? item->m_pcPrinterID : QString();
//        return (macAddr == compare);
//    });
//
//    return (it != m_ListPrinterItem.end())? std::distance(m_ListPrinterItem.begin(), it) : -1;
//}

void CusListModel::UploadProgressCallBack(const std::string& strMac, const float& progress)
{
	//qDebug() << "Upload Progress" << progress;
	QString macAddress = QString::fromStdString(strMac);
	ListModelItem* item = getDeviceItem(macAddress);

	if (item)
	{
		bool success = qFuzzyCompare(progress, 1.0f);
		item->setGCodeTransProgress(success ? 100.0f : qRound(progress * 100.0f));
		if (success) sendUIControlCmdToDevice(item->pcIpAddr(), (int)PrintControlType::PRINT_START, m_gcodeUploadPath);//Auto Start
	}
}

void CusListModel::UploadMessageCallBack(const std::string& strMac, const std::string& errMsg)
{
	//qDebug() << "Upload Error" << errMsg;
	QString macAddress = QString::fromStdString(strMac);
	ListModelItem* item = getDeviceItem(macAddress);
	if (item) item->setGCodeTransProgress(-1);
}

void CusListModel::ImportProgressCallBack(const std::string& strMac, const float& progress)
{
	//qDebug() << "Import Progress" << progress;
	QString macAddress = QString::fromStdString(strMac);
	ListModelItem* item = getDeviceItem(macAddress);

	if (item) 
	{
		bool success = qFuzzyCompare(progress, 1.0f);
		item->setGCodeImportProgress(success ? 100.0f : qRound(progress * 100.0f));
		if (success) item->setGCodeImportProgress(0);//Reset value
	}
}

void CusListModel::ImportMessageCallBack(const std::string& strMac, const std::string& errMsg)
{
	//qDebug() << "Import Error" << errMsg;
	QString macAddress = QString::fromStdString(strMac);
	ListModelItem* item = getDeviceItem(macAddress);
	if (item) item->setGCodeImportProgress(-1);
}

/******************** ListModelItem ********************/
ListModelItem::ListModelItem(QObject* parent) : QObject(parent), 
m_pcHasCamera(false), m_pcHasSelect(false), m_fanOpened(false), m_ledOpened(false),
m_pcPrinterState(0), m_pcPrinterStatus(0), m_pcPrinterMethod(0), m_pcTotalPrintTime(0), m_pcLeftTime(0), m_pcCurPrintLayer(0), m_pcTotalPrintLayer(0), m_pcCurPrintProgress(0),
m_pcPrintSpeed(0), m_pcBedDstTemp(0), m_pcNozzleDstTemp(0), m_pcBedTemp(0), m_pcNozzleTemp(0), m_pcGroup(1), m_pcX(0), m_pcY(0), m_pcZ(0), m_gCodeTransProgress(0), m_gCodeImportProgress(0)
{
	m_pcIpAddr = QString();
	m_pcGCodeName = QString();
	m_pcPrinterID = QString();
	m_pcDeviceName = QString();
	m_pcPrinterModel = QString();
	m_gCodeFileList = QStringList();
}

ListModelItem::ListModelItem(const ListModelItem &listModelItem) : QObject(nullptr)
{
    Q_UNUSED(listModelItem)
}

ListModelItem::~ListModelItem()
{

}

const QString &ListModelItem::pcPrinterID() const
{
    return m_pcPrinterID;
}

void ListModelItem::setPcPrinterID(const QString& printerID)
{
	if (m_pcPrinterID == printerID) 
		return;

	m_pcPrinterID = printerID;
	emit pcPrinterIDChanged();
}

const QString &ListModelItem::pcPrinterModel() const
{
    return m_pcPrinterModel;
}

void ListModelItem::setPcPrinterModel(const QString& printerModel)
{
	if (m_pcPrinterModel == printerModel) 
		return;

	m_pcPrinterModel = printerModel;
	emit pcPrinterModelChanged();
}

int ListModelItem::pcPrinterState() const
{
    return m_pcPrinterState;
}

void ListModelItem::setPcPrinterState(int state)
{
	if (m_pcPrinterState == state) 
		return;

	m_pcPrinterState = state;
	emit pcPrinterStateChanged();
}

int ListModelItem::pcPrinterStatus() const
{
	return m_pcPrinterStatus;
}

void ListModelItem::setPcPrinterStatus(int status)
{
	if (m_pcPrinterStatus == status) 
		return;

	m_pcPrinterStatus = status;
	emit pcPrinterStatusChanged();
}

int ListModelItem::pcPrinterMethod() const
{
	return m_pcPrinterMethod;
}

void ListModelItem::setPcPrinterMethod(int method)
{
	if (m_pcPrinterMethod == method)
		return;

	m_pcPrinterMethod = method;
	emit pcPrinterMethodChanged();
}

int ListModelItem::pcHasCamera() const
{
    return m_pcHasCamera;
}

void ListModelItem::setPcHasCamera(int hasCamera)
{
	if (m_pcHasCamera == hasCamera)
		return;

	m_pcHasCamera = hasCamera;
	emit pcHasCameraChanged();
}

int ListModelItem::pcHasSelect() const
{
	return m_pcHasSelect;
}

void ListModelItem::setPcHasSelect(int hasSelect)
{
	if (m_pcHasSelect == hasSelect)
		return;

	m_pcHasSelect = hasSelect;

	emit pcHasSelectChanged();
}

const QString &ListModelItem::pcDeviceName() const
{
    return m_pcDeviceName;
}

void ListModelItem::setPcDeviceName(const QString& deviceName)
{
    if (m_pcDeviceName == deviceName)
        return;

    m_pcDeviceName = deviceName;
    emit pcDeviceNameChanged();
}

const QString &ListModelItem::pcIpAddr() const
{
    return m_pcIpAddr;
}

void ListModelItem::setPcIpAddr(const QString& ipAddr)
{
	if (m_pcIpAddr == ipAddr)
		return;

	m_pcIpAddr = ipAddr;
	emit pcIpAddrChanged();
}

const QString &ListModelItem::pcGCodeName() const
{
    return m_pcGCodeName;
}

void ListModelItem::setPcGCodeName(const QString& gcodeName)
{
	if (m_pcGCodeName == gcodeName)
		return;

	m_pcGCodeName = gcodeName;
	emit pcGCodeNameChanged();
}

int ListModelItem::pcTotalPrintTime() const
{
    return m_pcTotalPrintTime;
}

void ListModelItem::setPcTotalPrintTime(int totalPrintTime)
{
	if (m_pcTotalPrintTime == totalPrintTime)
		return;

	m_pcTotalPrintTime = totalPrintTime;
	emit pcTotalPrintTimeChanged();
}

int ListModelItem::pcLeftTime() const
{
    return m_pcLeftTime;
}

void ListModelItem::setPcLeftTime(int leftTime)
{
	if (m_pcLeftTime == leftTime)
		return;

	m_pcLeftTime = leftTime;
	emit pcLeftTimeChanged();
}

int ListModelItem::pcCurPrintLayer() const
{
	return m_pcCurPrintLayer;
}

void ListModelItem::setPcCurPrintLayer(int layer)
{
	if (m_pcCurPrintLayer == layer)
		return;

	m_pcCurPrintLayer = layer;
	emit pcCurPrintLayerChanged();
}

int ListModelItem::pcTotalPrintLayer() const
{
	return m_pcTotalPrintLayer;
}

void ListModelItem::setPcTotalPrintLayer(int totalLayer)
{
	if (m_pcTotalPrintLayer == totalLayer)
		return;

	m_pcTotalPrintLayer = totalLayer;
	emit pcTotalPrintLayerChanged();
}

int ListModelItem::pcCurPrintProgress() const
{
    return m_pcCurPrintProgress;
}

void ListModelItem::setPcCurPrintProgress(int progress)
{
	if (m_pcCurPrintProgress == progress)
		return;

	m_pcCurPrintProgress = progress;
	emit pcCurPrintProgressChanged();
}

int ListModelItem::pcPrintSpeed() const
{
    return m_pcPrintSpeed;
}

void ListModelItem::setPcPrintSpeed(int speed)
{
    if (m_pcPrintSpeed == speed)
        return;

    m_pcPrintSpeed = speed;
    emit pcPrintSpeedChanged();
}

int ListModelItem::pcBedDstTemp() const
{
    return m_pcBedDstTemp;
}

void ListModelItem::setPcBedDstTemp(int bedDstTemp)
{
    if (m_pcBedDstTemp == bedDstTemp)
        return;

    m_pcBedDstTemp = bedDstTemp;
    emit pcBedDstTempChanged();
}

int ListModelItem::pcNozzleDstTemp() const
{
    return m_pcNozzleDstTemp;
}

void ListModelItem::setPcNozzleDstTemp(int nozzleDstTemp)
{
    if (m_pcNozzleDstTemp == nozzleDstTemp)
        return;

    m_pcNozzleDstTemp = nozzleDstTemp;
    emit pcNozzleDstTempChanged();
}

int ListModelItem::pcBedTemp() const
{
    return m_pcBedTemp;
}

void ListModelItem::setPcBedTemp(int bedTemp)
{
	if (m_pcBedTemp == bedTemp)
		return;

	m_pcBedTemp = bedTemp;
	emit pcBedTempChanged();
}

int ListModelItem::pcNozzleTemp() const
{
    return m_pcNozzleTemp;
}

void ListModelItem::setPcNozzleTemp(int nozzleTemp)
{
	if (m_pcNozzleTemp == nozzleTemp)
		return;

	m_pcNozzleTemp = nozzleTemp;
	emit pcNozzleTempChanged();
}

int ListModelItem::pcGroup() const
{
    return m_pcGroup;
}

void ListModelItem::setPcGroup(int groupNo)
{
    if (m_pcGroup == groupNo)
        return;

    m_pcGroup = groupNo;
    emit pcGroupChanged();
}

float ListModelItem::pcX() const
{
    return m_pcX;
}

void ListModelItem::setPcX(float newPcX)
{
    if (qFuzzyCompare(m_pcX, newPcX))
        return;

    m_pcX = newPcX;
    emit pcXChanged();
}

float ListModelItem::pcY() const
{
    return m_pcY;
}

void ListModelItem::setPcY(float newPcY)
{
    if (qFuzzyCompare(m_pcY, newPcY))
        return;

    m_pcY = newPcY;
    emit pcYChanged();
}

float ListModelItem::pcZ() const
{
    return m_pcZ;
}

void ListModelItem::setPcZ(float newPcZ)
{
    if (qFuzzyCompare(m_pcZ, newPcZ))
        return;

    m_pcZ = newPcZ;
    emit pcZChanged();
}

bool ListModelItem::fanOpened() const
{
    return m_fanOpened;
}

void ListModelItem::setFanOpened(bool fanOpened)
{
    if (m_fanOpened == fanOpened)
        return;

    m_fanOpened = fanOpened;
    emit fanOpenedChanged();
}

bool ListModelItem::ledOpened() const
{
    return m_ledOpened;
}

void ListModelItem::setLedOpened(bool ledOpened)
{
    if (m_ledOpened == ledOpened)
        return;

    m_ledOpened = ledOpened;
    emit ledOpenedChanged();
}

float ListModelItem::gCodeTransProgress() const
{
    return m_gCodeTransProgress;
}

void ListModelItem::setGCodeTransProgress(float progress)
{
	if (qFuzzyCompare(m_gCodeTransProgress, progress))
		return;

	m_gCodeTransProgress = progress;
    emit gCodeTransProgressChanged();
}

float ListModelItem::gCodeImportProgress() const
{
	return m_gCodeImportProgress;
}

void ListModelItem::setGCodeImportProgress(float progress)
{
	if (qFuzzyCompare(m_gCodeImportProgress, progress))
		return;

	m_gCodeImportProgress = progress;
	emit gCodeImportProgressChanged();
}

const QStringList& ListModelItem::gCodeFileList() const
{
	return m_gCodeFileList;
}

void ListModelItem::setGCodeFileList(const QStringList& fileList)
{
	m_gCodeFileList = fileList;
	emit gCodeFileListChanged();
}

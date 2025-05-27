#include <QTime>
//#include <external/inout/cxopenandsavefilemanager.h>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "cuswifimanagermodel.h"
#include <QString>
#include <QVariantMap>
#include <QDebug>
#include <QTimer>
#include "localnetworkinterface/RemotePrinterManager.h"
#include "kernel/abstractkernel.h"
#include <QMetaObject>
#pragma execution_character_set("utf-8")
static int process = 0;
using namespace creative_kernel;
CusWifiManagerModel::CusWifiManagerModel()
{
    //
    m_DataType = SEARCH_DATA;
}

void CusWifiManagerModel::changeDataSource(int dataType)
{
    //1.切换信号到新的绑定关系
    //2.解除旧的绑定关系
    //3.刷新当前数据类型
    //4.重新发起获取设备列表的请求

    switch (dataType) {
    case SEARCH_DATA:
    {
        break;
    }
    case MANUAL_DATA:
    {
        break;
    }
    }
    //3
    m_DataType = (CurDataType)dataType;
}

void CusWifiManagerModel::refreshWifiList()
{
    if (m_DataType == SEARCH_DATA)
    {
        getRemotePrinterManager()->searchDevices();
    }
}

void CusWifiManagerModel::sendFile(QString dn, QString filePath)
{
    filePath = filePath.replace("file:///","");
    if (filePath.isEmpty())
    {
        return;
    }
	getRemotePrinterManager()->pushFile(m_mapPrinterInfo[dn], filePath.toStdString(), 
        std::bind(&CusWifiManagerModel::onFileTransProgress, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&CusWifiManagerModel::onFileTransMessage, this, std::placeholders::_1, std::placeholders::_2));
}

void CusWifiManagerModel::findDeviceList()
{
    if (!m_bFirstFind)
    {
        qDebug()<< "CusWifiManagerModel::findDeviceList()" ;
        connect(this, &CusWifiManagerModel::sigGetInfoSuccess, this, &CusWifiManagerModel::slotGetInfoSuccess, Qt::ConnectionType::QueuedConnection);
        connect(this, &CusWifiManagerModel::sigFileTransProgress, this, &CusWifiManagerModel::slotFileTransProgress);
        //getRemotePrinterManager()->setSearchCb();
        getRemotePrinterManager()->setGetInfoCb(std::bind(&CusWifiManagerModel::onGetInfoSuccess, this, std::placeholders::_1));
        getRemotePrinterManager()->setGetFileListCb(std::bind(&CusWifiManagerModel::onGetFileList, this, std::placeholders::_1, std::placeholders::_2));
		getRemotePrinterManager()->searchDevices();
        m_bFirstFind = true;
    }
}

void CusWifiManagerModel::createConnect(QString strIp)
{
    //调用连接接口
    m_bIsConnectingIp = true;
    m_curConnectingIp = strIp;

    RemotePrinter printer;
    printer.ipAddress = strIp;
    printer.type = RemotePrinerType::REMOTE_PRINTER_TYPE_LAN;
    printer.uniqueId = printer.ipAddress.isEmpty() ? printer.macAddress : printer.ipAddress;
    getRemotePrinterManager()->addPrinter(printer);
}

void CusWifiManagerModel::reconnectIp(QString dn)
{
    int rowIndex = getRow(dn);

    if(rowIndex == -1)
        return;

    CloudAndWifiInfo info = m_CloudAndWifiList.at(rowIndex);
    info.setIsConnected(true);
    m_CloudAndWifiList.replace(rowIndex, info);

    refreshItem(getRow(dn), 2);
}

Q_INVOKABLE void CusWifiManagerModel::sendUIControlCmdToDevice(QString strMac,int nCmdType, QString value)
{
    qDebug() << "cmdType =" << nCmdType << "----value =" << value;
    PrintControlType cmdType = (PrintControlType)nCmdType;
	getRemotePrinterManager()->controlPrinter(m_mapPrinterInfo[strMac], cmdType, value.toStdString());
}
void CusWifiManagerModel::updateModelDataByPrinterStates(RemotePrinter curState, CloudAndWifiInfo &info)
{
    if (curState.macAddress != info.deviceDN())
    {
        return;
    }
    if (curState.deviceName != info.deviceName())
    {
        //修改表名
    }

    bool isAllowTrans = false;
    if (curState.printerStatus == 1 && curState.TFCardStatus == 1)
    {
        isAllowTrans = true;
    }

    if (isAllowTrans != info.isAllowTrans())
    {
        //刷新是否支持传输
        refreshIsAllowTrans(info.deviceDN(), isAllowTrans);
    }

    //刷新打印状态
    if (info.curState() != curState.printState)
    {
        refreshCurState(info.deviceDN(), curState.printState);
    }

    //刷新进度
    if (info.curProgress() != curState.printProgress)
    {
        refreshProgress(info.deviceDN(), curState.printProgress);
    }
}

Q_INVOKABLE void CusWifiManagerModel::removeTmpFilePath(QString filePath)
{
    if (QFile::exists(filePath))
    {
        QFile::remove(filePath);
    }
}

Q_INVOKABLE void CusWifiManagerModel::resetSomeStatus(QString strMac)
{
    refreshTransProgress(strMac, 0);

	QVariantMap map;
	auto curState = m_mapPrinterInfo[strMac];
	map.insert("ipAddress", QVariant::fromValue(curState.ipAddress));
	map.insert("macAddress", QVariant::fromValue(curState.macAddress));
	map.insert("deviceName", QVariant::fromValue(curState.deviceName));
	map.insert("productKey", QVariant::fromValue(curState.productKey));
	map.insert("printerName", QVariant::fromValue(curState.printerName));
	map.insert("printFileName", QVariant::fromValue(curState.printFileName));
	map.insert("curPosition", QVariant::fromValue(curState.curPosition));
	map.insert("printerStatus", QVariant::fromValue(curState.printerStatus));
	map.insert("TFCardStatus", QVariant::fromValue(curState.TFCardStatus));
	map.insert("printState", QVariant::fromValue(curState.printState));
	map.insert("fanSwitchState", QVariant::fromValue(curState.fanSwitchState));
	map.insert("printProgress", QVariant::fromValue(curState.printProgress));
	map.insert("nozzleTemp", QVariant::fromValue(curState.nozzleTemp));
	map.insert("nozzleTempTarget", QVariant::fromValue(curState.nozzleTempTarget));
	map.insert("bedTemp", QVariant::fromValue(curState.bedTemp));
	map.insert("begTempTarget", QVariant::fromValue(curState.begTempTarget));
	map.insert("printSpeed", QVariant::fromValue(curState.printSpeed));
	map.insert("printJobTime", QVariant::fromValue(curState.printJobTime));
	map.insert("printLeftTime", QVariant::fromValue(curState.printLeftTime));
	map.insert("video", QVariant::fromValue(curState.video));

	emit sigPrinterStateChanged(strMac, map);
}

Q_INVOKABLE bool CusWifiManagerModel::isConnected(QString strIp)
{
    bool ret = false;
    for (auto item : m_mapPrinterInfo)
    {
        if (item.ipAddress == strIp)
        {
            ret = true;
            break;
        }
    }
    return ret;
}

void CusWifiManagerModel::onGetFileList(const std::string& strMac, const std::string& filelist)
{
    QStringList list = QString(filelist.c_str()).split("\r");

    QStringList files;
    for (auto const& item : list)
    {
        if (item.endsWith(".gcode"))
        {
            files.push_back(item.trimmed());
        }
    }
    if (m_mapPrinterFileList.contains(strMac.c_str()))
    {
        m_mapPrinterFileList[strMac.c_str()] = files;
    }
    else
    {
        m_mapPrinterFileList.insert(strMac.c_str(), files);
    }
}

void CusWifiManagerModel::onFileTransProgress(const std::string& strMac, const float& progress)
{
    emit sigFileTransProgress(strMac.c_str(), progress);
}

void CusWifiManagerModel::onFileTransMessage(const std::string& strMac, const std::string& errMsg)
{
    emit sigFileTransMessage(strMac.c_str(), QString(errMsg.c_str()));
}

void CusWifiManagerModel::onGetInfoSuccess(RemotePrinter curState)
{
    emit sigGetInfoSuccess(curState);
}

void CusWifiManagerModel::slotGetInfoSuccess(RemotePrinter curState)
{
    QString macStr = curState.macAddress;
    //qDebug() << "CusWifiManagerModel::slotGetInfoSuccess";
    bool isExist = false;
    int devConnectType = 1;         //1:自动搜索  2:手动连接
    if (m_bIsConnectingIp)
    {
        if (curState.ipAddress == m_curConnectingIp)
        {
            devConnectType = 2;
            emit sigConnectedIpSuccessed();
            m_bIsConnectingIp = false;
        }
    }
    CloudAndWifiInfo info;
    m_mapPrinterInfo.insert(macStr, curState);
    for (int i = 0; i < m_CloudAndWifiList.count(); i++)
    {
        if (m_CloudAndWifiList.at(i).deviceIP() == curState.ipAddress)
        {
            info = m_CloudAndWifiList.at(i);
            isExist = true;
            break;
        }
    }
    if (!isExist)
    {
        CloudAndWifiInfo info;
        info.setDeviceName(curState.printerName);
        info.setDeviceIP(curState.ipAddress);
        info.setHasCamera(false);
        info.setCurProgress(curState.printProgress);
        info.setTransFileNameDisplay(curState.printFileName);
        info.setTransFilePath(curState.printFileName);
        info.setcurState(0);
        info.setIsConnected(true);
        info.setDeviceDN(curState.macAddress);
        info.setDevConnectType(devConnectType);
        info.setCurTransProgress(0);
        addModelData(info);
    }
    else
    {
        updateModelDataByPrinterStates(curState, info);

        QVariantMap map;
        map.insert("ipAddress", QVariant::fromValue(curState.ipAddress));
        map.insert("macAddress", QVariant::fromValue(curState.macAddress));
        map.insert("deviceName", QVariant::fromValue(curState.deviceName));
        map.insert("productKey", QVariant::fromValue(curState.productKey));
        map.insert("printerName", QVariant::fromValue(curState.printerName));
        map.insert("printFileName", QVariant::fromValue(curState.printFileName));
        map.insert("curPosition", QVariant::fromValue(curState.curPosition));
        map.insert("printerStatus", QVariant::fromValue(curState.printerStatus));
        map.insert("TFCardStatus", QVariant::fromValue(curState.TFCardStatus));
        map.insert("printState", QVariant::fromValue(curState.printState));
        map.insert("fanSwitchState", QVariant::fromValue(curState.fanSwitchState));
        map.insert("printProgress", QVariant::fromValue(curState.printProgress));
        map.insert("nozzleTemp", QVariant::fromValue(curState.nozzleTemp));
        map.insert("nozzleTempTarget", QVariant::fromValue(curState.nozzleTempTarget));
        map.insert("bedTemp", QVariant::fromValue(curState.bedTemp));
        map.insert("begTempTarget", QVariant::fromValue(curState.begTempTarget));
        map.insert("printSpeed", QVariant::fromValue(curState.printSpeed));
        map.insert("printJobTime", QVariant::fromValue(curState.printJobTime));
        map.insert("printLeftTime", QVariant::fromValue(curState.printLeftTime));
        map.insert("video", QVariant::fromValue(curState.video));

        emit sigPrinterStateChanged(macStr, map);
        //        updateQmlJsonData(macStr,curState);
    }
}

void CusWifiManagerModel::slotGetInfoFailed(QString macStr)
{
    int rowIndex = getRow(macStr);

    if (rowIndex == -1)
        return;
    refreshIsConnected(rowIndex, false);
}

void CusWifiManagerModel::slotFileTransProgress(QString macStr, float progress)
{
    qDebug() << "CusWifiManagerModel::slotFileTransProgress %1,%2" << macStr << progress;

    refreshTransProgress(macStr,(int)(progress*100));
    //    emit sigFileTransProgress(macStr, progress * 100);
}

void CusWifiManagerModel::refreshItem(int row, int column)
{
    QModelIndex topLeft = createIndex(row,column);
    emit dataChanged(topLeft, topLeft);
}

QString CusWifiManagerModel::secToTime(QString sec)
{
    int iSec = sec.toInt();
    int H = iSec / (60*60);
    int M = (iSec- (H * 60 * 60)) / 60;
    int S = (iSec - (H * 60 * 60)) - M * 60;
    QString hour = QString::number(H);
    if (hour.length() == 1) hour = "0" + hour;
    QString min = QString::number(M);
    if (min.length() == 1) min = "0" + min;
    QString qstrSec = QString::number(S);
    if (qstrSec.length() == 1) qstrSec = "0" + qstrSec;
    QString qTime = hour + ":" + min + ":" + qstrSec;
    return qTime;
}

int CusWifiManagerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_CloudAndWifiList.count();
}

QVariant CusWifiManagerModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_CloudAndWifiList.count())
        return QVariant();

    const CloudAndWifiInfo &cwInfo = m_CloudAndWifiList[index.row()];
    switch (role)
    {
    case CHECK_ROLE:
        return cwInfo.selectState();
        break;
    case DEVICENAME_ROLE:
    {
        return cwInfo.deviceName();
    }
    case DEVICEIP_ROLE:
    {
        return cwInfo.deviceIP();
    }
    case HASCAMERA_ROLE:
    {
        return cwInfo.hasCamera();
    }
    case CURPROGRESS_ROLE:
    {
        return cwInfo.curProgress();
    }
    case TRANSFILENAME_ROLE:
    {
        return cwInfo.transFileNameDisplay();
    }
    case TRANSFILEPATH_ROLE:
    {
        return cwInfo.transFilePath();
    }
    case ISALLOWTRANS_ROLE:
    {
        return cwInfo.isAllowTrans();
    }
    case CURSTATE_ROLE:
    {
        return cwInfo.curState();
    }
    case DEVICEDN_ROLE:
    {
        return cwInfo.deviceDN();
    }
    case DEVICE_INTERNALNAME_ROLE:
    {
        return cwInfo.interDeviceName();
    }
    case FILELIST_ROLE:
    {
        return cwInfo.fileList();
    }
    case ISONLINE_ROLE:
    {
        return cwInfo.isOnline();
    }
    case ISCONNECT_ROLE:
    {
        return cwInfo.isConnected();
    }
    case CONNECTTYPE_ROLE:
    {
        return cwInfo.devConnectType();
    }
    case TRANSPROGRESS_ROLE:
        return cwInfo.curTransProgress();
    default:
    {
        return cwInfo.deviceName();
    }
    }

    return QVariant();
}

QHash<int, QByteArray> CusWifiManagerModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CHECK_ROLE] = "selectRow";
    roles[DEVICENAME_ROLE] = "deviceName";
    roles[DEVICE_INTERNALNAME_ROLE] = "internalDeviceName";
    roles[DEVICEIP_ROLE] = "deviceIP";
    roles[DEVICEDN_ROLE] = "deviceDN";
    roles[HASCAMERA_ROLE] = "hasCamera";
    roles[CURPROGRESS_ROLE] = "curProgress";
    roles[TRANSFILENAME_ROLE] = "transFileName";
    roles[TRANSFILEPATH_ROLE] = "transFilePath";
    roles[ISALLOWTRANS_ROLE] = "isAllowTrans";
    roles[FILELIST_ROLE] = "fileList";
    roles[CURSTATE_ROLE] = "curState";
    roles[ISONLINE_ROLE] = "isOnline";
    roles[ISCONNECT_ROLE] = "isConnected";
    roles[CONNECTTYPE_ROLE] = "connectType";
    roles[TRANSPROGRESS_ROLE] = "curTransProgress";
    return roles;
}


void CusWifiManagerModel::addModelData(CloudAndWifiInfo info)
{
    beginInsertRows(QModelIndex(), m_CloudAndWifiList.size(), m_CloudAndWifiList.size());
    m_CloudAndWifiList.push_back(info);
    endInsertRows();
}

int CusWifiManagerModel::getRow(QString dn)
{
    if(m_CloudAndWifiList.size() == 0)
        return -1;

    int rowIndex = 0;
    for(auto& data : m_CloudAndWifiList)
    {
        if(data.deviceDN() == dn)
        {
            break;
        }
        else if(rowIndex == m_CloudAndWifiList.count() - 1)
        {
            return -1;
        }

        rowIndex++;
    }
    //找不到
    if(rowIndex < 0 && rowIndex > m_CloudAndWifiList.count() - 1)
        return -1;

    return rowIndex;
}

QList<QString> CusWifiManagerModel::getSelectPrinterKeyList()
{
    int nCount = m_CloudAndWifiList.size();
    QList<QString> rowsMac;
    for (int row = nCount - 1; row >= 0; row--)
    {
        if (m_CloudAndWifiList.at(row).selectState())
        {
            rowsMac.push_back(m_CloudAndWifiList.at(row).deviceDN());
        }
    }
    return rowsMac;
}


void CusWifiManagerModel::refreshSelectStateByRow(QString dn, bool checked)
{
    int rowIndex = getRow(dn);

    if(rowIndex == -1)
        return;
    //beginResetModel();
    CloudAndWifiInfo info = m_CloudAndWifiList.at(rowIndex);
    info.setSelcetState(checked);
    m_CloudAndWifiList.replace(rowIndex, info);
    //endResetModel();
}

void CusWifiManagerModel::sendFileToPrinters(QString strFilePath)
{
    QList <QString > listPrinters = getSelectPrinterKeyList();
    for(int i = 0; i < listPrinters.count(); i++)
    {
        sendFile(listPrinters[i],strFilePath);
    }

}

Q_INVOKABLE QVariantMap CusWifiManagerModel::getGCodeListData(QString strMac)
{
    QVariantMap map;
    if (m_mapPrinterFileList.contains(strMac))
    {
        QStringList fileList = m_mapPrinterFileList[strMac];
        for (const auto file : fileList)
        {
            map.insert(file, "/media/mmcblk0p1/creality/gztemp/" + file);
        }
    }
	return map;
}



void CusWifiManagerModel::refreshCameraState(QString dn, int state)
{
    int rowIndex = getRow(dn);

    if(rowIndex == -1)
        return;

    CloudAndWifiInfo info = m_CloudAndWifiList.at(rowIndex);
    info.setHasCamera(state);
    m_CloudAndWifiList.replace(rowIndex, info);

    refreshItem(getRow(dn), 4);
}

void CusWifiManagerModel::refreshProgress(QString dn, float progress)
{
    int rowIndex = getRow(dn);

    if(rowIndex == -1)
        return;

    CloudAndWifiInfo info = m_CloudAndWifiList.at(rowIndex);
    info.setCurProgress(progress);
    m_CloudAndWifiList.replace(rowIndex, info);
    refreshItem(getRow(dn), 5);
}

void CusWifiManagerModel::refreshCurState(QString dn, int state)
{
    int rowIndex = getRow(dn);

    if(rowIndex == -1)
        return;

    CloudAndWifiInfo info = m_CloudAndWifiList.at(rowIndex);
    info.setcurState(state);
    m_CloudAndWifiList.replace(rowIndex, info);
    refreshItem(getRow(dn), 6);
}

void CusWifiManagerModel::refreshIsConnected(int rowIndex, bool isConnected)
{
    CloudAndWifiInfo info = m_CloudAndWifiList.at(rowIndex);
    info.setIsConnected(isConnected);
    m_CloudAndWifiList.replace(rowIndex, info);
    refreshItem(rowIndex, 2);
}

void CusWifiManagerModel::refreshIsAllowTrans(QString dn, bool isAllow)
{
    int rowIndex = getRow(dn);

    if(rowIndex == -1)
        return;

    CloudAndWifiInfo info = m_CloudAndWifiList.at(rowIndex);
    info.setIsAllowTrans(isAllow);
    m_CloudAndWifiList.replace(rowIndex, info);
    refreshItem(getRow(dn), 3);
}

void CusWifiManagerModel::refreshTransProgress(QString dn, int progress)
{
    int rowIndex = getRow(dn);

    if(rowIndex == -1)
        return;

    CloudAndWifiInfo info = m_CloudAndWifiList.at(rowIndex);
    info.setCurTransProgress(progress);
    m_CloudAndWifiList.replace(rowIndex, info);
    refreshItem(rowIndex, 1);
}

CloudAndWifiInfo::CloudAndWifiInfo()
{

}

CloudAndWifiInfo::~CloudAndWifiInfo()
{

}

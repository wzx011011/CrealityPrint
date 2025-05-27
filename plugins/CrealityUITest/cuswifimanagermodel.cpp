#include <QTime>
//#include <external/inout/cxopenandsavefilemanager.h>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "cuswifimanagermodel.h"
//#include "utils/wificontrolmng.h"
//#include "utils/wifiprinter.h"
//#include "report/slicereport.h"
//#include "kernel/abstractkernel.h"
//#include "devicenamemanager.h"
//#include "us/slicermanager.h"
//#include "parse/machinesetting.h"

#include<QString>
#include <QVariantMap>
#include <QDebug>
#include <QTimer>

#pragma execution_character_set("utf-8")
static int process = 0;

CusWifiManagerModel::CusWifiManagerModel()
{
    //
    m_DataType = WIFI_DATA;
//    connect(&WifiControlMng::instance(),SIGNAL(printerChanged(QVariant)),this,SLOT(slotPrinterChanged(QVariant)));
//    connect(&WifiControlMng::instance(),SIGNAL(progressChanged(QString, int)),this,SLOT(slotProgressChanged(QString, int)));
//    connect(&WifiControlMng::instance(),SIGNAL(statesChanged(QString, int)),this,SLOT(slotStatesChanged(QString, int)));
//    connect(&WifiControlMng::instance(),SIGNAL(cameraStatusChanged(QString, bool)),this,SLOT(slotCameraStatusChanged(QString, bool)));
//    connect(&WifiControlMng::instance(),SIGNAL(transError(QString, int, QString)),this,SLOT(slotTransError(QString, int, QString)));
//    connect(&WifiControlMng::instance(),SIGNAL(printError(QString, int, QString)),this,SLOT(slotPrintError(QString, int, QString)));
//    connect(&WifiControlMng::instance(), &WifiControlMng::updatePrintingPara, this, &CusWifiManagerModel::slotInitDeviceInfoByDn);

}

void CusWifiManagerModel::changeDataSource(int dataType)
{
    //1.切换信号到新的绑定关系
    //2.解除旧的绑定关系
    //3.刷新当前数据类型
    //4.重新发起获取设备列表的请求

    switch (dataType) {
    case WIFI_DATA:
    {


        //关闭创想云的 轮询
//        PrinterDeviceControl::instance().pauseGetDeviceTask();
//        //1
//        connect(&WifiControlMng::instance(),SIGNAL(printerChanged(QVariant)),this,SLOT(slotPrinterChanged(QVariant)));
//        connect(&WifiControlMng::instance(),SIGNAL(progressChanged(QString, int)),this,SLOT(slotProgressChanged(QString, int)));
//        connect(&WifiControlMng::instance(),SIGNAL(statesChanged(QString, int)),this,SLOT(slotStatesChanged(QString, int)));
//        connect(&WifiControlMng::instance(),SIGNAL(cameraStatusChanged(QString, bool)),this,SLOT(slotCameraStatusChanged(QString, bool)));
//        //2
//        disconnect(&PrinterDeviceControl::instance(),SIGNAL(printerChanged(QVariant)),this,SLOT(slotPrinterChanged(QVariant)));
//        disconnect(&PrinterDeviceControl::instance(),SIGNAL(progressChanged(QString, int)),this,SLOT(slotProgressChanged(QString, int)));
//        disconnect(&PrinterDeviceControl::instance(),SIGNAL(statesChanged(QString, int)),this,SLOT(slotStatesChanged(QString, int)));
//        disconnect(&PrinterDeviceControl::instance(),SIGNAL(cameraStatusChanged(QString, bool)),this,SLOT(slotCameraStatusChanged(QString, bool)));
        break;
    }
    case CLOUD_DATA:
    {
        //关闭wifi的轮询
//        WifiControlMng::instance().pauseAllPrinterTimer();
//        //1
//        connect(&PrinterDeviceControl::instance(),SIGNAL(printerChanged(QVariant)),this,SLOT(slotPrinterChanged(QVariant)));
//        connect(&PrinterDeviceControl::instance(),SIGNAL(progressChanged(QString, int)),this,SLOT(slotProgressChanged(QString, int)));
//        connect(&PrinterDeviceControl::instance(),SIGNAL(statesChanged(QString, int)),this,SLOT(slotStatesChanged(QString, int)));
//        connect(&PrinterDeviceControl::instance(),SIGNAL(cameraStatusChanged(QString, bool)),this,SLOT(slotCameraStatusChanged(QString, bool)));
//        //2
//        disconnect(&WifiControlMng::instance(),SIGNAL(printerChanged(QVariant)),this,SLOT(slotPrinterChanged(QVariant)));
//        disconnect(&WifiControlMng::instance(),SIGNAL(progressChanged(QString, int)),this,SLOT(slotProgressChanged(QString, int)));
//        disconnect(&WifiControlMng::instance(),SIGNAL(statesChanged(QString, int)),this,SLOT(slotStatesChanged(QString, int)));
//        disconnect(&WifiControlMng::instance(),SIGNAL(cameraStatusChanged(QString, bool)),this,SLOT(slotCameraStatusChanged(QString, bool)));
//        disconnect(&WifiControlMng::instance(),SIGNAL(transError(QString, int, QString)),this,SLOT(slotTransError(QString, int, QString)));
//        disconnect(&WifiControlMng::instance(),SIGNAL(printError(QString, int, QString)),this,SLOT(slotPrintError(QString, int, QString)));
//        disconnect(&WifiControlMng::instance(), &WifiControlMng::updatePrintingPara, this, &CusWifiManagerModel::slotInitDeviceInfoByDn);
        break;
    }
    }
    //3
    m_DataType = (CurDataType)dataType;
    //4
    refreshWifiList();
}

void CusWifiManagerModel::refreshWifiList(QVariantList dataList)
{
    beginResetModel();
    m_CloudAndWifiList.clear();
    for (QVariant data : dataList)
    {
        QVariantMap varMap = data.value<QVariantMap>();
        CloudAndWifiInfo info;
        info.setDeviceName(varMap["deviceName"].toString());
        info.setDeviceIP(varMap["deviceIP"].toString());
        info.setHasCamera(varMap["hasCamera"].toBool());
        info.setCurProgress(varMap["curProgress"].toInt());
        info.setTransFileNameDisplay(varMap["transFileName"].toString());
        info.setTransFilePath(varMap["transFilePath"].toString());
        info.setIsAllowTrans(varMap["isAllowTrans"].toInt());
        info.setcurState(varMap["curState"].toInt());
        info.setDeviceDN(varMap["dn"].toString());
        m_CloudAndWifiList << info;
    }
    endResetModel();
}

void CusWifiManagerModel::refreshWifiList()
{
    if (m_DataType == WIFI_DATA)
    {
        //刷新先清空表格数据
        beginResetModel();
        m_CloudAndWifiList.clear();
        qDebug()<< "CusWifiManagerModel::refreshWifiList()" ;

        for(int i = 0; i < 10; i ++)
        {
            CloudAndWifiInfo info;
            info.setDeviceDN(QString("%1").arg(i));
            info.setDeviceName("Test");
            info.setDeviceIP(QString("192.169.1.%1").arg(i));
            if(i %2 ==0)
            {
                info.setIsConnected(false);
                info.setIsAllowTrans(false);
                info.setcurState(0);
            }
            else
            {

                info.setIsConnected(true);
                info.setIsAllowTrans(true);
                info.setcurState(4);
            }
            info.setCurProgress(50 * i % 100);



            m_CloudAndWifiList << info;
        }
        endResetModel();
//        WifiControlMng::instance().scanPrinter();
    }
    else if (m_DataType == CLOUD_DATA)
    {
        beginResetModel();
        m_CloudAndWifiList.clear();
        endResetModel();
        
//        PrinterDeviceControl::instance().refreshPrinterListCmd();
    }

}

Q_INVOKABLE void CusWifiManagerModel::showAllTableData()
{
    if (m_DataType == WIFI_DATA)
    {
        //如果没有数据，刷新
        if (m_CloudAndWifiList.isEmpty())
        {
            refreshWifiList();
        }
        else
        {

        }
    }
    else if (m_DataType == CLOUD_DATA)
    {

    }
}

void CusWifiManagerModel::sendFile(QString dn, QString filePath)
{

}

void CusWifiManagerModel::pauseTrans(QString dn)
{

}

void CusWifiManagerModel::stopTrans(QString dn)
{
}

void CusWifiManagerModel::getPrintingStatusCmd(QString dn)
{
    if (m_DataType == WIFI_DATA)
    {

    }
    else if (m_DataType == CLOUD_DATA)
    {
    }
}

void CusWifiManagerModel::pauseGetDeviceTask()
{
    switch (m_DataType) {
    case WIFI_DATA:
    {
        break;
    }
    case CLOUD_DATA:
    {

        break;
    }
    }
}


void CusWifiManagerModel::startPrint(QString dn, QString fileName)
{
    switch (m_DataType) {
    case WIFI_DATA:
    {

        break;
    }
    case CLOUD_DATA:
    {

        break;
    }
    }
}

Q_INVOKABLE void CusWifiManagerModel::pausePrint(QString dn)
{

}

void CusWifiManagerModel::continuePrint(QString dn)
{

}

Q_INVOKABLE void CusWifiManagerModel::stopPrint(QString dn)
{

}

void CusWifiManagerModel::setWifiToken(QString dn, QString token)
{

}

void CusWifiManagerModel::uploadCxdlp(QString filePath)
{

}

QVariantMap CusWifiManagerModel::getSlicerInfo(QString dn)
{

    QVariantMap valueMap;

    return valueMap;
}

bool CusWifiManagerModel::verifySlicer(QString deviceType, QString filePath)
{
    return false;
}

QVariantList CusWifiManagerModel::getOnlineCamera()
{
    QVariantList camList;

    return camList;
}



void CusWifiManagerModel::setParaData(QString dn)
{

}

void CusWifiManagerModel::createConnect(QString strIp)
{
    //调用连接接口



    //模拟返回的情况
    QTimer::singleShot(3000, this, [=]
    {
//        emit sigConnectedIpFailed();
        emit sigConnectedIpSuccessed();


        CloudAndWifiInfo info;
        info.setDeviceDN(QString("%1").arg(m_CloudAndWifiList.count() + 1));
        info.setDeviceName("Test2");
        info.setDeviceIP(QString("192.169.2.%1").arg(m_CloudAndWifiList.count() + 1));

        info.setcurState(0);
        info.setIsConnected(false);
        info.setIsAllowTrans(false);


        addModelData(info);
    });

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

void CusWifiManagerModel::slotPrinterChanged(QVariant printInfo)
{
    if(m_DataType == WIFI_DATA)
    {
#if 1
        //通过信号 一个一个的往表格中 添加数据
//        if (printInfo == "")return;
//        CWifiPrinter* varMap = printInfo.value<CWifiPrinter*>();
//        CloudAndWifiInfo info;
//        info.setDeviceName(varMap->getPrinterName());
//        info.setDeviceIP(varMap->getPrinterIp());
//        info.setHasCamera(varMap->getCameraEnable());
//        info.setCurProgress(varMap->getProgress());
//        info.setTransFileNameDisplay(varMap->getFileDisPlay());
//        info.setTransFilePath(varMap->getFilePath());
//        info.setcurState(varMap->getTaskStatus());
//        info.seteviceDN(varMap->getPrinterDN());
//        info.setInterDeviceName(varMap->getPrinterInteralName());
//        beginInsertRows(QModelIndex(), m_CloudAndWifiList.size(), m_CloudAndWifiList.size());
//        m_CloudAndWifiList.push_back(info);
//        endInsertRows();
#else
        QVariantList wifiList = WifiControlMng::instance().getPrinterList();
        beginResetModel();
        m_CloudAndWifiList.clear();
        for (QVariant data : wifiList)
        {
            CWifiPrinter* varMap = data.value<CWifiPrinter*>();
            CloudAndWifiInfo info;
            info.setDeviceName(varMap->getPrinterName());
            info.setDeviceIP(varMap->getPrinterIp());
            info.setHasCamera(varMap->getCameraEnable());
            info.setCurProgress(varMap->getProgress());
            info.setTransFileNameDisplay(varMap->getFileDisPlay());
            info.setTransFilePath(varMap->getFilePath());
            info.setcurState(varMap->getTaskStatus());
            info.seteviceDN(varMap->getPrinterDN());
            info.setInterDeviceName(varMap->getPrinterInteralName());
            m_CloudAndWifiList << info;
        }
        endResetModel();
#endif
    }
    if(m_DataType == CLOUD_DATA)
    {

    }


}

void CusWifiManagerModel::slotProgressChanged(QString dn, int progress)
{
    refreshProgress(dn, progress);
    //通知界面刷新
    emit sigProgressChanged(dn, progress);
}

void CusWifiManagerModel::slotStatesChanged(QString dn, int state)
{
    refreshCurState(dn, state);

    //通知界面刷新
    emit sigStateChanged(dn, state);
}

void CusWifiManagerModel::slotCameraStatusChanged(QString dn, bool online)
{
    refreshCameraState(dn, online);
}

void CusWifiManagerModel::slotPrintError(QString dn, int errorCode, QString msg)
{
    QString eMsg = erroCodeToString_Print(errorCode);
    //修改当前状态
    refreshCurState(dn, errorCode + 30);
    //creative_kernel::AbstractKernelUI::getSelf()->executeCommand("requestDialogText", this, "messageSingleBtnDlg", eMsg);
}

void CusWifiManagerModel::slotTransError(QString dn, int errorCode, QString msg)
{
//    QString eMsg = erroCodeToString_Trans(errorCode);
//    //修改当前状态
//    refreshCurState(dn, errorCode + 20);

    //creative_kernel::AbstractKernelUI::getSelf()->executeCommand("requestDialogText", this, "messageSingleBtnDlg", eMsg);
}

void CusWifiManagerModel::registerEngine(QQmlApplicationEngine *engine)
{
    m_Engine = engine;
//    m_Engine->rootContext()->setContextProperty("qPrintParam", m_PrintParam);
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

    return roles;
}

bool CusWifiManagerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug() << "CusWifiManagerModel::setData";
    return true;
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

QString CusWifiManagerModel::erroCodeToString_Print(int errorCode)
{
    QString msg;

    return msg;
}

QString CusWifiManagerModel::erroCodeToString_Trans(int errorCode)
{
    QString msg;
    return msg;
}
void CusWifiManagerModel::editName(QString dn, QString name)
{
}
void CusWifiManagerModel::refreshFileName(QString dn, QString filePath)
{

}
//导出的时候，
void CusWifiManagerModel::refreshFileName(QString fileName)
{

}

int CusWifiManagerModel::getProgress(int row)
{
    return m_CloudAndWifiList[row].curProgress();
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
    //if (rowsMac.size() == 0)return rowsMac;
    return rowsMac;
}


void CusWifiManagerModel::refreshSelectStateByRow(QString dn, bool checked)
{
    int rowIndex = getRow(dn);

    if(rowIndex == -1)
        return;
    beginResetModel();
    CloudAndWifiInfo info = m_CloudAndWifiList.at(rowIndex);
    info.setSelcetState(checked);
    m_CloudAndWifiList.replace(rowIndex, info);
    endResetModel();


}

void CusWifiManagerModel::sendFileToPrinters(QString strFilePath)
{
    QList <QString > listPrinters = getSelectPrinterKeyList();
    for(int i = 0; i < listPrinters.count(); i++)
    {
        sendFile(listPrinters[i],strFilePath);
    }

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

CloudAndWifiInfo::CloudAndWifiInfo()
{

}

CloudAndWifiInfo::~CloudAndWifiInfo()
{

}

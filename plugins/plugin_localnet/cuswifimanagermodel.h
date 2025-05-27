#ifndef CUSWIFIMANAGERMODEL_H
#define CUSWIFIMANAGERMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QTimer>
#include <QJsonObject>
//#include "basickernelexport.h"
#include "cuswifimanagermodelData.h"
#include "localnetworkinterface/RemotePrinter.h"
#include <QThreadPool>

class CloudAndWifiInfo
{
public:

    CloudAndWifiInfo();
    ~CloudAndWifiInfo();

    bool selectState() const { return m_bSelectRow; }
    void setSelcetState( bool selectrow)  { m_bSelectRow =selectrow; }

    QString  deviceName() const { return m_DeviceName; }
    void setDeviceName(QString name) { if(m_DeviceName != name) m_DeviceName = name; }

    QString  interDeviceName() const { return m_InternalDeviceName; }
    void setInterDeviceName(QString name) { if (m_InternalDeviceName != name) m_InternalDeviceName = name; }

    QString  deviceIP() const { return m_DeviceIP; }
    void setDeviceIP(QString ip) { if(m_DeviceIP != ip) m_DeviceIP = ip; }

    bool hasCamera() const { return m_HasCamera; }
    void setHasCamera(bool isCamera) { if(m_HasCamera != isCamera) m_HasCamera = isCamera; }

    QString transFilePath() const { return m_TransFilePath; }
    void setTransFilePath(QString filePath) { if(m_TransFilePath != filePath) m_TransFilePath = filePath; }
    //表格中的文件名显示
    QString transFileNameDisplay() const { return m_TransFileNameDisplay; }
    void setTransFileNameDisplay(QString fileDisplay) { if (m_TransFileNameDisplay != fileDisplay) m_TransFileNameDisplay = fileDisplay; }

    bool isAllowTrans() const { return m_IsAllowTrans; }
    void setIsAllowTrans(bool isAllow) { if(m_IsAllowTrans != isAllow) m_IsAllowTrans = isAllow; }

    TransAndPrintState curState() const { return m_CurState; }
    void setcurState(int state) { if(m_CurState != state) m_CurState = (TransAndPrintState)state; }

    int curProgress() const { return m_CurProgress; }
    void setCurProgress(int curProgress) { if(m_CurProgress != curProgress) m_CurProgress = curProgress; }

    QString deviceDN() const { return m_DeviceDN; }
    void setDeviceDN(QString dn) { if(m_DeviceDN != dn) m_DeviceDN = dn; }
    //模型列表
    QVariantList fileList() const { return m_FileList; }
    void setFileList(QVariantList fileList) { m_FileList = fileList; }

    bool isOnline() const { return m_IsOnline; }
    void setIsOnline(bool isOnline) { m_IsOnline = isOnline; }

    bool isConnected() const { return m_IsConnected;}
    void setIsConnected(bool isConnected) { m_IsConnected = isConnected; }

    int devConnectType() const { return m_DevConnectType; }
    void setDevConnectType(int type) { m_DevConnectType = type; }

    int curTransProgress() const{ return m_CurTransProgress;}
    void setCurTransProgress(int nprogress){ m_CurTransProgress = nprogress;}

private:
    bool m_bSelectRow = false;
    QString m_DeviceName = "";                                   //设备名称
    QString m_InternalDeviceName = "";                          //内部设备名称
    QString m_DeviceIP = "";                                    //设备IP地址
    bool m_HasCamera = false;                                   //是否有摄像头
    int m_CurProgress = 0;                                      //当前进度
    int m_CurTransProgress =0;                                  //传输文件进度
    QString m_TransFilePath = "";                               //传输的文件路径
    QString m_TransFileNameDisplay = "";                        //文件名的显示。 //远程导入进来的文件路径是一段md5数字，这个时候需要一个明确的文件名
    TransAndPrintState  m_CurState;                             //当前传输状态
    QString m_DeviceDN = "";                                    //设备的DN号,KEY值
    QVariantList m_FileList; //创想云账号下的模型列表
    bool m_IsOnline = false;
    bool m_IsConnected = false;
    bool m_IsAllowTrans = true;                                        //当前状态是否允许传输文件
    int m_DevConnectType = 1;                                         //1:自动搜索  2:手动连接
};

class QQmlApplicationEngine;

class /*BASIC_KERNEL_API*/ CusWifiManagerModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum WifiInfoRoles {
        CHECK_ROLE  = Qt::UserRole + 1,
        DEVICENAME_ROLE,
        DEVICE_INTERNALNAME_ROLE,
        DEVICEIP_ROLE,
        DEVICEDN_ROLE,
        HASCAMERA_ROLE,
        CURPROGRESS_ROLE,
        TRANSFILENAME_ROLE,
        TRANSFILEPATH_ROLE,
        ISALLOWTRANS_ROLE,
        CURSTATE_ROLE,
        FILELIST_ROLE,
        ISONLINE_ROLE,
        ISCONNECT_ROLE,         //连接状态
        CONNECTTYPE_ROLE,        //连接类型，1:自动搜索  2:手动连接
        TRANSPROGRESS_ROLE       //文件传输进度
    };

    enum CurDataType{
        SEARCH_DATA,  
        MANUAL_DATA,
        CONTROL_DATA
    };

    CusWifiManagerModel();
    //按钮切换数据
    Q_INVOKABLE void changeDataSource(int dataType);
    Q_INVOKABLE void refreshWifiList();
    void sendFile(QString dn, QString filePath);
    //lsg
    Q_INVOKABLE void findDeviceList();
    // 创建连接，用于手动创建连接，加入表数据
    Q_INVOKABLE void createConnect(QString strIp);
    // 重新连接IP
    Q_INVOKABLE void reconnectIp(QString dn);
    //获取被选中的打印机
    QList<QString> getSelectPrinterKeyList();
    //设置打印机列表选中
    Q_INVOKABLE void refreshSelectStateByRow(QString dn, bool checked);
    // 支持一个文件多个打印机传输
    Q_INVOKABLE void sendFileToPrinters(QString strFilePath);

    Q_INVOKABLE QVariantMap getGCodeListData(QString strMac);

    //qml 发送控制命令
    Q_INVOKABLE void sendUIControlCmdToDevice(QString strMac,int nCmdType, QString value);

    void updateModelDataByPrinterStates(RemotePrinter curState, CloudAndWifiInfo& info);


    Q_INVOKABLE void removeTmpFilePath(QString filePath);
    Q_INVOKABLE void resetSomeStatus(QString strMac);
    Q_INVOKABLE bool isConnected(QString strIp);
    void onGetFileList(const std::string&strMac, const std::string& filelist);
    void onFileTransProgress(const std::string& strMac, const float& progress);
    void onFileTransMessage(const std::string& strMac, const std::string& errMsg);
    void onGetInfoSuccess(RemotePrinter curState);
public slots:
    void slotGetInfoSuccess(RemotePrinter curState);
    void slotGetInfoFailed(QString macStr);
    void slotFileTransProgress(QString macStr, float progress);
signals:
    void sigGetInfoSuccess(RemotePrinter curState);
    void sigProgressChanged(QString dn, int progress);
    void sigStateChanged(QString dn, int curState);
    void sigFileTransProgress(QString macStr, float progressVal);
    void sigFileTransMessage(QString macStr, QString errMsg);

    void sigPrinterStateChanged(QString dn, QVariantMap ps);

    //连接ip的 成功的 返回
    void sigConnectedIpSuccessed();
	void sigConnectedIpFailed();

protected:
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const;
private:
    void addModelData(CloudAndWifiInfo info);         //新增表数据
    int getRow(QString dn);    //通过dn号查询行号
    void refreshCameraState(QString dn, int state);
    //打印机进度
    void refreshProgress(QString dn, float progress);
    //0: 不在线 1：在线
    void refreshCurState(QString dn, int state); //更新当前状态
    void refreshIsConnected(int rowIndex, bool isConnected);      //更新是否连接成功
    void refreshIsAllowTrans(QString dn, bool isAllow);     //更新是否允许传输文件
    //文件传输进度
    void refreshTransProgress(QString dn, int progress);

    void refreshItem(int row, int column); //刷新单项数据
    QString secToTime(QString sec);

    QList<CloudAndWifiInfo> m_CloudAndWifiList;
    CurDataType     m_DataType;//当前的数据来源

    bool m_bIsConnectingIp = false;
    QString m_curStrMac = "";
    QString m_curConnectingIp;                          //记录当前正在连接的IP
    QMap<QString, RemotePrinter> m_mapPrinterInfo;      //mac,与数据对应
    QMap<QString, QStringList> m_mapPrinterFileList;      //mac,与数据对应
    bool m_bFirstFind = false;
};

#endif // CUSWIFIMANAGERMODEL_H

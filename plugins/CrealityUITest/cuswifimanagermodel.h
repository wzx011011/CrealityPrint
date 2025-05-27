#ifndef CUSWIFILISTMODEL_H
#define CUSWIFILISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QTimer>
#include <QJsonObject>
//#include "basickernelexport.h"
#include "cuswifimanagermodelData.h"

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
private:
    bool m_bSelectRow = false;
    QString m_DeviceName = "";                                   //设备名称
    QString m_InternalDeviceName = "";                          //内部设备名称
    QString m_DeviceIP = "";                                    //设备IP地址
    bool m_HasCamera = false;                                   //是否有摄像头
    int m_CurProgress = 0;                                      //当前进度
    QString m_TransFilePath = "";                               //传输的文件路径
    QString m_TransFileNameDisplay = "";                        //文件名的显示。 //远程导入进来的文件路径是一段md5数字，这个时候需要一个明确的文件名
    TransAndPrintState  m_CurState;                             //当前传输状态
    QString m_DeviceDN = "";                                    //设备的DN号,KEY值
    QVariantList m_FileList; //创想云账号下的模型列表
    bool m_IsOnline = false;
    bool m_IsConnected = false;
    bool m_IsAllowTrans = true;                                        //当前状态是否允许传输文件
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
        NEEDTIME_ROLE,
        TOTALLAYERS_ROLE,
        LAYERTHICK_ROLE
    };

    enum CurDataType{
        WIFI_DATA,
        CLOUD_DATA
    };

    CusWifiManagerModel();
    //按钮切换数据
    Q_INVOKABLE void changeDataSource(int dataType);
    //刷新整个数据
    Q_INVOKABLE void refreshWifiList(QVariantList dataList);
    Q_INVOKABLE void refreshWifiList();
    //显示表格数据,list无数据就刷新，有数据就保持原来关闭前的，打开轮询就行
    Q_INVOKABLE void showAllTableData();
    //更新文件名
    Q_INVOKABLE void refreshFileName(QString dn, QString fileName);
    Q_INVOKABLE void refreshFileName(QString fileName);
    //更新打印机别名
    Q_INVOKABLE void editName(QString dn, QString name);
    //读取进度
    Q_INVOKABLE int getProgress(int row);
    //更新进度
    Q_INVOKABLE void refreshProgress(QString dn, float progress);
    //发送文件
    Q_INVOKABLE void sendFile(QString dn, QString filePath = QString());
    //暂停发送
    Q_INVOKABLE void pauseTrans(QString dn);
    //停止发送
    Q_INVOKABLE void stopTrans(QString dn);
    //获取打印机实时状态信息
    Q_INVOKABLE void getPrintingStatusCmd(QString dn);
    //暂停轮询功能
    Q_INVOKABLE void pauseGetDeviceTask();
    //
    //开始打印
    Q_INVOKABLE void startPrint(QString dn, QString fileName = QString());
    //暂停打印
    Q_INVOKABLE void pausePrint(QString dn);
    //继续打印
    Q_INVOKABLE void continuePrint(QString dn);
    //停止打印
    Q_INVOKABLE void stopPrint(QString dn);
    //设置token
    Q_INVOKABLE void setWifiToken(QString dn, QString token);
    //创想云上传切片文件
    Q_INVOKABLE void uploadCxdlp(QString filePath);
    //获取切片文件的信息
    Q_INVOKABLE QVariantMap getSlicerInfo(QString dn);
    //校验文件 相同返回true,不同返回false
    Q_INVOKABLE bool verifySlicer(QString deviceType, QString filePath);
    //获取在线的摄像头
    Q_INVOKABLE QVariantList getOnlineCamera();

    Q_INVOKABLE void setParaData(QString dn);

    //lsg
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

public slots:
    //打印机上下线变化
    void slotPrinterChanged(QVariant);
    //进度更新
    void slotProgressChanged(QString dn, int progress);
    //传输和打印状态更新
    void slotStatesChanged(QString dn, int state);
    //摄像头是否在线
    void slotCameraStatusChanged(QString dn, bool online);
    //打印错误
    void slotPrintError(QString dn, int errorCode, QString msg);
    //传输状态
    void slotTransError(QString dn, int errorCode, QString msg);
    //注册engine
    void registerEngine(QQmlApplicationEngine* engine);



signals:
    void sigProgressChanged(QString dn, int progress);
    void sigStateChanged(QString dn, int curState);

    //连接ip的 成功的 返回
    void sigConnectedIpSuccessed();
    void sigConnectedIpFailed();

protected:
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
private:

    void addModelData(CloudAndWifiInfo info);         //新增表数据

    int getRow(QString dn);    //通过dn号查询行号

    QString erroCodeToString_Print(int errorCode);
    QString erroCodeToString_Trans(int errorCode);


    void refreshCameraState(QString dn, int state);//0: 不在线 1：在线
    void refreshCurState(QString dn, int state); //更新当前状态

    void refreshIsConnected(int rowIndex, bool isConnected);      //更新是否连接成功
    void refreshIsAllowTrans(QString dn, bool isAllow);     //更新是否允许传输文件


    void refreshItem(int row, int column); //刷新单项数据
    QString secToTime(QString sec);
    void changeSignalBinding();

    QList<CloudAndWifiInfo> m_CloudAndWifiList;
    QTimer*      m_Timer = nullptr;
    QQmlApplicationEngine*  m_Engine;
    CurDataType     m_DataType;//当前的数据来源

};

#endif // CUSWIFILISTMODEL_H

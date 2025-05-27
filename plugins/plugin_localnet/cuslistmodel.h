#ifndef CUSLISTMODEL_H
#define CUSLISTMODEL_H

#include <QMap>
#include <QTime>
#include <QTimer>
#include <QVector>
#include <QThread>
#include <QObject>
#include <QString>
#include <QLocale>
#include <QSettings>
#include <QMetaEnum>
#include <QDateTime>
#include <QMutexLocker>
#include <QAbstractListModel>
#include <QProcessEnvironment>
#include <QtConcurrent/QtConcurrent>

#include "kernel/abstractkernel.h"
#include "localnetworkinterface/RemotePrinterManager.h"

class ListModelItem;
class CusListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CusListModel(QObject *parent = nullptr);
	~CusListModel();
	
	void onGetInfoSuccess(RemotePrinter curState);
	void onGetFileList(const std::string& std_macAddr, const std::string& std_list);
	void onGetPreviewImg(const std::string& std_macAddr, const std::string& std_imgType, const std::string& std_imgData);

    //刷新设备列表
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void findDeviceList();
	//添加设备
	Q_INVOKABLE void createConnect(const QString& deviceIpAddr);
	//控制命令
	Q_INVOKABLE void sendUIControlCmdToDevice(QString deviceIpAddr, int nCmdType, QString value);

	//复位
	//Q_INVOKABLE void batch_reset();
	//暂停
	Q_INVOKABLE void batch_pause(const QStringList& deviceIpAddrs);
    //一键预热
    Q_INVOKABLE void batch_warmUp(const QStringList& deviceIpAddrs);
	//停止
	Q_INVOKABLE void batch_stop(const QStringList& deviceIpAddrs);
    //开始打印
    Q_INVOKABLE void batch_start(const QStringList& deviceIpAddrs, const QString& filePath = QString());
	//传输文件
	Q_INVOKABLE void importFile(QString macAddress, QString filePath);
	Q_INVOKABLE void deleteFile(QString macAddress, QString filePath);

    //获取分组
    Q_INVOKABLE QStringList getGroups();
    //添加分组
    Q_INVOKABLE void addGroup(const QString& addGroupName);
    //修改组名
    Q_INVOKABLE void editGroupName(int groupIndex, const QString& newGroupName);
	//修改设备分组
	Q_INVOKABLE void editDeviceGroup(const QString& macAddress, int newGroupNo);
	//修改设备名称
	Q_INVOKABLE void editDeviceName(const QString& macAddress, const QString& newDeviceName);

	//获取时间
	Q_INVOKABLE qint64 getCurrentUtcTime();
	Q_INVOKABLE QDateTime getCurrentDateTime(bool start);
    //获取Item
    Q_INVOKABLE ListModelItem* getDeviceItem(const QString& macAddress);

protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int,QByteArray> roleNames() const;

signals:
	void sigConnectedIpSuccessed();
	void sigGetInfoSuccess(RemotePrinter curState);
	void sigGetFileList(const QString& from_std_string, const QString& from_std_list);
	void sigGetPreviewImage(const QString& from_std_macAddr, const QImage& previewImage);

private slots:
	void slotGetInfoSuccess(RemotePrinter curState);
	void slotGetFileList(const QString& from_std_string, const QString& from_std_list);
	void slotGetPreviewImage(const QString& from_std_macAddr, const QImage& previewImage);

private:
	void readConfig();
	void writeConfig();
    //int  getRow(QString macAddr, bool hasMac = true);//通过mac或ip获取index

	void UploadProgressCallBack(const std::string& strMac, const float& progress);
	void UploadMessageCallBack(const std::string& strMac, const std::string& errMsg);//Upload

	void ImportProgressCallBack(const std::string& strMac, const float& progress);
	void ImportMessageCallBack(const std::string& strMac, const std::string& errMsg);//Import

    bool m_bFirstFind = false;
	bool m_bIsConnecting = false;
	ImgProvider* m_previewProvider;

	QRegExp m_infoRegExp;
	QString m_appDataPath;
	QString m_gcodeUploadPath;
	QString m_curConnectingIpAddr;
	QStringList m_deviceGroupNames;

    //QList<ListModelItem*> m_ListPrinterItem;
	QMap<QString, ListModelItem*> m_MapPrinterItem;
	QMap<QString, QPair<int, QString>> m_MapDeviceInfo;
};

class ListModelItem: public QObject
{
    Q_OBJECT

public:
    enum DataType {
        E_PrinterID,
		E_PrinterModel,
        E_PrinterState,
		E_PrinterStatus,
		E_PrinterMethod,
        E_HasCamera,
		E_HasSelect,
        E_DeviceName,
        E_IpAddr,
        E_GCodeName,
        E_TotalPrintTime,
        E_LeftTime,
		E_CurPrintLayer,
		E_TotalPrintLayer,
		E_CurPrintProgress,
        E_PrintSpeed,
        E_BedDstTemp,
        E_NozzleDstTemp,
        E_BedTemp,
        E_NozzleTemp,
        E_GroupNo,
        E_X,
        E_Y,
        E_Z,
        E_FanOpened,
        E_LedOpened,
		E_TransProgress,
		E_ImportProgress,
		E_GCodeFiles,
    };

    Q_PROPERTY(QString pcPrinterID READ pcPrinterID WRITE setPcPrinterID NOTIFY pcPrinterIDChanged)
    Q_PROPERTY(QString pcPrinterModel READ pcPrinterModel WRITE setPcPrinterModel NOTIFY pcPrinterModelChanged)
    Q_PROPERTY(int pcPrinterState READ pcPrinterState WRITE setPcPrinterState NOTIFY pcPrinterStateChanged)
	Q_PROPERTY(int pcPrinterStatus READ pcPrinterStatus WRITE setPcPrinterStatus NOTIFY pcPrinterStatusChanged)
	Q_PROPERTY(int pcPrinterMethod READ pcPrinterMethod WRITE setPcPrinterMethod NOTIFY pcPrinterMethodChanged)
    Q_PROPERTY(int pcHasCamera READ pcHasCamera WRITE setPcHasCamera NOTIFY pcHasCameraChanged)
	Q_PROPERTY(int pcHasSelect READ pcHasSelect WRITE setPcHasSelect NOTIFY pcHasSelectChanged)
    Q_PROPERTY(QString pcDeviceName READ pcDeviceName WRITE setPcDeviceName NOTIFY pcDeviceNameChanged)
    Q_PROPERTY(QString pcIpAddr READ pcIpAddr WRITE setPcIpAddr NOTIFY pcIpAddrChanged)
    Q_PROPERTY(QString pcGCodeName READ pcGCodeName WRITE setPcGCodeName NOTIFY pcGCodeNameChanged)
    Q_PROPERTY(int pcTotalPrintTime READ pcTotalPrintTime WRITE setPcTotalPrintTime NOTIFY pcTotalPrintTimeChanged)
    Q_PROPERTY(int pcLeftTime READ pcLeftTime WRITE setPcLeftTime NOTIFY pcLeftTimeChanged)
	Q_PROPERTY(int pcCurPrintLayer READ pcCurPrintLayer WRITE setPcCurPrintLayer NOTIFY pcCurPrintLayerChanged)
	Q_PROPERTY(int pcTotalPrintLayer READ pcTotalPrintLayer WRITE setPcTotalPrintLayer NOTIFY pcTotalPrintLayerChanged)
	Q_PROPERTY(int pcCurPrintProgress READ pcCurPrintProgress WRITE setPcCurPrintProgress NOTIFY pcCurPrintProgressChanged)
    Q_PROPERTY(int pcPrintSpeed READ pcPrintSpeed WRITE setPcPrintSpeed NOTIFY pcPrintSpeedChanged)
    Q_PROPERTY(int pcBedDstTemp READ pcBedDstTemp WRITE setPcBedDstTemp NOTIFY pcBedDstTempChanged)
    Q_PROPERTY(int pcNozzleDstTemp READ pcNozzleDstTemp WRITE setPcNozzleDstTemp NOTIFY pcNozzleDstTempChanged)
    Q_PROPERTY(int pcBedTemp READ pcBedTemp WRITE setPcBedTemp NOTIFY pcBedTempChanged)
    Q_PROPERTY(int pcNozzleTemp READ pcNozzleTemp WRITE setPcNozzleTemp NOTIFY pcNozzleTempChanged)
    Q_PROPERTY(int pcGroup READ pcGroup WRITE setPcGroup NOTIFY pcGroupChanged)
    Q_PROPERTY(float pcX READ pcX WRITE setPcX NOTIFY pcXChanged)
    Q_PROPERTY(float pcY READ pcY WRITE setPcY NOTIFY pcYChanged)
    Q_PROPERTY(float pcZ READ pcZ WRITE setPcZ NOTIFY pcZChanged)
    Q_PROPERTY(bool fanOpened READ fanOpened WRITE setFanOpened NOTIFY fanOpenedChanged)
    Q_PROPERTY(bool ledOpened READ ledOpened WRITE setLedOpened NOTIFY ledOpenedChanged)
    Q_PROPERTY(float gCodeTransProgress READ gCodeTransProgress WRITE setGCodeTransProgress NOTIFY gCodeTransProgressChanged)
	Q_PROPERTY(float gCodeImportProgress READ gCodeImportProgress WRITE setGCodeImportProgress NOTIFY gCodeImportProgressChanged)
	Q_PROPERTY(QStringList gCodeFileList READ gCodeFileList WRITE setGCodeFileList NOTIFY gCodeFileListChanged)

    explicit ListModelItem(QObject* parent = nullptr);
    ListModelItem(const ListModelItem& listModelItem);
    ~ListModelItem();

    const QString &pcPrinterID() const;
	void setPcPrinterID(const QString& printerID);

    const QString &pcPrinterModel() const; 
	void setPcPrinterModel(const QString& printerModel);

    int pcPrinterState() const;
    void setPcPrinterState(int state);

	int pcPrinterStatus() const;
	void setPcPrinterStatus(int status);

	int pcPrinterMethod() const;
	void setPcPrinterMethod(int method);

    int pcHasCamera() const;
	void setPcHasCamera(int hasCamera);

	int pcHasSelect() const;
	void setPcHasSelect(int hasSelect);

    const QString &pcDeviceName() const;
    void setPcDeviceName(const QString& deviceName);

    const QString &pcIpAddr() const;
	void setPcIpAddr(const QString& ipAddr);

    const QString &pcGCodeName() const;
	void setPcGCodeName(const QString& gcodeName);

    int pcTotalPrintTime() const;
	void setPcTotalPrintTime(int totalPrintTime); 

    int pcLeftTime() const;
	void setPcLeftTime(int leftTime);

	int pcCurPrintLayer() const;
	void setPcCurPrintLayer(int layer);

	int pcTotalPrintLayer() const;
	void setPcTotalPrintLayer(int totalLayer);

    int pcCurPrintProgress() const;
	void setPcCurPrintProgress(int progress);

    int pcPrintSpeed() const;
    void setPcPrintSpeed(int speed);

    int pcBedDstTemp() const;
    void setPcBedDstTemp(int bedDstTemp);

    int pcNozzleDstTemp() const;
    void setPcNozzleDstTemp(int nozzleDstTemp);

    int pcBedTemp() const;
	void setPcBedTemp(int bedTemp);

    int pcNozzleTemp() const;
	void setPcNozzleTemp(int nozzleTemp);

    int pcGroup() const;
    void setPcGroup(int groupNo);

    float pcX() const;
    void setPcX(float newPcX);

    float pcY() const;
    void setPcY(float newPcY);

    float pcZ() const;
    void setPcZ(float newPcZ);

    bool fanOpened() const;
    void setFanOpened(bool fanOpened);

    bool ledOpened() const;
    void setLedOpened(bool ledOpened);

	float gCodeTransProgress() const;
	void setGCodeTransProgress(float progress);

	float gCodeImportProgress() const;
	void setGCodeImportProgress(float progress);

	const QStringList &gCodeFileList() const;
	void setGCodeFileList(const QStringList& fileList);

	friend class CusListModel;

signals:
    void pcPrinterStateChanged();
	void pcPrinterStatusChanged();
	void pcPrinterMethodChanged();
	void pcPrinterModelChanged();
    void pcHasCameraChanged();
	void pcHasSelectChanged();
    void pcDeviceNameChanged();
    void pcIpAddrChanged();
    void pcGCodeNameChanged();
    void pcTotalPrintTimeChanged();
    void pcLeftTimeChanged();
	void pcCurPrintLayerChanged();
	void pcTotalPrintLayerChanged();
	void pcCurPrintProgressChanged();
    void pcPrintSpeedChanged();
    void pcBedDstTempChanged();
    void pcNozzleDstTempChanged();
    void pcBedTempChanged();
    void pcNozzleTempChanged();
    void pcGroupChanged();
    void pcXChanged();
    void pcYChanged();
    void pcZChanged();
    void pcPrinterIDChanged();
    void fanOpenedChanged();
    void ledOpenedChanged();
    void gCodeTransProgressChanged();
	void gCodeImportProgressChanged();
	void gCodeFileListChanged();

private:
	QString m_pcPrinterID;
    QString m_pcPrinterModel;
    int m_pcPrinterState;
	int m_pcPrinterStatus;
	int m_pcPrinterMethod;//0:local 1:lan 2:cloud
    int m_pcHasCamera;
	int m_pcHasSelect;
    QString m_pcDeviceName;
    QString m_pcIpAddr;
    QString m_pcGCodeName;
    int m_pcTotalPrintTime;
    int m_pcLeftTime;
	int m_pcCurPrintLayer;
	int m_pcTotalPrintLayer;
    int m_pcCurPrintProgress;
    int m_pcPrintSpeed;
    int m_pcBedDstTemp;
    int m_pcNozzleDstTemp;
    int m_pcBedTemp;
    int m_pcNozzleTemp;
    int m_pcGroup;
	float m_pcX;
	float m_pcY;
	float m_pcZ;
    bool m_fanOpened;
    bool m_ledOpened;
	float m_gCodeTransProgress;
	float m_gCodeImportProgress;
	QStringList m_gCodeFileList;
};
Q_DECLARE_METATYPE(ListModelItem)


#endif // CUSLISTMODEL_H

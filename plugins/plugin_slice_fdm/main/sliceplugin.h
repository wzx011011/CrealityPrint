#ifndef _NULLSPACE_SLICEPLUGIN_1590155449305_H
#define _NULLSPACE_SLICEPLUGIN_1590155449305_H
#include "creativeinterface.h"
#include <QtCore/QSharedPointer>
#include <QtCore/QUrl>
#include "inout/cxhandlerbase.h"
#include "slice/basicsliceui.h"
#include "us/slicermanager.h"
#include <QStandardPaths> 
#include <QtCore/QProcess>

#include "oss_interface.h"
#include "cxprinter/USBPrinterOutputDeviceManager.h"

class AddCommand;
class SlicePreviewFlow;
class SliceUI;
class ParameterSettingUI;
class PreviewWorker;
class SliceAttain;
class SliceCommand;
class SlicePlugin: public QObject, public CreativeInterface , public CXHandleBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "creative.SlicePlugin")
	Q_INTERFACES(CreativeInterface)
    Q_PROPERTY(bool slicing READ isslice CONSTANT)
    Q_PROPERTY(bool hasSwitch READ hasSwitch CONSTANT)
public:
	SlicePlugin(QObject* parent = nullptr);
	virtual ~SlicePlugin();

    Q_INVOKABLE int sliceMainOp(QString op, int param = 0);
	Q_INVOKABLE void uploadToAliyun(QString file_name);
	Q_INVOKABLE void uploadModelToAliyun(int categoryId, QString groupName, QString groupDesc, bool isShare, bool isOriginal, int modelType, QString license, int isCombine);
	Q_INVOKABLE void uploadModelBtnClick();
	Q_INVOKABLE void uploadModelLocalFileBtnClick();
	Q_INVOKABLE bool outPlatform();
	Q_INVOKABLE void viewMyModel();
	Q_INVOKABLE void viewMyUploads();
	Q_INVOKABLE void cloudPrinting(QString gcodeID);
	Q_INVOKABLE void clusterUploadLocalGcodeToAliyun();
    Q_INVOKABLE QObject* sliceModel();
    Q_INVOKABLE QObject* slicePlugin();

	bool isslice();
    bool hasSwitch();

    void localSaveTmpGcode();
	QString getTmpFilePeth();
    QString getCanWriteFolder(){ return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation); }
	bool copyFileToPath(QString sourceDir, QString toDir, bool deleteFileIfExist = true);
signals:
    void sigSliceSuccess(SliceAttain* attai);
    void sigSendGcodeInfo(QString gCodePath);
    void sigSendModelName(QString modelName);
	
public slots:
	void onSliceSuccess(SliceAttain* attain);
	void onSliceFailed();
	void visibleSliceUI(SlicerManager::SLICERTYPE type);
    void onExporSuccess();
	//lisugui
	void onSaveSuccess();
	void onProcessStateChanged(QProcess::ProcessState state);
protected:
	QString name() override;
	QString info() override;

	void handle(const QString& fileName) override;
	void handle(const QStringList& fileNames) override;
	void initialize() override;
	void uninitialize() override;

private: 
	QString getFileMd5(const QString& filepath);
	void uploadProgress(long long upload_size, long long total_size);
	void uploadResult(const oss_wrapper::OSSError& error);
	void uploadModelProgress(long long upload_size, long long total_size);
	void uploadModelResult(const oss_wrapper::OSSError& error);
	void getCategoryListResult(const QByteArray& response);
	void uploadGCodeResult(const std::string& response);
	void modelGroupCreateResult(const std::string& response);
	void uploadModelGroupSingleProgress(long long upload_size, long long total_size);
	void uploadModelGroupSingleResult(const oss_wrapper::OSSError& error);
	void uploadGcodeFiles(const QStringList& fileNames);
	void uploadClusterGcodeFilesProgress(long long upload_size, long long total_size);
	void uploadClusterGcodeFilesResult(const oss_wrapper::OSSError& error);

	//
	void getAliyunOssInfomation(int type);
	void getAliyunOssKey();
	void funcAliyunOssKeyResult(const QByteArray& response);
	void getAliyunOssBucket();
	void funcAliyunOssBucketResult(const QByteArray& response);
	void uploadModelToAliyunOss();

	void uploadGcodeToAliyunFromSofeware();
	void uploadLocalSingleGcodeToAliyun();

	QString m_accessKeyId = "";
	QString m_secretAccessKey = "";
	QString m_sessionToken = "";
	QString m_endPoint = "";
	QString m_fileBucket = "";
	QString m_prefixPath = "";
	QString m_internalBucket = "";
	QString m_internalPrefixPath = "";
	int getAliyunOssInfoCtn = 0;
	int getAliyunOssInfoType = 0; //1: upload model; 2: upload gcode from sofeware; 3: local single file ; 4 :
	int m_uploadModelType = 0;
	QString m_uploadGcodeName = "";
	QStringList m_uploadClusterFileNameList;


protected:
	bool m_bSlicing;
	BasicSliceUI* m_sliceUI;	
	
	SlicePreviewFlow* m_slicePreviewFlow;
	PreviewWorker* m_previewWorker;
	QString m_filePath;
	bool m_isOnlinePrint;

	double m_sliceStartTime;
	QString m_strTmpFilePath = "";
	bool isUSBonline;
	bool isRunning;
signals:
	void positionChanged();
    void sigUpdateProgress(int prog);
	void sigUploadSuccess(QString responseStr);
private:
	SliceCommand* m_command;
	QProcess* process;

	QString m_fileKey;
	QString m_fileName;
	QString m_modelFileName;
	int m_categoryId;
	QString m_groupName;
	QString m_groupDesc;
	QString m_isShare;
	QString m_isOriginal;
	int m_modelType;
	QString m_license;

	QStringList modelNameList;
	QStringList modelPathList;
	QStringList modelFilekeyList;
	int uploadGroupCount;
	int uploadGroupCountTmp;
	int m_saveGCodeFlag; // 1: export; 2 : updateGCode
	bool isUploadModelFile;
	bool isClusterUploadGcode;
	int clusterUploadGcodeTmpCtn;
	int clusterUploadGcodeCtn;
	QStringList clusterUploadGcodeNameList;
	QStringList clusterUploadGcodeFilekeyList;
	
	USBPrinterOutputDeviceManager* m_usbPrinterModel = nullptr;
	time_t m_printingTime = 0;
};
#endif // _NULLSPACE_SLICEPLUGIN_1590155449305_H

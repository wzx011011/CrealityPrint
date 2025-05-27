#ifndef _CREALITYCLOUDUI_15916733167626_H
#define _CREALITYCLOUDUI_15916733167626_H
#include <QtCore/QObject>
#include <vector>
#include <QThreadPool>
#include "qrcodeimageprovider.h"
#include "cameraServer.h"

class QueryEquipmentInfoTask;
class QueryGroupPrintDeviceInfoTask;

class CrealityCloudUI: public QObject
{
	Q_OBJECT
public:
	CrealityCloudUI();
    virtual ~CrealityCloudUI();

	void initCxNetWort();
	void setqmlUI(QObject* obj);
	void loginSuccess(QString token, QString userid);
	void loginFailed(QString errorMsg);


	void setDeviceStatus(const std::string& status, const std::string& id);
	void setDeviceInformation(const std::string& date, const std::string& id, bool isNewIOT = false);
	void setDeviceInformationRealTime(const std::string& date, const std::string& id);
	void setDeviceInformationPosition(const std::string& date, const std::string& id);
	void setClusterDeviceList(std::string data);
	void setClusterTaskList(std::string data);


protected:

public slots://qml data
	void slotLanguageChanged();
	void onLogin();
	void onLogout();
	void loginUpdate();
	void onLoginDlgClose();
	void onLoginByAccont(QString accont, QString password, QString logintype, bool autoLogin, QString areaCode);
	void onQucikLoginByAccont(QString phoneNumber, QString phoneAreaCode, QString verifyCode, bool autoLogin);
	void onGetVerificationCode(QString phoneNumber);
	void onSigAutoLogin(QString token, QString userId);
	void onShowGcodeUpload(QString type);
	void onShowModelUpload(QString type);
	void onGetGCodeList(int page, int pageSize);
	void onDeleteGCode(QString id);
	void onGetModelList(QString page, int pageSize);
	void onDeleteModel(QString id);
	void onImportModel(QString id, QString name, int modelType); //1: model library; 2: my model
	void onImportGCode(QString id, QString name, QString loadLink);
	void onGetCategoryList(int type);
	void onGetModelLibraryList(QString id, QString page, int size);
	void onGetModelGroupDetail(QString id, int count);
	void onGetModelGroupInfomation(QString id);
	void onSearchModelList(QString key, int page, int size);
	void onImportDetailItem(QString id, QString name, QString link);
	void onShareModel(QString id);
	void onGetFavoritedList(QString page, int pageSize);
	void onCancelFavoriteModel(QString id);
	void onFavoriteModel(QString id, bool status);
	void onSigDownLoadModelType(QString type);
	void onSigDownLoadModelToLocall(QString id, int count, QString name, QString path);
	void onGetPrinterList(int page, int pageSize);
	void onGetGCodeListPrintControl(int page, int pageSize, QString type);
	void onSigStartPrintGcode(QString url, QString productName, QString iotId, QString productKey, QString gcodeId, QString printerName, QString printerId, QString tbid, int type);
	void onSigControlPrintGcode(QString productName, QString iotId, QString productKey, QString control, QString tbid, int type);
	void onSigControlPrintTempSpeed(QString productName, QString iotId, QString productKey, QString info, int value, QString tbid, int type);
	void onSigUpdatePreviewImage(QString printID, QString deviceID);
	void onSigGetPrintingGcodeInfo(QString printID);
	void onInitP2PPipeline(QString strApiLicense, QString strDid, QString strInit, int portNum, QString productName);
	void restartP2PPipeline(QString strdid);
	void onPauseGetDeviceTask(QString str); 
	void onSigInitCameraLicense(QString productName, QString iotId, QString productKey, QString did, QString license, QString tbid, int type);
	void onSigUpdateDeviceData(QString productName, QString iotId, QString productKey, QString tbid, int type);
	void onGetCloudSliceList(int page, int pageSize);
	void onSigPersonalSpaceTutorial(QString type);
	void onGetRecommendModelAlwaysShowDlg();
	void onShowModelLibraryDlg(bool isMoreBtn, QString id, QString name, int count, QString author, QString avtar, QString ctime);
	void onGetPurchasedList(QString page, int pageSize);
	void onWizardClicked();
	void onUserInfoDlgClicked();
	void onServerTypeChanged(int serverType);
	void onInitAdvanceParam();
	//group
	void onGetGroupPrintDeviceList();
	void onGetClusterGcodeList(int page, int pageSize);
	void onAddClusterTask(QString ids, QString info);
	void onClusterConformPrinterStatus(QString name, QString status);
	void onClusterUploadGcode();
	void onSigSelectPrintControl(QString type);
	void onSigChangeClusterTaskTime(QString time);
	void onClusterGcodeSliceTypeList();
	void onClsterResetGcodeSliceType(QString gcodeId, QString sliceId);

private:
	void funcLoginSuccess(const std::string& response); 
	void funcQrLogin(const std::string& identical, const time_t& expireTime);
	void onLoginCallback(const int& state, const std::string& token, const std::string& useId);
	void funcLoginByAccont(const std::string& response);
	void funcQucikLoginByAccont(const std::string& response);
	void funcGetVerificationCode(const std::string& response);
	void funcGetGCodeListResult(const std::string& response, int page);
	void funcDeleteGCodeResult(const std::string& response, QString deleteGcodeId);
	void funcGetModelListResult(const std::string& response, QString getModelListPage);
	void funcDeleteModelResult(const std::string& response, QString deleteModelId);
	void funcImportModelResult(int state, int type, std::string& extractPath, bool needImport);
	void funcImportMyModelResult(int state, std::string& filename, std::string& downloadPath);
	void funcDownLoadGCodeResult(const QByteArray& response);
	void funcGetCategoryList(const std::string& response);
	void funcGetRecommendModelLibraryListResult(const std::string& response, QString getModelLibraryListPage);
	void funcGetModelLibraryListResult(const std::string& response, QString getModelLibraryListPage);
	void funcGetModelGroupDetailResult(const std::string& response,int type=1, bool needDownload=false, const std::string& downloadPath="", bool needImport=false);
	void funcGetModelGroupInfomationResult(const std::string& response);
	void funcSearchModelListResult(const std::string& response, int page);
	void funcImportDetailItemResult(int state, std::string fileName);
	void funcGetFavoritedListResult(const std::string& response, const QString& getFavoritedListPage);
	void funcCancelFavoriteModelResult(const std::string& response, int favoriteModelType, QString cancelFavoriteModelId);
	void funcGetPrinterTypeListResult(const std::string& response);
	void funcGetOnwerDeviceListResult(const std::string& response);
	void funcGetGCodeListPrintControlResult(const QByteArray& response);
	void funcGetStartPrintGcodePrintIdResult(const int& state, const std::string& startPrintGcodePrintId, const QString& startPrintGcodeUrl, const QString& startPrintGcodeTbId);
	void funcSetDeviceDesiredProperty_NewIOT(const std::string& response);
	void funcGetAliyunInfoStartPrintGcodeResult(const QByteArray& response);
	void funcSetDeviceDesiredPropertyStart(const QByteArray& response);
	void funcSetDeviceDesiredProperty_NewIOTControlPrintGcode(const std::string& response);
	void funcGetAliyunInfoControlPrintGcodeResult(const QByteArray& response);
	void funcSetDeviceDesiredProperty(const QByteArray& response);
	void funcSetDeviceDesiredProperty_NewIOTControlPrintTempSpeed(const std::string& response);
	void funcGetAliyunInfoControlPrintTempSpeedResult(const QByteArray& response);
	void funcSetDeviceDesiredPropertyTempSpeedResult(const QByteArray& response);
	void funcGetUpdatePreviewImage(const QByteArray& response);
	void funcGetPrintingGcodeInfoResult(const QByteArray& response);
	void funcGetAliyunInfoInitCameraLicenseResult(const QByteArray& response);
	void funcInitCameraLicenseResult(const QByteArray& response);
	void funcGetAliyunInfoUpdateDeviceData(const QByteArray& response);
	void funcUpdateDeviceDataResult(const QByteArray& response);
	void funcUpdateDeviceDataNewIotResult(const std::string& response);
	void funcGetCloudSliceListResult(const QByteArray& response);
	void funcGetRecommendModelAlwaysShowDlgResult(const std::string& response);
	void funcGetPurchasedListResult(const QByteArray& response);
	void funconGetClusterGcodeListResult(const std::string& response, const int& page);
	void funcAddClusterTaskResult(const bool& response);
	void funcClusterConformIdleResult(const std::string& response);
	void funcClusterCancelPrintTaskResult(const std::string& response);
	void funcClusterTaskReportResult(const std::string& response);
	void funcClusterGcodeSliceTypeListResult(const std::string& response);
	void funcClsterResetGcodeSliceTypeResult(const QByteArray& response);
	void funcGetServerRequestOverseas(const QByteArray& response);
	void funcGetServerRequestChina(const QByteArray& response);

	void importModel(QString id, QString name, int modelType, QString downModelToLocallPath = ""); //1: model library; 2: my model
	void getModelGroupDetail(QString id, int count, int type=1, bool needDownload = false, const std::string& downloadPath = "", const bool& needImport = false);
	void importDetailItem(QString id, QString name, QString link, QString downModelToLocallPath = "");
private:
    void getAliyunInfoStartPrintGcode();
	void getAliyunInfoControlPrintGcode();
	QString getSetDeviceDesiredPropertyUrl(QString accessKeyId, QString sessionToken, QString secretAccessKey, QString productName, QString productKey, QString iotId, QString info, int value = 0);
	void getAliyunInfoControlPrintTempSpeed();
	void getAliyunInfoInitCameraLicense();
	void getAliyunInfoUpdateDeviceData();
	void getServerRequestOverseas();
	void getServerRequestChina();

	QString hmacSha1(QByteArray key, QByteArray baseString);
	QString currentTimeToUTC();
	int randTime();
	QString percentEncodeUrl(QString value);
	QString percentEncodeToken(QString value);
	QString generateQueryString(QMap<QString, QString> parameter, bool isEncodeKV);
	QString generateSignString(QString method, QMap<QString, QString> parameter);
	QString generate(QString method, QMap<QString, QString> parameter, QString accessKeySecret);

	void modelLibraryDownLoadSuccess();
	void modelLibraryDownLoadFailed();
	void myModelDownloadSuccess();
	void myModelDownloadFailed();

private:
    QObject* m_qmlUI;
	QrCodeImageProvider* m_imgProvider;
	QString importGcodeId = "";
	QString importGcodeName = "";
	bool importGcodeStatus = false;
	QString importDetailItemUrl = "";
	int downDetailItemType = 0;
	QString startPrintGcodeUrl = "";
	QString startPrintGcodeProductName = "";
	QString startPrintGcodeProductKey = "";
	QString startPrintGcodeIotId = "";
	QString startPrintGcodePrintId = "";
	int startPrintGcodeType = 0;
	bool startPrintGcodeStatus = false;
	QString controlPrintGcodeProductName = "";
	QString controlPrintGcodeProductKey = "";
	QString controlPrintGcodeIotId = "";
	QString controlPrintGcodeInfo = "";
	QString controlPrintTempSpeedProductName = "";
	QString controlPrintTempSpeedProductKey = "";
	QString controlPrintTempSpeedIotId = "";
	QString controlPrintTempSpeedInfo = "";
	int controlPrintTempSpeedValue = 0;
	bool controlPrintTempSpeedStatus = false;
	QString updatePreviewImageDeviceId = "";
	bool updatePreviewImageStatus = false;
	QString initCameraLicenseProductName = "";
	QString initCameraLicenseProduceKey = "";
	QString initCameraLicenseIotId = "";
	QString initCameraLicenseDid = "";
	QString initCameraLicenseLicense = "";
	bool initCameraLicenseStatus = false;
	QString updateDeviceDataProductName = "";
	QString updateDeviceDataProductKey = "";
	QString updateDeviceDataIotId = "";
	int getCloudSliceListPage = 0;
	bool getCloudSliceListStatus = false;
	QString getPurchasedListPage = "";
	bool getPurchasedListStatus = false;

	bool m_isAutoLogin = false;


	QueryEquipmentInfoTask* m_queryEquipmentTask = nullptr;
	QThreadPool* m_threadPool;
	CameraServer* m_pCameraServer;
	std::vector<CameraServer*> m_vecCameraServer;
};


#endif // _CREALITYCLOUDUI_15916733167626_H

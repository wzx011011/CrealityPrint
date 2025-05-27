#include "crealitycloudui.h"
#include <QDebug>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlProperty>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtCore/QSettings>
#include <QClipboard>
#include <QApplication>
#include <QMimeData>
#include <QDesktopServices>
//#include <QStandardPaths>
#include "kernel/translator.h"
#include "us/slicermanager.h"
#include "kernel/abstractkernel.h"
#include "crealitycloud/cxnetworkmanager.h"
#include "interface/gadatainterface.h"
#include "quazip/JlCompress.h"
#include "qtusercore/module/systemutil.h"
#include "interface/modelinterface.h"
#include "inout/cxopenandsavefilemanager.h"
#include "queryequipmentinfotask.h"

using namespace creative_kernel;

//iot sign begin
QString CrealityCloudUI::hmacSha1(QByteArray key, QByteArray baseString)
{
	int blockSize = 64; // HMAC-SHA-1 block size, defined in SHA-1 standard
	if (key.length() > blockSize) { // if key is longer than block size (64), reduce key length with SHA-1 compression
		key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
	}
	QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char"6"
	QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char"/"
	// ascii characters 0x36 ("6") and 0x5c ("/") are selected because they have large
	// Hamming distance (http://en.wikipedia.org/wiki/Hamming_distance)
	for (int i = 0; i < key.length(); i++) {
		innerPadding[i] = innerPadding[i] ^ key.at(i); // XOR operation between every byte in key and innerpadding, of key length
		outerPadding[i] = outerPadding[i] ^ key.at(i); // XOR operation between every byte in key and outerpadding, of key length
	}
	// result = hash ( outerPadding CONCAT hash ( innerPadding CONCAT baseString ) ).toBase64
	QByteArray total = outerPadding;
	QByteArray part = innerPadding;
	part.append(baseString);
	total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));
	QByteArray hashed = QCryptographicHash::hash(total, QCryptographicHash::Sha1);
	return hashed.toBase64();
}

QString CrealityCloudUI::currentTimeToUTC()
{
	QDateTime time = QDateTime::currentDateTime().toUTC();
	return time.toString("yyyy-MM-ddThh:mm:ssZ");
}

int CrealityCloudUI::randTime()
{
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
	return qrand();
}

QString CrealityCloudUI::percentEncodeUrl(QString value)
{
	return value.toUtf8().replace("&", "%26")
		.replace("=", "%3D")
		.replace(":", "%3A")
		.replace("/", "%2F")
		.replace("+", "%2B")
		.replace("*", "%2A")
		.replace("{", "%7B")
		.replace("}", "%7D")
		.replace("\"", "%22")
		.replace(",", "%2C")
		.replace(" ", "%20");
}

QString CrealityCloudUI::percentEncodeToken(QString value)
{
	return value.toUtf8().replace("%", "%25");
}


QString CrealityCloudUI::generateQueryString(QMap<QString, QString> parameter, bool isEncodeKV)
{
	QMap<QString, QString>::iterator iter = parameter.begin();
	QString tmpStr = "";
	while (iter != parameter.end())
	{
		if (isEncodeKV)
		{
			tmpStr.append(percentEncodeUrl(iter.key())).append("=").append(percentEncodeUrl(iter.value())).append("&");
		}
		else {
			tmpStr.append(iter.key()).append("=").append(iter.value()).append("&");
		}

		iter++;

	}

	tmpStr = tmpStr.left(tmpStr.length() - 1);

	return tmpStr;
}

QString CrealityCloudUI::generateSignString(QString method, QMap<QString, QString> parameter)
{
	if (method == nullptr)
	{
		return "";
	}
	QString queryStr = generateQueryString(parameter, true);
	QString stringTosign = "";
	stringTosign.append(method).append("&");
	stringTosign.append(percentEncodeUrl("/")).append("&");
	stringTosign.append(percentEncodeUrl(queryStr));

	return stringTosign;
}

QString CrealityCloudUI::generate(QString method, QMap<QString, QString> parameter, QString accessKeySecret)
{
	QString signString = generateSignString(method, parameter);
	return hmacSha1((accessKeySecret + "&").toUtf8(), signString.toUtf8());
}
//end


void CrealityCloudUI::modelLibraryDownLoadSuccess()
{
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	QObject* pModelDlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
	QMetaObject::invokeMethod(pModelDlg, "modelLibraryDownLoadSuccess");
}

void CrealityCloudUI::modelLibraryDownLoadFailed()
{
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	QObject* pModelDlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
	QMetaObject::invokeMethod(pModelDlg, "modelLibraryDownLoadFailed");
}

void CrealityCloudUI::myModelDownloadSuccess()
{
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
	QMetaObject::invokeMethod(pUserInfoDlg, "myModelDownloadSuccess");
}

void CrealityCloudUI::myModelDownloadFailed()
{
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
	QMetaObject::invokeMethod(pUserInfoDlg, "myModelDownloadFailed");
}


CrealityCloudUI::CrealityCloudUI()
	:m_qmlUI(nullptr)
{
	m_threadPool = new QThreadPool();
	m_threadPool->setMaxThreadCount(36);
	disconnect(Translator::getInstance(), SIGNAL(languageChanged()), this, SLOT(slotLanguageChanged()));
	connect(Translator::getInstance(), SIGNAL(languageChanged()), this, SLOT(slotLanguageChanged()));
}

CrealityCloudUI::~CrealityCloudUI()
{
}

void CrealityCloudUI::initCxNetWort()
{ 
	creative_kernel::getcxNetworkManager()->init();
	creative_kernel::getcxNetworkManager()->setThreadPool(m_threadPool);
}

void CrealityCloudUI::setqmlUI(QObject* obj)
{
	m_qmlUI = obj;

	//QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");


	//login dialog
	QQmlEngine* engine;
	QObject* plogindlg = pTopBar->findChild<QObject*>("LoginDlg");

	if (plogindlg)
	{
		QObject::connect(pTopBar, SIGNAL(login()), this, SLOT(onLogin()));
		QObject::connect(pTopBar, SIGNAL(logout()), this, SLOT(onLogout()));
		QObject::connect(pTopBar, SIGNAL(initP2PPipelie(QString, QString, QString, int, QString)), this, SLOT(onInitP2PPipeline(QString, QString, QString, int, QString)));
		QObject::connect(pTopBar, SIGNAL(dlgClose()), this, SLOT(onLoginDlgClose()));
		QObject::connect(pTopBar, SIGNAL(loginByAccont(QString, QString, QString, bool, QString)), this, SLOT(onLoginByAccont(QString, QString, QString, bool, QString)));
		QObject::connect(pTopBar, SIGNAL(quickloginByAccont(QString, QString, QString, bool)), this, SLOT(onQucikLoginByAccont(QString, QString, QString, bool)));
		QObject::connect(pTopBar, SIGNAL(qrcodeImgUpdate()), this, SLOT(loginUpdate()));
		QObject::connect(pTopBar, SIGNAL(getVerificationCode(QString)), this, SLOT(onGetVerificationCode(QString)));
		QObject::connect(pTopBar, SIGNAL(sigAutoLogin(QString, QString)), this, SLOT(onSigAutoLogin(QString, QString)));
		QObject::connect(pTopBar, SIGNAL(showGcodeUpload(QString)), this, SLOT(onShowGcodeUpload(QString)));
		QObject::connect(pTopBar, SIGNAL(showModelUpload(QString)), this, SLOT(onShowModelUpload(QString)));
		QObject::connect(pTopBar, SIGNAL(getGCodeList(int, int)), this, SLOT(onGetGCodeList(int, int)));
		QObject::connect(pTopBar, SIGNAL(deleteGCode(QString)), this, SLOT(onDeleteGCode(QString)));
		QObject::connect(pTopBar, SIGNAL(getModelList(QString, int)), this, SLOT(onGetModelList(QString, int)));
		QObject::connect(pTopBar, SIGNAL(deleteModel(QString)), this, SLOT(onDeleteModel(QString)));
		QObject::connect(pTopBar, SIGNAL(importModel(QString, QString, int)), this, SLOT(onImportModel(QString, QString, int)));
		QObject::connect(pTopBar, SIGNAL(importGCode(QString, QString, QString)), this, SLOT(onImportGCode(QString, QString, QString)));
		QObject::connect(pTopBar, SIGNAL(shareModel(QString)), this, SLOT(onShareModel(QString)));
		QObject::connect(pTopBar, SIGNAL(getFavoritedList(QString, int)), this, SLOT(onGetFavoritedList(QString, int)));
		QObject::connect(pTopBar, SIGNAL(cancelFavoriteModel(QString)), this, SLOT(onCancelFavoriteModel(QString)));
		QObject::connect(pTopBar, SIGNAL(sigDownLoadModelType(QString)), this, SLOT(onSigDownLoadModelType(QString)));
		QObject::connect(pTopBar, SIGNAL(sigDownLoadModelToLocall(QString, int, QString, QString)), this, SLOT(onSigDownLoadModelToLocall(QString, int, QString, QString)));
		QObject::connect(pTopBar, SIGNAL(getPrinterList(int, int)), this, SLOT(onGetPrinterList(int, int)));
		QObject::connect(pTopBar, SIGNAL(getGCodeListPrintControl(int, int, QString)), this, SLOT(onGetGCodeListPrintControl(int, int, QString)));
		QObject::connect(pTopBar, SIGNAL(pauseGetDeviceTask(QString)), this, SLOT(onPauseGetDeviceTask(QString)));
		QObject::connect(pTopBar, SIGNAL(sigInitCameraLicense(QString, QString, QString, QString, QString, QString, int)), this, SLOT(onSigInitCameraLicense(QString, QString, QString, QString, QString, QString, int)));
		QObject::connect(pTopBar, SIGNAL(sigUpdateDeviceData(QString, QString, QString, QString, int)), this, SLOT(onSigUpdateDeviceData(QString, QString, QString, QString, int)));
		QObject::connect(pTopBar, SIGNAL(getCloudSliceList(int, int)), this, SLOT(onGetCloudSliceList(int, int)));
		QObject::connect(pTopBar, SIGNAL(sigPersonalSpaceTutorial(QString)), this, SLOT(onSigPersonalSpaceTutorial(QString)));
		QObject::connect(pTopBar, SIGNAL(getPurchasedList(QString, int)), this, SLOT(onGetPurchasedList(QString, int)));
		QObject::connect(pTopBar, SIGNAL(wizardClicked()), this, SLOT(onWizardClicked()));
		QObject::connect(pTopBar, SIGNAL(getGroupPrintDeviceList()), this, SLOT(onGetGroupPrintDeviceList()));
		QObject::connect(pTopBar, SIGNAL(getClusterGcodeList(int, int)), this, SLOT(onGetClusterGcodeList(int, int)));
		QObject::connect(pTopBar, SIGNAL(addClusterTask(QString, QString)), this, SLOT(onAddClusterTask(QString, QString)));
		QObject::connect(pTopBar, SIGNAL(clusterConformPrinterStatus(QString, QString)), this, SLOT(onClusterConformPrinterStatus(QString, QString)));
		QObject::connect(pTopBar, SIGNAL(clusterUploadGcode()), this, SLOT(onClusterUploadGcode()));
		QObject::connect(pTopBar, SIGNAL(sigSelectPrintControl(QString)), this, SLOT(onSigSelectPrintControl(QString)));
		QObject::connect(pTopBar, SIGNAL(sigUserInfoDlgClicked()), this, SLOT(onUserInfoDlgClicked()));
		QObject::connect(pTopBar, SIGNAL(sigChangeClusterTaskTime(QString)), this, SLOT(onSigChangeClusterTaskTime(QString)));
		QObject::connect(pTopBar, SIGNAL(clusterGcodeSliceTypeList()), this, SLOT(onClusterGcodeSliceTypeList()));
		QObject::connect(pTopBar, SIGNAL(clsterResetGcodeSliceType(QString, QString)), this, SLOT(onClsterResetGcodeSliceType(QString, QString)));
		QObject::connect(pTopBar, SIGNAL(changeServer(int)), this, SLOT(onServerTypeChanged(int)));
		QObject::connect(plogindlg, SIGNAL(serverTypeChanged(int)), this, SLOT(onServerTypeChanged(int)));


		getServerRequestOverseas();
		getServerRequestChina();

	}

	QObject* pModeldlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
	if (pModeldlg)
	{
		QObject::connect(pTopBar, SIGNAL(getCategoryList(int)), this, SLOT(onGetCategoryList(int)));
		QObject::connect(pTopBar, SIGNAL(getModelLibraryList(QString, QString, int)), this, SLOT(onGetModelLibraryList(QString, QString, int)));
		QObject::connect(pTopBar, SIGNAL(getModelGroupDetail(QString, int)), this, SLOT(onGetModelGroupDetail(QString, int)), Qt::QueuedConnection);
		QObject::connect(pTopBar, SIGNAL(getModelGroupInfomation(QString)), this, SLOT(onGetModelGroupInfomation(QString)), Qt::QueuedConnection);
		QObject::connect(pTopBar, SIGNAL(searchModelList(QString, int, int)), this, SLOT(onSearchModelList(QString, int, int)));
		QObject::connect(pTopBar, SIGNAL(importDetailItem(QString, QString, QString)), this, SLOT(onImportDetailItem(QString, QString, QString)));
		QObject::connect(pTopBar, SIGNAL(favoriteModel(QString, bool)), this, SLOT(onFavoriteModel(QString, bool)));
	}

	QObject* pPrintControldlg = pTopBar->findChild<QObject*>("PrintControlInfoDlg");
	if (pPrintControldlg)
	{
		QObject::connect(pTopBar, SIGNAL(sigStartPrintGcode(QString, QString, QString, QString, QString, QString, QString, QString, int)), this, SLOT(onSigStartPrintGcode(QString, QString, QString, QString, QString, QString, QString, QString, int)));
		QObject::connect(pTopBar, SIGNAL(sigControlPrintGcode(QString, QString, QString, QString, QString, int)), this, SLOT(onSigControlPrintGcode(QString, QString, QString, QString, QString, int)));
		QObject::connect(pTopBar, SIGNAL(sigControlPrintTempSpeed(QString, QString, QString, QString, int, QString, int)), this, SLOT(onSigControlPrintTempSpeed(QString, QString, QString, QString, int, QString, int)));
		QObject::connect(pTopBar, SIGNAL(sigUpdatePreviewImage(QString, QString)), this, SLOT(onSigUpdatePreviewImage(QString, QString)));
		QObject::connect(pTopBar, SIGNAL(sigGetPrintingGcodeInfo(QString)), this, SLOT(onSigGetPrintingGcodeInfo(QString)));

	}

	QObject* mainObj = AbstractKernelUI::getSelf()->getUI("UIAppWindow");
	if (mainObj)
	{
		QObject::connect(mainObj, SIGNAL(initAdvance()), this, SLOT(onInitAdvanceParam()));
		QObject::connect(mainObj, SIGNAL(getRecommendModel()), this, SLOT(onGetRecommendModelAlwaysShowDlg()));
		QObject::connect(mainObj, SIGNAL(showModelLibraryDlg(bool, QString, QString, int, QString, QString, QString)), this, SLOT(onShowModelLibraryDlg(bool, QString, QString, int, QString, QString, QString)), Qt::QueuedConnection);
	}

	m_imgProvider = new QrCodeImageProvider();
	engine = qmlEngine(plogindlg);
	engine->addImageProvider(QLatin1String("qrcodeImgProvider"), m_imgProvider);
	m_imgProvider->setProviderUrl("image://qrcodeImgProvider/");
	m_imgProvider->setparameterUI(this);
}

void CrealityCloudUI::loginSuccess(QString token, QString userid)
{
	qDebug() << "loginSuccess";
	creative_kernel::getcxNetworkManager()->setloginInfo(0, token, userid);
	creative_kernel::getcxNetworkManager()->getInfo(std::bind(
		&CrealityCloudUI::funcLoginSuccess, this, std::placeholders::_1));
}

void CrealityCloudUI::loginFailed(QString errorMsg)
{
	qDebug() << "loginFailed msg " << errorMsg;
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	QMetaObject::invokeMethod(pTopBar, "loginFailed", Q_ARG(QVariant, QVariant::fromValue(errorMsg)));
}

void CrealityCloudUI::setDeviceStatus(const std::string& status, const std::string& id)
{
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
	QMetaObject::invokeMethod(pUserInfoDlg, "updateDeviceStatus", Q_ARG(QVariant, QVariant::fromValue(QString(status.c_str()))), Q_ARG(QVariant, QVariant::fromValue(QString(id.c_str()))));
}

void CrealityCloudUI::setDeviceInformation(const std::string& date, const std::string& id, bool isNewIOT)
{
	QString data = QString(date.c_str());
	QString deviceId = QString(id.c_str());
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");

	if (isNewIOT)
	{
		QMetaObject::invokeMethod(pUserInfoDlg, "updateDeviceInformation_NewIOT", Q_ARG(QVariant, QVariant::fromValue(data)), Q_ARG(QVariant, QVariant::fromValue(deviceId)));
	}
	else
	{
		QMetaObject::invokeMethod(pUserInfoDlg, "updateDeviceInformation", Q_ARG(QVariant, QVariant::fromValue(data)), Q_ARG(QVariant, QVariant::fromValue(deviceId)));
	}
}

void CrealityCloudUI::setDeviceInformationRealTime(const std::string& date, const std::string& id)
{
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");

	QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
	QMetaObject::invokeMethod(pUserInfoDlg, "updateDeviceInformation_NewIOT_RealTime", Q_ARG(QVariant, QVariant::fromValue(QString(date.c_str()))), Q_ARG(QVariant, QVariant::fromValue(QString(id.c_str()))));
}

void CrealityCloudUI::setDeviceInformationPosition(const std::string& date, const std::string& id)
{
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");

	QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
	QMetaObject::invokeMethod(pUserInfoDlg, "updateDeviceInformation_NewIOT_Position", Q_ARG(QVariant, QVariant::fromValue(QString(date.c_str()))), Q_ARG(QVariant, QVariant::fromValue(QString(id.c_str()))));
}

void CrealityCloudUI::setClusterDeviceList(std::string data)
{
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");

	QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("GroupPrintDlg");
	QMetaObject::invokeMethod(pUserInfoDlg, "setClusterDeviceListData", Q_ARG(QVariant, QVariant::fromValue(QString(data.c_str()))));
}

void CrealityCloudUI::setClusterTaskList(std::string data)
{
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");

	QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("GroupPrintDlg");
	QMetaObject::invokeMethod(pUserInfoDlg, "setClusterTaskListData", Q_ARG(QVariant, QVariant::fromValue(QString(data.c_str()))));
}

void CrealityCloudUI::onLoginDlgClose()
{
	m_imgProvider->setDlgClosed(true);
}

void CrealityCloudUI::onLoginByAccont(QString accont, QString password, QString logintype, bool autoLogin, QString areaCode)
{
	QString loginAccont = "";
	if (logintype == "2")
	{
		loginAccont = accont;
	}
	else
	{
		loginAccont = areaCode + accont;
	}

	QSettings setting;
	setting.beginGroup("profile_setting");
	if (logintype == "2")
	{
		setting.setValue("login_mail_accont", accont);
		//setting.setValue("login_mail_password", password);
	}
	else
	{
		setting.setValue("login_mobile_accont", accont);
		setting.setValue("login_mobile_areaCode", areaCode);
	}
	if (autoLogin)
	{
		setting.setValue("login_auto", "1");
	}
	else
	{
		setting.setValue("login_auto", "0");
		setting.setValue("login_token", "");
		setting.setValue("login_userid", "");
	}
	setting.endGroup();


	QString strContent = QString::fromLatin1("{\"type\": %1, \"account\" : \"%2\", \"password\" : \"%3\"}").arg(logintype).arg(loginAccont).arg(password);
	creative_kernel::getcxNetworkManager()->loginByAccont(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcLoginByAccont, this, std::placeholders::_1));

	return;
}

void CrealityCloudUI::onQucikLoginByAccont(QString phoneNumber, QString phoneAreaCode, QString verifyCode, bool autoLogin)
{
	QSettings setting;
	setting.beginGroup("profile_setting");
	setting.setValue("login_mobile_accont", phoneNumber);
	setting.setValue("login_mobile_areaCode", phoneAreaCode);
	if (autoLogin)
	{
		setting.setValue("login_auto", "1");
	}
	else {
		setting.setValue("login_auto", "0");
	}
	setting.endGroup();
	QString loginAccont = phoneAreaCode + phoneNumber;
	QString strContent = QString::fromLatin1("{\"phoneNumber\": \"%1\", \"phoneAreaCode\" : \"%2\", \"verifyCode\" : \"%3\"}").arg(loginAccont).arg(phoneAreaCode).arg(verifyCode);

	creative_kernel::getcxNetworkManager()->quickLoginByAccont(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcQucikLoginByAccont, this, std::placeholders::_1));
}

void CrealityCloudUI::onGetVerificationCode(QString phoneNumber)
{
	QString strContent = QString::fromLatin1("{\"account\":\"%1\", \"verifyCodeType\" : %2, \"accountType\" : %3}").arg(phoneNumber).arg(6).arg(1);
	creative_kernel::getcxNetworkManager()->getVerificationCode(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcGetVerificationCode, this, std::placeholders::_1));
}

void CrealityCloudUI::onSigAutoLogin(QString token, QString userId)
{
	m_isAutoLogin = true;
	loginSuccess(token, userId);

	//onInitAdvanceParam();
}

void CrealityCloudUI::onShowGcodeUpload(QString type)
{
	if (type == "gcodeUpload")
	{
		QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
		QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");
		QMetaObject::invokeMethod(fdmPanel, "uploadBtnClicked");
	}
	else if (type == "clusteruploadgcode")
	{
		QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
		QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");
		QMetaObject::invokeMethod(fdmPanel, "clusterUploadGcodeBtn");
	}
}

void CrealityCloudUI::onShowModelUpload(QString type)
{
	QObject* pSlicePanel = AbstractKernelUI::getSelf()->getUI("rightpanel");
	QObject* pRifhtPanel = pSlicePanel->findChild<QObject*>("RightInfoPanel");
	QMetaObject::invokeMethod(pRifhtPanel, "uploadModelBtnClick", Q_ARG(QVariant, QVariant::fromValue(type)));
}

void CrealityCloudUI::onGetGCodeList(int page, int pageSize)
{
	QString strContent = QString("{\"page\":%1, \"pageSize\":%2, \"isUpload\":%3}").arg(page).arg(pageSize).arg("true");
	creative_kernel::getcxNetworkManager()->getGCodeList(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcGetGCodeListResult, this, std::placeholders::_1, page));
}

void CrealityCloudUI::onDeleteGCode(QString id)
{
	QString strContent = QString("{\"id\":\"%1\"}").arg(id);
	creative_kernel::getcxNetworkManager()->deleteGcode(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcDeleteGCodeResult, this, std::placeholders::_1, id));
}

void CrealityCloudUI::onGetModelList(QString page, int pageSize)
{
	QString strContent = QString("{\"cursor\":\"%1\", \"limit\":%2}").arg(page).arg(pageSize);
	QString strurl = "/api/cxy/v3/model/listUpload";
	creative_kernel::getcxNetworkManager()->getModelList(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcGetModelListResult, this, std::placeholders::_1, page));
}

void CrealityCloudUI::onDeleteModel(QString id)
{
	QString strContent = QString("{\"id\":\"%1\"}").arg(id);
	creative_kernel::getcxNetworkManager()->deleteModel(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcDeleteModelResult, this, std::placeholders::_1, id));
}

void CrealityCloudUI::onImportModel(QString id, QString name, int type)
{
	importModel(id, name, type);
}

void CrealityCloudUI::importModel(QString id, QString name, int type, QString downModelToLocallPath)
{
	QString strContent = QString("{\"modelId\":\"%1\"}").arg(id);
	std::string downloadPath = downModelToLocallPath.toStdString();
	if (type == 1) {
		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pModelLibraryDlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
		QMetaObject::invokeMethod(pModelLibraryDlg, "startDownLoadModel", Q_ARG(QVariant, QVariant::fromValue(QString("downimport"))));
	}
	QString path = getCanWriteFolder() + "/myModels/" + id + "/";

	QDir tempDir;
	if (!tempDir.exists(path))
	{
		tempDir.mkpath(path);
	}
	getModelGroupDetail(id, 100, type, true, path.toStdString(), true);
}
void CrealityCloudUI::onImportGCode(QString id, QString name, QString loadLink)
{
	if (importGcodeStatus)
	{
		return;
	}
	QObject* pmainWinbj = AbstractKernelUI::getSelf()->getUI("UIAppWindow");
	QMetaObject::invokeMethod(pmainWinbj, "printerConfigBtnClicked");

	QString suffix = loadLink.right(loadLink.length() - loadLink.lastIndexOf("."));

	std::function<void(QByteArray)> call_back = std::bind(
		&CrealityCloudUI::funcDownLoadGCodeResult, this, std::placeholders::_1);
	QMap<QString, QString> data;
	data.clear();
	data.insert("__REQUEST_URl__", loadLink);
	data.insert("__REQUEST_TYPE__", "3");
	data.insert("__REQUEST_TIME_OUT__", "60000");
	creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
	importGcodeStatus = true;
	importGcodeId = id;
	importGcodeName = name;
}

void CrealityCloudUI::onGetCategoryList(int type)
{
	QString strContent = QString("{\"type\":%1}").arg(type);
	creative_kernel::getcxNetworkManager()->getCategoryList(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcGetCategoryList, this, std::placeholders::_1));
}

void CrealityCloudUI::onGetModelLibraryList(QString id, QString page, int size)
{
	QString strContent = "";
	QString strurl = "";
	std::function<void(QByteArray)> call_back;
	if (id == "recommendmodellist")
	{
		strContent = QString("{\"cursor\":\"%1\", \"limit\":%2,\"excludeAd\": true}").arg(page).arg(size);
		creative_kernel::getcxNetworkManager()->getRecommendModel(strContent.toStdString(), std::bind(
			&CrealityCloudUI::funcGetRecommendModelLibraryListResult, this, std::placeholders::_1, page));
	}
	else {
		strContent = QString("{\"cursor\":\"%1\", \"limit\":%2,\"categoryId\": \"%3\", \"filterType\": %4, \"isPay\": %5}").arg(page).arg(size).arg(id).arg(2).arg(2/*0: all; 1: pay;2: free*/);
		creative_kernel::getcxNetworkManager()->getModelLibraryList(strContent.toStdString(), std::bind(
			&CrealityCloudUI::funcGetModelLibraryListResult, this, std::placeholders::_1, page));
	}
}

void CrealityCloudUI::onGetModelGroupDetail(QString id, int count)
{
	getModelGroupDetail(id, count);
}

void CrealityCloudUI::getModelGroupDetail(QString id, int count, int type, bool needDownload, const std::string& downloadPath, const bool& needImport)
{
	QString strContent = QString("{\"cursor\":\"%1\", \"limit\":%2,\"modelId\": \"%3\"}").arg("").arg(count).arg(id);
	creative_kernel::getcxNetworkManager()->getModelGroupDetail(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcGetModelGroupDetailResult, this, std::placeholders::_1, type, needDownload, downloadPath, needImport));
}

void CrealityCloudUI::onGetModelGroupInfomation(QString id)
{
	QString strContent = QString("{\"id\":\"%1\"}").arg(id);
	creative_kernel::getcxNetworkManager()->getModelGroupInfomation(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcGetModelGroupInfomationResult, this, std::placeholders::_1));
}

void CrealityCloudUI::onSearchModelList(QString key, int page, int size)
{
	QString strContent = QString("{\"page\":%1, \"pageSize\":%2,\"keyword\": \"%3\"}").arg(page).arg(size).arg(key);
	creative_kernel::getcxNetworkManager()->searchModelList(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcSearchModelListResult, this, std::placeholders::_1, page));
}

void CrealityCloudUI::onImportDetailItem(QString id, QString name, QString link)
{
	importDetailItem(id, name, link);
}

void CrealityCloudUI::importDetailItem(QString id, QString name, QString link, QString downModelToLocallPath)
{
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	QObject* pModelLibraryDlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
	QMetaObject::invokeMethod(pModelLibraryDlg, "startDownloadDetailModel");

	QString fullpath;
	if (downModelToLocallPath.isEmpty())
	{
		fullpath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/" + name + ".stl";
	}
	else
	{
		fullpath = downModelToLocallPath + "/" + name + ".stl";
	}

	QString strContent = QString("{\"id\":\"%1\"}").arg(id);
	creative_kernel::getcxNetworkManager()->importDetailItem(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcImportDetailItemResult, this, std::placeholders::_1, fullpath.toStdString()), fullpath.toStdString());
}

void CrealityCloudUI::onShareModel(QString id)
{
	QSettings setting;
	setting.beginGroup("profile_setting");
	QString strStartType = setting.value("service_type", "-1").toString();
	setting.endGroup();
	QString clipBoardContent = "https://www.crealitycloud.cn/model-detail/";//"http://model-dev.crealitygroup.com/model-detail/";
#ifdef CLOUD_BETA_URL
	if (strStartType == "0")
	{
		clipBoardContent = "https://www.crealitycloud.cn/model-detail/";
	}
	else if (strStartType == "1")
	{
		clipBoardContent = "https://www.crealitycloud.com/model-detail/";
	}
#endif
	clipBoardContent = clipBoardContent + id;
	QClipboard* clipboard = QApplication::clipboard();
	QMimeData* mimeData = new QMimeData();
	mimeData->setText(clipBoardContent);
	clipboard->setMimeData(mimeData);

	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
	QMetaObject::invokeMethod(pUserInfoDlg, "copyModelLinkSuccess");
}

void CrealityCloudUI::onGetFavoritedList(QString page, int pageSize)
{
	QString strContent = QString("{\"cursor\":\"%1\", \"limit\":%2}").arg(page).arg(pageSize);
	creative_kernel::getcxNetworkManager()->getFavoritedList(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcGetFavoritedListResult, this, std::placeholders::_1, page));
}

void CrealityCloudUI::onCancelFavoriteModel(QString id)
{
	QString strContent = QString("{\"id\":\"%1\", \"action\": %2}").arg(id).arg("false");
	creative_kernel::getcxNetworkManager()->addOrDeleteFavoriteModel(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcCancelFavoriteModelResult, this, std::placeholders::_1, 1, id));
}

void CrealityCloudUI::onFavoriteModel(QString id, bool status)
{
	if (creative_kernel::getcxNetworkManager()->getLoginInfoLoginState() == 1)
	{
		QString strContent = "";
		if (status)
		{
			strContent = QString("{\"id\":\"%1\", \"action\": %2}").arg(id).arg("false");
		}
		else {
			strContent = QString("{\"id\":\"%1\", \"action\": %2}").arg(id).arg("true");
		}
		
		creative_kernel::getcxNetworkManager()->addOrDeleteFavoriteModel(strContent.toStdString(), std::bind(
			&CrealityCloudUI::funcCancelFavoriteModelResult, this, std::placeholders::_1, 2, id));
	}
	else
	{
		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QString param = "modelLibraryCollect";
		QMetaObject::invokeMethod(pTopBar, "showUploadMsgDlg", Q_ARG(QVariant, QVariant::fromValue(param)));
	}
}

void CrealityCloudUI::onSigDownLoadModelType(QString type)
{
	if (creative_kernel::getcxNetworkManager()->getLoginInfoLoginState() == 1)
	{
		//open folder dialog
		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pModelDlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
		QMetaObject::invokeMethod(pModelDlg, "openDownFolderDlg", Q_ARG(QVariant, QVariant::fromValue(type)));

	}
	else
	{
		/*QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QString param = type;
		QMetaObject::invokeMethod(pTopBar, "showUploadMsgDlg", Q_ARG(QVariant, QVariant::fromValue(param)));*/
		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pModelDlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
		QMetaObject::invokeMethod(pModelDlg, "openDownFolderDlg", Q_ARG(QVariant, QVariant::fromValue(type)));

	}
}

void CrealityCloudUI::onSigDownLoadModelToLocall(QString id, int count, QString name, QString path)
{
	QUrl tempUrl = QUrl(path);
	auto downModelToLocallPath = tempUrl.toLocalFile() + "/";
	if (count != 0)
	{
		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pModelLibraryDlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
		QMetaObject::invokeMethod(pModelLibraryDlg, "startDownLoadModel", Q_ARG(QVariant, QVariant::fromValue(QString("downlocall"))));

		getModelGroupDetail(id, count,3, true, downModelToLocallPath.toStdString());
	}
	else {
		downDetailItemType = 1;
		importDetailItem(id, name, "", downModelToLocallPath);
	}

	creative_kernel::sendDataToGA("Model Download", "Download From Model Library to Local");
}

void CrealityCloudUI::onGetPrinterList(int page, int pageSize)
{
	creative_kernel::getcxNetworkManager()->getPrinterTypeList(std::bind(
		&CrealityCloudUI::funcGetPrinterTypeListResult, this, std::placeholders::_1));

	QString strContent = QString("{\"page\":%1, \"pageSize\":%2}").arg(page).arg(pageSize);
	creative_kernel::getcxNetworkManager()->getOnwerDeviceList(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcGetOnwerDeviceListResult, this, std::placeholders::_1));
}

void CrealityCloudUI::onGetGCodeListPrintControl(int page, int pageSize, QString type)
{
	if (type == "gcode")
	{
		QString strContent = QString("{\"page\":%1, \"pageSize\":%2, \"isUpload\":%3}").arg(page).arg(pageSize).arg("true");
		QString strurl = "/api/cxy/v2/gcode/ownerList";
		std::function<void(QByteArray)> call_back = std::bind(
			&CrealityCloudUI::funcGetGCodeListPrintControlResult, this, std::placeholders::_1);
		QMap<QString, QString> data;
		data.clear();
		data.insert("__REQUEST_URl__", strurl);
		data.insert("__REQUEST_TYPE__", "1");
		data.insert("__REQUEST_BODY__", strContent.toUtf8());
		data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
		data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
		creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);

		creative_kernel::sendDataToGA("Creality Cloud Printing", "My Slices-Printing");
	}
	else if (type == "cloudslice")
	{
		QString strContent = QString("{\"page\":%1, \"pageSize\":%2, \"isUpload\":%3}").arg(page).arg(pageSize).arg("false");
		QString strurl = "/api/cxy/v2/gcode/ownerList";
		std::function<void(QByteArray)> call_back = std::bind(
			&CrealityCloudUI::funcGetGCodeListPrintControlResult, this, std::placeholders::_1);
		QMap<QString, QString> data;
		data.clear();
		data.insert("__REQUEST_URl__", strurl);
		data.insert("__REQUEST_TYPE__", "1");
		data.insert("__REQUEST_BODY__", strContent.toUtf8());
		data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
		data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
		creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);

		creative_kernel::sendDataToGA("Creality Cloud Printing", "My Slices-Printing");
	}
	else if (type == "devices")
	{
		QString strContent = QString("{\"page\":%1, \"pageSize\":%2}").arg(page).arg(pageSize);
		QString strurl = "/api/cxy/v2/gcode/ownerList";
		std::function<void(QByteArray)> call_back = std::bind(
			&CrealityCloudUI::funcGetGCodeListPrintControlResult, this, std::placeholders::_1);
		QMap<QString, QString> data;
		data.clear();
		data.insert("__REQUEST_URl__", strurl);
		data.insert("__REQUEST_TYPE__", "1");
		data.insert("__REQUEST_BODY__", strContent.toUtf8());
		data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
		data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
		creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);

		creative_kernel::sendDataToGA("Creality Cloud Printing", "My Devices-Printing");
	}
}

void CrealityCloudUI::onSigStartPrintGcode(QString url, QString productName, QString iotId, QString productKey, QString gcodeId, QString printerName, QString printerId, QString tbid, int type)
{
	if (startPrintGcodeStatus)
	{
		return;
	}
	startPrintGcodeType = type;
	startPrintGcodeUrl = url;
	startPrintGcodeProductName = productName;
	startPrintGcodeProductKey = productKey;
	startPrintGcodeIotId = iotId;

	//get printId
	QString strContent = QString("{\"gcodeId\":\"%1\", \"deviceName\":\"%2\", \"printer\":{\"custom\": false, \"id\": %3, \"name\":\"%4\", \"type\":1}}").arg(gcodeId).arg(productName).arg(printerId.toInt()).arg(printerName);
	creative_kernel::getcxNetworkManager()->startPrintGcode(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcGetStartPrintGcodePrintIdResult, this, std::placeholders::_1, std::placeholders::_2, url, tbid));

	startPrintGcodeStatus = true;
}

void CrealityCloudUI::onSigControlPrintGcode(QString productName, QString iotId, QString productKey, QString control, QString tbid, int type)
{
	if (type == 1)//old iot
	{
		controlPrintGcodeProductName = productName;
		controlPrintGcodeProductKey = productKey;
		controlPrintGcodeIotId = iotId;
		controlPrintGcodeInfo = control;

		getAliyunInfoControlPrintGcode();
	}
	else if (type == 2)//new iot
	{
		QString info;
		if (control == "pause")
		{
			info = QString("{\"pause\":%1}").arg(1);
		}
		else if (control == "continue")
		{
			info = QString("{\"pause\":%1}").arg(0);
		}
		else if (control == "stop")
		{
			info = QString("{\"stop\":%1}").arg(1);

			//info = QString("{\"printId\":\"%1\"}").arg("");
		}
		else {
			return;
		}
		QString strContent = QString("{\"method\": \"set\", \"params\": %1}").arg(info);
		QString strurl = "/api/rest/iotrouter/rpc/oneway/" + tbid;
		creative_kernel::getcxNetworkManager()->setDeviceDesiredProperty_NewIOTControlPrintGcode(tbid.toStdString(), strContent.toStdString(), std::bind(
			&CrealityCloudUI::funcSetDeviceDesiredProperty_NewIOTControlPrintGcode, this, std::placeholders::_1));
	}
}

void CrealityCloudUI::onSigControlPrintTempSpeed(QString productName, QString iotId, QString productKey, QString info, int value, QString tbid, int type)
{
	if (controlPrintTempSpeedStatus)
	{
		return;
	}
	
	if (type == 1)//old iot
	{
		controlPrintTempSpeedProductName = productName;
		controlPrintTempSpeedProductKey = productKey;
		controlPrintTempSpeedIotId = iotId;
		controlPrintTempSpeedInfo = info;
		controlPrintTempSpeedValue = value;
		getAliyunInfoControlPrintTempSpeed();
	}
	else if (type == 2) //new iot
	{
		QString param;
		if (info == "nozzletargettemp")
		{
			param = QString("{\"nozzleTemp2\":%1}").arg(value);
		}
		else if (info == "hotbedtargettemp")
		{
			param = QString("{\"bedTemp2\":%1}").arg(value);
		}
		else if (info == "fanswitch")
		{
			param = QString("{\"fan\":%1}").arg(value);
		}
		else if (info == "printspeedtarget")
		{
			param = QString("{\"curFeedratePct\":%1}").arg(value);
		}
		else if (info == "gcodeCmdG0X")
		{
			if (value == 0)
			{
				value = 0;
			}
			else {
				value = value / 100;
			}
			param = QString("{\"gcodeCmd\":\"G0 X%1\"}").arg(value);
		}
		else if (info == "gcodeCmdG0Y")
		{
			if (value == 0)
			{
				value = 0;
			}
			else {
				value = value / 100;
			}
			param = QString("{\"gcodeCmd\":\"G0 Y%1\"}").arg(value);
		}
		else if (info == "gcodeCmdG0Z")
		{
			if (value == 0)
			{
				value = 0;
			}
			else {
				value = value / 100;
			}
			param = QString("{\"gcodeCmd\":\"G0 Z%1\"}").arg(value);
		}
		else if (info == "gcodeCmdG28XY")
		{
			param = QString("{\"gcodeCmd\":\"G28 X Y\"}");
		}
		else if (info == "gcodeCmdG28Z")
		{
			param = QString("{\"gcodeCmd\":\"G28 Z\"}");
		}
		else {
			return;
		}

		QString strContent = QString("{\"method\": \"set\", \"params\": %1}").arg(param);
		creative_kernel::getcxNetworkManager()->setDeviceDesiredProperty_NewIOTControlPrintTempSpeed(strContent.toStdString(), tbid.toStdString(), std::bind(
			&CrealityCloudUI::funcSetDeviceDesiredProperty_NewIOTControlPrintTempSpeed, this, std::placeholders::_1));
	}

	controlPrintTempSpeedStatus = true;
}

void CrealityCloudUI::onSigUpdatePreviewImage(QString printID, QString deviceID)
{
	if (printID.left(6) == "local_")
	{
		return;
	}
	if (updatePreviewImageStatus)
	{
		return;
	}

	QString strContent = QString("{\"id\": \"%1\"}").arg(printID);
	QString strurl = "/api/cxy/v2/print/printDetail";
	std::function<void(QByteArray)> call_back = std::bind(
		&CrealityCloudUI::funcGetUpdatePreviewImage, this, std::placeholders::_1);
	QMap<QString, QString> data;
	data.clear();
	data.insert("__REQUEST_URl__", strurl);
	data.insert("__REQUEST_TYPE__", "1");
	data.insert("__REQUEST_BODY__", strContent.toUtf8());
	data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
	data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
	creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
	updatePreviewImageDeviceId = deviceID;
	updatePreviewImageStatus = true;
}

void CrealityCloudUI::onSigGetPrintingGcodeInfo(QString printID)
{
	if (printID.left(6) == "local_")
	{
		return;
	}

	QString strContent = QString::fromLatin1("{\"id\": \"%1\"}").arg(printID);
	QString strurl = "/api/cxy/v2/print/printDetail";
	std::function<void(QByteArray)> call_back = std::bind(
		&CrealityCloudUI::funcGetPrintingGcodeInfoResult, this, std::placeholders::_1);
	QMap<QString, QString> data;
	data.clear();
	data.insert("__REQUEST_URl__", strurl);
	data.insert("__REQUEST_TYPE__", "1");
	data.insert("__REQUEST_BODY__", strContent.toUtf8());
	data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
	data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
	creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
}

void CrealityCloudUI::onInitP2PPipeline(QString strApiLicense, QString strDid, QString strInit, int portNum, QString productName)
{
	m_pCameraServer = new CameraServer(portNum);

	//QString strDid = "CNA-000884-KZNLZ";
	//QString strApiLicense = "EMYUSV";
	//QString strInit = "EBGCEBBBKLJDHFJFEAGFENENHLMMGINMGDEJAECLBEILLLLACFBFCBOMHBLNJAKLADMOLFDBOBMKAMCAJGNMIDAJ";

	QSettings setting;
	setting.beginGroup("profile_setting");
	QString strStartType = setting.value("service_type", "-1").toString();
	setting.endGroup();
	strInit = "EBGCEBBBKLJDHFJFEAGFENENHLMMGINMGDEJAECLBEILLLLACFBFCBOMHBLNJAKLADMOLFDBOBMKAMCAJGNMIDAJ";
#ifdef CLOUD_BETA_URL
	if (strStartType == "0")
	{
		strInit = "EBGCEBBBKLJDHFJFEAGFENENHLMMGINMGDEJAECLBEILLLLACFBFCBOMHBLNJAKLADMOLFDBOBMKAMCAJGNMIDAJ";
	}
	else if (strStartType == "1")
	{
		strInit = "EEGDFHBLKGJIGEJLEKGIFKECHANKHMNDHDFJBACAAGJBLNLFDJAMCCOHGBLFJMLAAEMFLDDHOBMOBECEIM";
	}
#endif


	QObject::connect(m_pCameraServer, SIGNAL(sigCameraRestart(QString)), this, SLOT(restartP2PPipeline(QString)));


	m_pCameraServer->startP2PServer(productName.toStdString(), strDid.toStdString(), strApiLicense.toStdString(), strInit.toStdString());
	m_vecCameraServer.push_back(m_pCameraServer);
}

void CrealityCloudUI::restartP2PPipeline(QString strdid)
{
	qDebug() << "restartP2PPipeline";
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	QMetaObject::invokeMethod(pTopBar, "restartVideo", Q_ARG(QVariant, QVariant::fromValue(strdid)));


	//QObject* pPrintControl = pTopBar->findChild<QObject*>("PrintControlInfoDlg");
	//QMetaObject::invokeMethod(pPrintControl, "restartVideoPlay");
}

void CrealityCloudUI::onPauseGetDeviceTask(QString pause)
{
	if (pause == "exit")
	{
		for (int i = 0; i < m_vecCameraServer.size(); i++)
		{
			if (m_vecCameraServer[i])
			{
				m_vecCameraServer[i]->stopVideoThread();
			}
		}
		creative_kernel::getcxNetworkManager()->uninit();
		m_threadPool->waitForDone(100);
	}
	else if (pause == "loginout")
	{
		if (m_queryEquipmentTask) {
			m_queryEquipmentTask->setStop(true);
		}
		m_queryEquipmentTask = nullptr;

	}
}

void CrealityCloudUI::onSigInitCameraLicense(QString productName, QString iotId, QString productKey, QString did, QString license, QString tbid, int type)
{
	if (initCameraLicenseStatus)
	{
		return;
	}

	if (type == 1)//old iot
	{

		initCameraLicenseProductName = productName;
		initCameraLicenseProduceKey = productKey;
		initCameraLicenseIotId = iotId;
		initCameraLicenseDid = did;
		initCameraLicenseLicense = license;

		getAliyunInfoInitCameraLicense();

//		QSettings setting;
//		setting.beginGroup("profile_setting");
//		QString strStartType = setting.value("service_type", "-1").toString();
//		setting.endGroup();
//		QString syInitString = "EBGCEBBBKLJDHFJFEAGFENENHLMMGINMGDEJAECLBEILLLLACFBFCBOMHBLNJAKLADMOLFDBOBMKAMCAJGNMIDAJ";
//		//"EFGBFFBJKEJKGGJJEEGFFHELHHNNHONHGLFNBHCCAEJDLNLPDDAGCIOFGDLGJMLAAOMOKCDLOONOBICJJIMM";
//#ifdef CLOUD_BETA_URL
//		if (strStartType == "0")
//		{
//			syInitString = "EBGCEBBBKLJDHFJFEAGFENENHLMMGINMGDEJAECLBEILLLLACFBFCBOMHBLNJAKLADMOLFDBOBMKAMCAJGNMIDAJ";
//		}
//		else if (strStartType == "1")
//		{
//			syInitString = "EEGDFHBLKGJIGEJLEKGIFKECHANKHMNDHDFJBACAAGJBLNLFDJAMCCOHGBLFJMLAAEMFLDDHOBMOBECEIM";
//		}
//#endif

	}
	else if (type == 2)//new iot
	{
		//todo
	}

	initCameraLicenseStatus = true;
}

void CrealityCloudUI::onSigUpdateDeviceData(QString productName, QString iotId, QString productKey, QString tbid, int type)
{
	if (type == 1)//old iot
	{
		updateDeviceDataProductName = productName;
		updateDeviceDataProductKey = iotId;
		updateDeviceDataIotId = productKey;
		getAliyunInfoUpdateDeviceData();
	}
	else if (type == 2)//new iot
	{
		creative_kernel::getcxNetworkManager()->queryDevicePropertyStatus_NewIOT(tbid.toStdString(), std::bind(
			&CrealityCloudUI::funcUpdateDeviceDataNewIotResult, this, std::placeholders::_1));
	}
}

void CrealityCloudUI::onGetCloudSliceList(int page, int pageSize)
{
	if (getCloudSliceListStatus)
	{
		return;
	}
	QString strContent = QString("{\"page\":%1, \"pageSize\":%2, \"isUpload\":%3}").arg(page).arg(pageSize).arg("false");
	QString strurl = "/api/cxy/v2/gcode/ownerList";
	std::function<void(QByteArray)> call_back = std::bind(
		&CrealityCloudUI::funcGetCloudSliceListResult, this, std::placeholders::_1);
	QMap<QString, QString> data;
	data.clear();
	data.insert("__REQUEST_URl__", strurl);
	data.insert("__REQUEST_TYPE__", "1");
	data.insert("__REQUEST_BODY__", strContent.toUtf8());
	data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
	data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
	creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);

	getCloudSliceListStatus = true;
	getCloudSliceListPage = page;
}

void CrealityCloudUI::onSigPersonalSpaceTutorial(QString type)
{
	QSettings setting;
	setting.beginGroup("profile_setting");
	QString strStartType = setting.value("service_type", "-1").toString();

	if (type == "myslices")
	{
		if (strStartType == "0")
			QDesktopServices::openUrl(QUrl(QString("https://www.crealitycloud.cn/post-detail/7776")));
		else
			QDesktopServices::openUrl(QUrl(QString("https://www.crealitycloud.com/post-detail/9768")));
	}
	else if (type == "mymodels")
	{
		if (strStartType == "0")
			QDesktopServices::openUrl(QUrl(QString("https://www.crealitycloud.cn/post-detail/7703")));
		else
			QDesktopServices::openUrl(QUrl(QString("https://www.crealitycloud.com/post-detail/9766")));
	}
	else if (type == "myfavorited")
	{
		if (strStartType == "0")
			QDesktopServices::openUrl(QUrl(QString("https://www.crealitycloud.cn/post-detail/7770")));
		else
			QDesktopServices::openUrl(QUrl(QString("https://www.crealitycloud.com/post-detail/9769")));

	}
	else if (type == "myprinter")
	{
		QDesktopServices::openUrl(QUrl(QString("www.baidu.com")));
	}
}

void CrealityCloudUI::onGetRecommendModelAlwaysShowDlg()
{
	QString strContent = QString("{\"cursor\":\"%1\", \"limit\":%2,\"excludeAd\": true}").arg(0).arg(8);

	creative_kernel::getcxNetworkManager()->getRecommendModel(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcGetRecommendModelAlwaysShowDlgResult, this, std::placeholders::_1));
}

void CrealityCloudUI::onShowModelLibraryDlg(bool isMoreBtn, QString id, QString name, int count, QString author, QString avtar, QString ctime)
{
	if (!isMoreBtn)
	{
		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QMetaObject::invokeMethod(pTopBar, "showModelLibraryDialog");

		QObject* pModelDlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
		QMetaObject::invokeMethod(pModelDlg, "onSigButtonClicked", Q_ARG(QVariant, QVariant::fromValue(id)),
			Q_ARG(QVariant, QVariant::fromValue(name)),
			Q_ARG(QVariant, QVariant::fromValue(count)),
			Q_ARG(QVariant, QVariant::fromValue(author)),
			Q_ARG(QVariant, QVariant::fromValue(avtar)),
			Q_ARG(QVariant, QVariant::fromValue(ctime)));
	}
	else {
		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QMetaObject::invokeMethod(pTopBar, "showModelLibraryDialog");

		QObject* pModelDlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
		QMetaObject::invokeMethod(pModelDlg, "justShowRecommendModel");
	}

	creative_kernel::sendDataToGA("Model Library", "Model Library (Home)");
}

void CrealityCloudUI::onGetPurchasedList(QString page, int pageSize)
{
	if (getPurchasedListStatus)
	{
		return;
	}

	QString strContent = QString("{\"cursor\":\"%1\", \"limit\":%2, \"userId\":%3, \"payStatus\":%4}").arg(page).arg(pageSize).arg(creative_kernel::getcxNetworkManager()->getLoginInfoUserID()).arg(2);
	QString strurl = "/api/cxy/v3/model/modelOrderList";
	std::function<void(QByteArray)> call_back = std::bind(
		&CrealityCloudUI::funcGetPurchasedListResult, this, std::placeholders::_1);
	QMap<QString, QString> data;
	data.clear();
	data.insert("__REQUEST_URl__", strurl);
	data.insert("__REQUEST_TYPE__", "1");
	data.insert("__REQUEST_BODY__", strContent.toUtf8());
	data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
	data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
	creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);

	getPurchasedListStatus = true;
	getPurchasedListPage = page;
}

void CrealityCloudUI::onWizardClicked()
{
//	AbstractKernelUI::getSelf()->switchPickMode();
}

void CrealityCloudUI::onUserInfoDlgClicked()
{
	creative_kernel::sendDataToGA("Personal Center", "Personal Center");
}

void CrealityCloudUI::onServerTypeChanged(int serverType)
{
	QSettings setting;
	setting.beginGroup("profile_setting");
	setting.setValue("service_type", QString::number(serverType));
	setting.endGroup();

	QString urlStr = "http://model-dev.crealitygroup.com";
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	QObject* plogindlg = pTopBar->findChild<QObject*>("LoginDlg");


	if (serverType == 0)
	{
#ifdef CLOUD_BETA_URL
		urlStr = "https://www.crealitycloud.cn";
#endif
	}
	else if (serverType == 1)
	{
#ifdef CLOUD_BETA_URL
		urlStr = "https://www.crealitycloud.com";
#endif
	}

	QMetaObject::invokeMethod(plogindlg, "setWebUrl", Q_ARG(QVariant, urlStr));

	creative_kernel::sendDataToGA("Server", "Choose Server");
}

void CrealityCloudUI::onInitAdvanceParam()
{
	QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
	if (pRightPanel)
	{
		QMetaObject::invokeMethod(pRightPanel, "initAdvanceParam");
	}
}

void CrealityCloudUI::onGetGroupPrintDeviceList()
{
	creative_kernel::getcxNetworkManager()->setFuncDeviceListCb(std::bind(&CrealityCloudUI::setClusterDeviceList, this, std::placeholders::_1));
	creative_kernel::getcxNetworkManager()->setFuncTaskListCb(std::bind(&CrealityCloudUI::setClusterTaskList, this, std::placeholders::_1));
}

void CrealityCloudUI::onGetClusterGcodeList(int page, int pageSize)
{
	QString strContent = QString("{\"page\":%1, \"pageSize\":%2}").arg(page).arg(pageSize);

	creative_kernel::getcxNetworkManager()->getGcodeList(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funconGetClusterGcodeListResult, this, std::placeholders::_1, page));
}

void CrealityCloudUI::onAddClusterTask(QString ids, QString info)
{
	QString strContent = QString("{\"seriesIds\":[0], \"deviceModels\":%1, \"gcodeInfo\":%2}").arg(ids).arg(info);
	creative_kernel::getcxNetworkManager()->addTask(strContent.toStdString(), std::bind(
		&CrealityCloudUI::funcAddClusterTaskResult, this, std::placeholders::_1));
}

void CrealityCloudUI::onClusterConformPrinterStatus(QString name, QString status)
{
	if (status == "clusteridle")
	{
		creative_kernel::getcxNetworkManager()->confirmDeviceIdle(name.toStdString(), std::bind(
			&CrealityCloudUI::funcClusterConformIdleResult, this, std::placeholders::_1));
	}
	else if (status == "clustercancle")
	{
		creative_kernel::getcxNetworkManager()->cancelTask(name.toStdString(), std::bind(
			&CrealityCloudUI::funcClusterCancelPrintTaskResult, this, std::placeholders::_1));
	}
	else if (status == "viewprintreport")
	{
		creative_kernel::getcxNetworkManager()->getPrintReport(name.toStdString(), std::bind(
			&CrealityCloudUI::funcClusterTaskReportResult, this, std::placeholders::_1));
	}
}

void CrealityCloudUI::onClusterUploadGcode()
{
	QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
	QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");
	QMetaObject::invokeMethod(fdmPanel, "clusterUploadGcodeBtn");
}

void CrealityCloudUI::onSigSelectPrintControl(QString type)
{
	if (creative_kernel::getcxNetworkManager()->getLoginInfoLoginState() == 1)
	{
		if (type == "clusterprint")
		{
			QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
			QMetaObject::invokeMethod(pTopBar, "showGroupPrintInfoDlg");
		}
		else if (type == "singleprint")
		{
			QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
			QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
			QMetaObject::invokeMethod(pUserInfoDlg, "setUserInfoDlgShow", Q_ARG(QVariant, QVariant::fromValue(type)));
		}
	}
	else 
	{
		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QMetaObject::invokeMethod(pTopBar, "showUploadMsgDlg", Q_ARG(QVariant, QVariant::fromValue(type)));
	}
}

void CrealityCloudUI::onSigChangeClusterTaskTime(QString time)
{
	//if (time == "30000ms")
	//{
	//	if (m_queryClusterDeviceTask) {
	//		m_queryClusterDeviceTask->setSleepTime(30000);
	//	}
	//}
	//else if (time == "5000ms")
	//{
	//	if (m_queryClusterDeviceTask) {
	//		m_queryClusterDeviceTask->setSleepTime(5000);
	//	}
	//}
}

void CrealityCloudUI::onClusterGcodeSliceTypeList()
{
	creative_kernel::getcxNetworkManager()->getGcodeSliceTypeListSuffix(std::bind(
		&CrealityCloudUI::funcClusterGcodeSliceTypeListResult, this, std::placeholders::_1));
}

void CrealityCloudUI::onClsterResetGcodeSliceType(QString gcodeId, QString sliceId)
{
	QString strContent = QString("{\"id\":\"%1\",\"parameter\":{\"device\":{\"id\":%2}}}").arg(gcodeId).arg(sliceId.toInt());
	QString strurl = "/api/cxy/v2/gcode/editGcode";
	std::function<void(QByteArray)> call_back = std::bind(
		&CrealityCloudUI::funcClsterResetGcodeSliceTypeResult, this, std::placeholders::_1);
	QMap<QString, QString> data;
	data.clear();
	data.insert("__REQUEST_URl__", strurl);
	data.insert("__REQUEST_TYPE__", "1");
	data.insert("__REQUEST_BODY__", strContent.toUtf8());
	data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
	data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
	creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
}

void CrealityCloudUI::funcLoginSuccess(const std::string& response)
{
	QJsonParseError error;
	QString errorMsg = "";
	const QJsonDocument document = QJsonDocument::fromJson(response.c_str(), &error);
	if (error.error != QJsonParseError::NoError)
	{
		qWarning() << "[funcLoginSuccess QJsonDocument]" << error.errorString() << "\n";
		loginFailed("Network Error");
		return;
	}
	QString strJson(document.toJson(QJsonDocument::Compact));

	QJsonObject object = document.object();
	int state = object.value(QString::fromLatin1("code")).toInt();

	if (state == 0)
	{
		const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
		const QJsonObject userInfoObj = result.value(QString::fromLatin1("userInfo")).toObject();
		const QJsonObject userbaseInfoObj = userInfoObj.value(QString::fromLatin1("base")).toObject();

		QString avatar = userbaseInfoObj.value(QString::fromLatin1("avatar")).toString();
		QString nickName = userbaseInfoObj.value(QString::fromLatin1("nickName")).toString();
		double maxStorageSpace = userInfoObj.value(QString::fromLatin1("maxStorageSpace")).toDouble();
		double usedStorageSpace = userInfoObj.value(QString::fromLatin1("usedStorageSpace")).toDouble();
		int loginState = 1;

		QSettings setting;
		setting.beginGroup("profile_setting");
		QString token = setting.value("login_token", "").toString();
		QString userID = setting.value("login_userid", "").toString();
		setting.endGroup();

		creative_kernel::getcxNetworkManager()->setloginInfo(loginState, token, userID);
		creative_kernel::getcxNetworkManager()->setUserInfo(nickName, avatar, maxStorageSpace, usedStorageSpace);


		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		//QQmlProperty::write(pTopBar, "imageurl", "");
		//QQmlProperty::write(pTopBar, "imageurl", creative_kernel::getcxNetworkManager()->getLoginInfoAvatar());

		QMetaObject::invokeMethod(pTopBar, "loginSuccess", Q_ARG(QVariant, QVariant::fromValue(creative_kernel::getcxNetworkManager()->getLoginInfoUserID())),
			Q_ARG(QVariant, QVariant::fromValue(creative_kernel::getcxNetworkManager()->getLoginInfoNickName())),
			Q_ARG(QVariant, QVariant::fromValue(creative_kernel::getcxNetworkManager()->getLoginInfoAvatar())),
			Q_ARG(QVariant, QVariant::fromValue(creative_kernel::getcxNetworkManager()->getLoginInfoUsedStorageSpace())),
			Q_ARG(QVariant, QVariant::fromValue(creative_kernel::getcxNetworkManager()->getLoginInfoMaxStorageSpace())));

	}
	else
	{
		QSettings setting;
		setting.beginGroup("profile_setting");
		QString token = setting.value("login_token", "").toString();
		QString userID = setting.value("login_userid", "").toString();
		setting.endGroup();

		creative_kernel::getcxNetworkManager()->setloginInfo(0, token, userID);
		errorMsg = object.value(QString::fromLatin1("msg")).toString();

		qDebug() << "getUserInfoFromCloud error:" << errorMsg;

		if (state == 4)
		{
			qDebug() << "token error:";
			QSettings setting;
			setting.beginGroup("profile_setting");
			setting.setValue("login_auto", "0");
			setting.setValue("login_token", "");
			setting.setValue("login_userid", "");
			setting.endGroup();
		}

		loginFailed(errorMsg);
		return;
	}

	if (m_isAutoLogin)
	{
		onInitAdvanceParam();
		m_isAutoLogin = false;
	}
}

void CrealityCloudUI::funcQrLogin(const std::string& identical, const time_t& expireTime)
{
	//QJsonParseError error;

	//QByteArray arrayJson = response;
	//const QJsonDocument document = QJsonDocument::fromJson(arrayJson, &error);
	//if (error.error != QJsonParseError::NoError)
	//{
	//	qWarning() << "[funcQrLogin QJsonDocument]" << error.errorString() << "\n";
	//	loginFailed("Network Error");
	//	return;
	//}
	//QString strJson(document.toJson(QJsonDocument::Compact));

	//QJsonObject object = document.object();
	//const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
	//QString infoStr = result.value(QString::fromLatin1("identical")).toString();
	//QString expireTimeStr = result.value(QString::fromLatin1("expireTime")).toString();

	m_imgProvider->setIdentical(identical.c_str());
	//m_imgProvider->setExpireTime(expireTimeStr);
	//creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);

	qDebug() << "loginUpdate";
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	QObject* pLogin = pTopBar->findChild<QObject*>("LoginDlg");
	QQmlProperty::write(pLogin, "imageurl", "");
	QQmlProperty::write(pLogin, "imageurl", m_imgProvider->providerUrl());

	QString strContent = QString::fromLatin1("{\"identical\": \"%1\"}").arg(identical.c_str());
	creative_kernel::getcxNetworkManager()->startQueryQrLoginInfo(strContent.toStdString(), std::bind(
		&CrealityCloudUI::onLoginCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void CrealityCloudUI::onLoginCallback(const int& state, const std::string& token, const std::string& useId)
{
	switch (state)
	{
	case 3:
	{
		QSettings setting;
		setting.beginGroup("profile_setting");
		setting.setValue("login_auto", "1");
		setting.setValue("login_token", token.c_str());
		setting.setValue("login_userid", useId.c_str());
		setting.endGroup();
		loginSuccess(token.c_str(), useId.c_str());
	}
		break;
	case 4:
	case 5:
		loginUpdate();
		break;
	default:
		break;
	}
}

void CrealityCloudUI::funcLoginByAccont(const std::string& response)
{
	QString errorMsg = "";
	QJsonParseError error;
	QString logintype = "";
	const QJsonDocument document = QJsonDocument::fromJson(response.c_str(), &error);
	if (error.error != QJsonParseError::NoError)
	{
		qWarning() << "[funcLoginByAccont QJsonDocument]" << error.errorString() << "\n";
		loginFailed("Network Error");
		return;
	}
	QString strJson(document.toJson(QJsonDocument::Compact));

	QJsonObject object = document.object();
	const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
	int state = object.value(QString::fromLatin1("code")).toInt();

	if (state == 0)
	{
		creative_kernel::getcxNetworkManager()->setloginInfo(1, result.value(QString::fromLatin1("token")).toString(), result.value(QString::fromLatin1("userId")).toString());
		logintype = result.value(QString::fromLatin1("type")).toString();

		QSettings setting;
		setting.beginGroup("profile_setting");
		setting.setValue("login_token", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
		setting.setValue("login_userid", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
		setting.endGroup();
		loginSuccess(creative_kernel::getcxNetworkManager()->getLoginInfoToken(), creative_kernel::getcxNetworkManager()->getLoginInfoUserID());

	}
	else
	{
		creative_kernel::getcxNetworkManager()->setloginInfo(0, "", "");
		errorMsg = object.value(QString::fromLatin1("msg")).toString();

		QSettings setting;
		setting.beginGroup("profile_setting");
		setting.setValue("login_token", "");
		setting.setValue("login_userid", "");
		setting.endGroup();

		loginFailed(errorMsg);
	}
}

void CrealityCloudUI::funcQucikLoginByAccont(const std::string& response)
{
	QString errorMsg = "";
	QJsonParseError error;
	const QJsonDocument document = QJsonDocument::fromJson(response.c_str(), &error);
	if (error.error != QJsonParseError::NoError)
	{
		qWarning() << "[funcQucikLoginByAccont QJsonDocument]" << error.errorString() << "\n";
		loginFailed("Network Error");
		return;
	}
	QString strJson(document.toJson(QJsonDocument::Compact));

	QJsonObject object = document.object();
	const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
	int state = object.value(QString::fromLatin1("code")).toInt();

	if (state == 0)
	{
		creative_kernel::getcxNetworkManager()->setloginInfo(1, result.value(QString::fromLatin1("token")).toString(), result.value(QString::fromLatin1("userId")).toString());

		QSettings setting;
		setting.beginGroup("profile_setting");
		setting.setValue("login_token", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
		setting.setValue("login_userid", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
		setting.endGroup();
		loginSuccess(creative_kernel::getcxNetworkManager()->getLoginInfoToken(), creative_kernel::getcxNetworkManager()->getLoginInfoUserID());

	}
	else
	{
		creative_kernel::getcxNetworkManager()->setloginInfo(0, "", "");
		errorMsg = object.value(QString::fromLatin1("msg")).toString();

		QSettings setting;
		setting.beginGroup("profile_setting");
		setting.setValue("login_token", "");
		setting.setValue("login_userid", "");
		setting.setValue("login_auto", "0");
		setting.endGroup();

		loginFailed(errorMsg);
	}

}

void CrealityCloudUI::funcGetVerificationCode(const std::string& response)
{
	QString errorMsg = "";
	QJsonParseError error;
	const QJsonDocument document = QJsonDocument::fromJson(response.c_str(), &error);
	if (error.error != QJsonParseError::NoError)
	{
		qWarning() << "[funcGetVerificationCode QJsonDocument]" << error.errorString() << "\n";
		loginFailed("Network Error");
		return;
	}
	QString strJson(document.toJson(QJsonDocument::Compact));

	QJsonObject object = document.object();
	const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
	int state = object.value(QString::fromLatin1("code")).toInt();

	if (state == 0)
	{
		//success
	}
	else
	{
		loginFailed(errorMsg);
	}
}

void CrealityCloudUI::funcGetGCodeListResult(const std::string& response, int page)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response.c_str(), &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));

		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
		QMetaObject::invokeMethod(pUserInfoDlg, "setMyGcodeList", Q_ARG(QVariant, QVariant::fromValue(strJson)), Q_ARG(QVariant, QVariant::fromValue(page)));
	}
}

void CrealityCloudUI::funcDeleteGCodeResult(const std::string& response, QString deleteGcodeId)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response.c_str(), &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));

		QJsonObject object = document.object();
		const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
		const QJsonArray list = result.value(QString::fromLatin1("list")).toArray();
		int state = object.value(QString::fromLatin1("code")).toInt();

		if (state == 0)
		{
			QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
			QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
			QMetaObject::invokeMethod(pUserInfoDlg, "deleteGCodeResult", Q_ARG(QVariant, QVariant::fromValue(deleteGcodeId)));
		}
	}
}

void CrealityCloudUI::funcGetModelListResult(const std::string& response, QString getModelListPage)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response.c_str(), &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));
		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
		QMetaObject::invokeMethod(pUserInfoDlg, "setMyModelList", Q_ARG(QVariant, QVariant::fromValue(strJson)), Q_ARG(QVariant, QVariant::fromValue(getModelListPage)));
	}
}

void CrealityCloudUI::funcDeleteModelResult(const std::string& response, QString deleteModelId)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response.c_str(), &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));

		QJsonObject object = document.object();
		const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
		const QJsonArray list = result.value(QString::fromLatin1("list")).toArray();
		int state = object.value(QString::fromLatin1("code")).toInt();

		if (state == 0)
		{
			QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
			QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
			QMetaObject::invokeMethod(pUserInfoDlg, "deleteModelResult", Q_ARG(QVariant, QVariant::fromValue(deleteModelId)));
		}
	}
}

void CrealityCloudUI::funcImportModelResult(int state, int type, std::string& downloadPath, bool needImport)
{
	if (state != 0)
	{
		if (type != 2)
		{
			modelLibraryDownLoadFailed();
		}
		else
		{
			myModelDownloadFailed();
		}
		return;
	}
	if (!needImport)
	{
		if (type != 2)
		{
			modelLibraryDownLoadSuccess();
		}
		else
		{
			myModelDownloadSuccess();
		}
		CXFILE.setLastSaveFileName(downloadPath.c_str());
		CXFILE.openLastSaveFolder();
		return;
	}
	QDir dir(downloadPath.c_str());
	QStringList fileNames;;
	for (const auto filename : dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden))
	{
		fileNames.push_back(QString(downloadPath.c_str()) + filename);
	}
	CXFILE.openWithNames(fileNames);

	if (type != 2)
	{
		modelLibraryDownLoadSuccess();
	}
	else
	{
		myModelDownloadSuccess();
	}
}

void CrealityCloudUI::funcImportMyModelResult(int state, std::string& filename, std::string& extractPath)
{
	if (state == 0)
	{
		QStringList fileList = JlCompress::extractDir(filename.c_str(), extractPath.c_str());

		CXFILE.openWithNames(fileList);
		myModelDownloadSuccess();
	}
	else {
		myModelDownloadFailed();

	}
}

void CrealityCloudUI::funcDownLoadGCodeResult(const QByteArray& response)
{
	QString path = getCanWriteFolder() + "/myGcodes/";
	QDir tempDir;
	path = path + importGcodeId;
	if (!tempDir.exists(path))
	{
		tempDir.mkpath(path);
	}

	QString fileName = path + "/" + importGcodeName + ".gcode" + ".gz";
	QFile file;
	file.setFileName(fileName);
	if (file.open(QIODevice::WriteOnly))
	{
		file.write(response);
		file.close();
	}
	else
	{
		qDebug() << "file open failed";
	}

	QString gcodeFilePath = fileName.left(fileName.lastIndexOf("."));
	QByteArray array = fileName.toLocal8Bit();
	const char* str = array.data();
	gzFile gzfile = gzopen(str, "rb");
	if (gzfile == NULL)
	{
		return;
	}
	int len = 0;
	char buf[1024];
	QByteArray fileArray;
	while (len = gzread(gzfile, buf, 1024))
	{
		fileArray.append((const char*)buf, len);
	}
	gzclose(gzfile);

	QFile filePath(gcodeFilePath);
	QByteArray gzipBateArray = "";
	if (filePath.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		filePath.write(fileArray);
	}
	filePath.close();

	CXFILE.openWithName(gcodeFilePath);
	myModelDownloadSuccess();
	importGcodeStatus = false;
}

void CrealityCloudUI::funcGetCategoryList(const std::string& response)
{
	if (!response.empty()) {
		QString strJson(response.c_str());
		int state = 0;

		if (state == 0)
		{
			QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
			QObject* pModelDlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
			QMetaObject::invokeMethod(pModelDlg, "setModelTypeListBtn", Q_ARG(QVariant, QVariant::fromValue(strJson)));
		}
	}
	else {
		//get Category List then get again
		onGetCategoryList(7);
	}
}

void CrealityCloudUI::funcGetRecommendModelLibraryListResult(const std::string& response, QString getModelLibraryListPage)
{
	if (!response.empty()) {
		QString strJson(response.c_str());

		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pModelDlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
		QMetaObject::invokeMethod(pModelDlg, "setModelRecommendLibraryList", Q_ARG(QVariant, QVariant::fromValue(strJson)), Q_ARG(QVariant, QVariant::fromValue(getModelLibraryListPage)));
	}
}

void CrealityCloudUI::funcGetModelLibraryListResult(const std::string& response, QString getModelLibraryListPage)
{
	if (!response.empty()) {
		QString strJson(response.c_str());

		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pModelDlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
		QMetaObject::invokeMethod(pModelDlg, "setModelLibraryList", Q_ARG(QVariant, QVariant::fromValue(strJson)), Q_ARG(QVariant, QVariant::fromValue(getModelLibraryListPage)));
	}
}

void CrealityCloudUI::funcGetModelGroupDetailResult(const std::string& response, int type, bool needDownload, const std::string& downloadPath, bool needImport)
{
	QString strJson(response.c_str());
	if (!needDownload)	{

		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pModelDlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
		QMetaObject::invokeMethod(pModelDlg, "setModelDetailInfo", Q_ARG(QVariant, QVariant::fromValue(strJson)));
		return;
	}
	creative_kernel::getcxNetworkManager()->downloadAllModel(response, std::bind(
		&CrealityCloudUI::funcImportModelResult, this, std::placeholders::_1, type, downloadPath, needImport), downloadPath);
}

void CrealityCloudUI::funcGetModelGroupInfomationResult(const std::string& response)
{
	QSettings setting;
	setting.beginGroup("profile_setting");
	QString strStartType = setting.value("service_type", "-1").toString();
	setting.endGroup();
	QString clipBoardContent = "http://model-dev.crealitygroup.com/model-detail/";
#ifdef CLOUD_BETA_URL
	if (strStartType == "0")
	{
		clipBoardContent = "https://www.crealitycloud.cn/model-detail/";
	}
	else if (strStartType == "1")
	{
		clipBoardContent = "https://www.crealitycloud.com/model-detail/";
	}
#endif

	if (!response.empty()) {
		QString strJson(response.c_str());

		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pModelDlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
		QMetaObject::invokeMethod(pModelDlg, "setModelGroupInfo", Q_ARG(QVariant, QVariant::fromValue(strJson)), Q_ARG(QVariant, QVariant::fromValue(clipBoardContent)));
	}
}

void CrealityCloudUI::funcSearchModelListResult(const std::string& response, int page)
{
	if (!response.empty()) {
		QString strJson(response.c_str());

		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pModelDlg = pTopBar->findChild<QObject*>("ModelLibraryDlg");
		QMetaObject::invokeMethod(pModelDlg, "setModelSearchList", Q_ARG(QVariant, QVariant::fromValue(strJson)), Q_ARG(QVariant, QVariant::fromValue(page)));
	}
}

void CrealityCloudUI::funcImportDetailItemResult(int state, std::string fileName)
{
	if (state != 0)
	{
		modelLibraryDownLoadFailed();
		return;
	}
	if (downDetailItemType == 1)
	{
		downDetailItemType = 0;
		modelLibraryDownLoadSuccess();
		CXFILE.setLastSaveFileName(fileName.c_str());
		CXFILE.openLastSaveFolder();
		return;
	}
	CXFILE.openWithName(fileName.c_str());
	modelLibraryDownLoadSuccess();
}

void CrealityCloudUI::funcGetFavoritedListResult(const std::string& response, const QString& getFavoritedListPage)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response.c_str(), &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));
		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
		QMetaObject::invokeMethod(pUserInfoDlg, "setFavoritedList", Q_ARG(QVariant, QVariant::fromValue(strJson)), Q_ARG(QVariant, QVariant::fromValue(getFavoritedListPage)));
	}
}

void CrealityCloudUI::funcCancelFavoriteModelResult(const std::string& response, int favoriteModelType, QString cancelFavoriteModelId)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response.c_str(), &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));
		QJsonObject object = document.object();
		const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
		const QJsonArray list = result.value(QString::fromLatin1("list")).toArray();
		int state = object.value(QString::fromLatin1("code")).toInt();

		if (state == 0)
		{
			if (favoriteModelType == 1) {
				QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
				QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
				QMetaObject::invokeMethod(pUserInfoDlg, "cancelFavoritedResult", Q_ARG(QVariant, QVariant::fromValue(cancelFavoriteModelId)));
			}
			else if (favoriteModelType == 2)
			{
				onGetModelGroupInfomation(cancelFavoriteModelId);
			}
			
		}
	}
}

void CrealityCloudUI::funcGetPrinterTypeListResult(const std::string& response)
{
	if (!response.empty()) {
		QString strJson(response.c_str());
		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
		QMetaObject::invokeMethod(pUserInfoDlg, "setPrinterImageMap", Q_ARG(QVariant, QVariant::fromValue(strJson)));

		QObject* pPrintControlInfoDlg = pTopBar->findChild<QObject*>("PrintControlInfoDlg");
		QMetaObject::invokeMethod(pPrintControlInfoDlg, "setPrinterModelInfo", Q_ARG(QVariant, QVariant::fromValue(strJson)));
	}
}

void CrealityCloudUI::funcGetOnwerDeviceListResult(const std::string& response)
{
	QString strJson(response.c_str());
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
	QMetaObject::invokeMethod(pUserInfoDlg, "setSerDeviceData", Q_ARG(QVariant, QVariant::fromValue(strJson)));

	QObject* pPrintControlDlg = pTopBar->findChild<QObject*>("PrintControlInfoDlg");
	QMetaObject::invokeMethod(pPrintControlDlg, "setServerDeviceData", Q_ARG(QVariant, QVariant::fromValue(strJson)));

	QObject* pPrintingControlDlg = pTopBar->findChild<QObject*>("PrintingControlInfoDlg");
	QMetaObject::invokeMethod(pPrintingControlDlg, "setServerPrintingDeviceData", Q_ARG(QVariant, QVariant::fromValue(strJson)));

	if (m_queryEquipmentTask == nullptr)
	{
		m_queryEquipmentTask = new QueryEquipmentInfoTask(this);
		m_queryEquipmentTask->start();
	}
	m_queryEquipmentTask->setEquipmentInfo(response);
}

void CrealityCloudUI::funcGetGCodeListPrintControlResult(const QByteArray& response)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));

		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("PrintControlInfoDlg");
		QMetaObject::invokeMethod(pUserInfoDlg, "insertMyGcodeList", Q_ARG(QVariant, QVariant::fromValue(strJson)));
	}
}

void CrealityCloudUI::funcGetStartPrintGcodePrintIdResult(const int& state, const std::string& startPrintGcodePrintId, const QString& startPrintGcodeUrl, const QString& startPrintGcodeTbId)
{
	if (state == 0)
	{
		qDebug() << "startPrint success";

		if (startPrintGcodeType == 1)//old iot
		{
			getAliyunInfoStartPrintGcode();
		}
		else if (startPrintGcodeType == 2) //new iot
		{
			QString info = QString("{\"printId\":\"%1\", \"print\":\"%2\"}").arg(startPrintGcodePrintId.c_str()).arg(startPrintGcodeUrl);
			QString strContent = QString("{\"method\": \"set\", \"params\": %1}").arg(info);
			creative_kernel::getcxNetworkManager()->setDeviceDesiredProperty_NewIOT(strContent.toStdString(), startPrintGcodeTbId.toStdString(), std::bind(
				&CrealityCloudUI::funcSetDeviceDesiredProperty_NewIOT, this, std::placeholders::_1));
		}

	}
	else {
		startPrintGcodeStatus = false;
	}
}

void CrealityCloudUI::funcSetDeviceDesiredProperty_NewIOT(const std::string& response)
{
	startPrintGcodeStatus = false;
}

void CrealityCloudUI::funcGetAliyunInfoStartPrintGcodeResult(const QByteArray& response)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));

		QJsonObject object = document.object();
		const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
		const QJsonObject aliyunInfo = result.value(QString::fromLatin1("aliyunInfo")).toObject();
		int state = object.value(QString::fromLatin1("code")).toInt();

		if (state == 0)
		{
			QString accessKeyId = aliyunInfo.value(QString::fromLatin1("accessKeyId")).toString();
			QString secretAccessKey = aliyunInfo.value(QString::fromLatin1("secretAccessKey")).toString();
			QString sessionToken = aliyunInfo.value(QString::fromLatin1("sessionToken")).toString();

			QMap<QString, QString> para;
			//public
			QString timeTmp = currentTimeToUTC();
			para.insert("Action", "SetDeviceDesiredProperty");
			para.insert("Format", "JSON");
			para.insert("Version", "2018-01-20");
			para.insert("SignatureMethod", "HMAC-SHA1");
			para.insert("SignatureNonce", QString::number(randTime()));
			para.insert("SignatureVersion", "1.0");
			para.insert("AccessKeyId", accessKeyId);
			para.insert("SecurityToken", percentEncodeToken(percentEncodeUrl(sessionToken)));
			para.insert("Timestamp", percentEncodeToken(percentEncodeUrl(timeTmp)));
			para.insert("RegionId", "cn-shanghai");

			//request
			para.insert("ProductKey", startPrintGcodeProductKey);
			para.insert("DeviceName", startPrintGcodeProductName);
			para.insert("IotId", startPrintGcodeIotId);
			QString strContent = QString("{\"print\":\"%1\",\"printId\":\"%2\"}").arg(startPrintGcodeUrl).arg(startPrintGcodePrintId);
			para.insert("Items", percentEncodeToken(percentEncodeUrl(strContent)));
			para.insert("Versions", percentEncodeToken(percentEncodeUrl("{}")));

			QString signString = generate("GET", para, secretAccessKey);

			para.insert("Signature", signString);
			para.insert("SecurityToken", sessionToken);
			para.insert("Timestamp", timeTmp);
			para.insert("Items", strContent);
			para.insert("Versions", "{}");

			QString strurl = "https://iot.cn-shanghai.aliyuncs.com/?" + generateQueryString(para, true);

			std::function<void(QByteArray)> call_back = std::bind(
				&CrealityCloudUI::funcSetDeviceDesiredPropertyStart, this, std::placeholders::_1);
			QMap<QString, QString> data;
			data.clear();
			data.insert("__REQUEST_URl__", strurl);
			data.insert("__REQUEST_TYPE__", "3");
			creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
		}
		else {
			startPrintGcodeStatus = false;
		}
	}
	else {
		startPrintGcodeStatus = false;
	}
}

void CrealityCloudUI::funcSetDeviceDesiredPropertyStart(const QByteArray& response)
{
	startPrintGcodeStatus = false;
}

void CrealityCloudUI::funcSetDeviceDesiredProperty_NewIOTControlPrintGcode(const std::string& response)
{
	QString strJson(response.c_str());
}

void CrealityCloudUI::funcGetAliyunInfoControlPrintGcodeResult(const QByteArray& response)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));

		QJsonObject object = document.object();
		const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
		const QJsonObject aliyunInfo = result.value(QString::fromLatin1("aliyunInfo")).toObject();
		int state = object.value(QString::fromLatin1("code")).toInt();

		if (state == 0)
		{
			QString accessKeyId = aliyunInfo.value(QString::fromLatin1("accessKeyId")).toString();
			QString secretAccessKey = aliyunInfo.value(QString::fromLatin1("secretAccessKey")).toString();
			QString sessionToken = aliyunInfo.value(QString::fromLatin1("sessionToken")).toString();

			QString strurl = getSetDeviceDesiredPropertyUrl(accessKeyId, sessionToken, secretAccessKey, controlPrintGcodeProductName, controlPrintGcodeProductKey, controlPrintGcodeIotId, controlPrintGcodeInfo);

			std::function<void(QByteArray)> call_back = std::bind(
				&CrealityCloudUI::funcSetDeviceDesiredProperty, this, std::placeholders::_1);
			QMap<QString, QString> data;
			data.clear();
			data.insert("__REQUEST_URl__", strurl);
			data.insert("__REQUEST_TYPE__", "3");
			creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
		}
		else {
		}
	}
	else {
	}
}

void CrealityCloudUI::funcSetDeviceDesiredProperty(const QByteArray& response)
{
}

void CrealityCloudUI::funcSetDeviceDesiredProperty_NewIOTControlPrintTempSpeed(const std::string& response)
{
	controlPrintTempSpeedStatus = false;
}

void CrealityCloudUI::funcGetAliyunInfoControlPrintTempSpeedResult(const QByteArray& response)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));

		QJsonObject object = document.object();
		const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
		const QJsonObject aliyunInfo = result.value(QString::fromLatin1("aliyunInfo")).toObject();
		int state = object.value(QString::fromLatin1("code")).toInt();

		if (state == 0)
		{
			QString accessKeyId = aliyunInfo.value(QString::fromLatin1("accessKeyId")).toString();
			QString secretAccessKey = aliyunInfo.value(QString::fromLatin1("secretAccessKey")).toString();
			QString sessionToken = aliyunInfo.value(QString::fromLatin1("sessionToken")).toString();

			QString strurl = getSetDeviceDesiredPropertyUrl(accessKeyId, sessionToken, secretAccessKey, controlPrintTempSpeedProductName, controlPrintTempSpeedProductKey, controlPrintTempSpeedIotId, controlPrintTempSpeedInfo, controlPrintTempSpeedValue);

			std::function<void(QByteArray)> call_back = std::bind(
				&CrealityCloudUI::funcSetDeviceDesiredPropertyTempSpeedResult, this, std::placeholders::_1);
			QMap<QString, QString> data;
			data.clear();
			data.insert("__REQUEST_URl__", strurl);
			data.insert("__REQUEST_TYPE__", "3");
			creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
		}
		else {
			controlPrintTempSpeedStatus = false;
		}
	}
	else {
		controlPrintTempSpeedStatus = false;
	}
}

void CrealityCloudUI::funcSetDeviceDesiredPropertyTempSpeedResult(const QByteArray& response)
{
	controlPrintTempSpeedStatus = false;
}

void CrealityCloudUI::funcGetUpdatePreviewImage(const QByteArray& response)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));

		QJsonObject object = document.object();
		const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
		const QJsonObject aliyunInfo = result.value(QString::fromLatin1("aliyunInfo")).toObject();
		int state = object.value(QString::fromLatin1("code")).toInt();

		if (state == 0)
		{
			const QJsonObject gcodeinfo = result.value(QString::fromLatin1("gcodeInfo")).toObject();
			QString previewImage = gcodeinfo.value(QString::fromLatin1("thumbnail")).toString();

			const QJsonObject content = gcodeinfo.value(QString::fromLatin1("content")).toObject();
			int totallayers = content.value(QString::fromLatin1("layerCount")).toInt();

			QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
			QMetaObject::invokeMethod(pTopBar, "setPrintPreviewImage", Q_ARG(QVariant, QVariant::fromValue(previewImage)), Q_ARG(QVariant, QVariant::fromValue(totallayers)), Q_ARG(QVariant, QVariant::fromValue(updatePreviewImageDeviceId)));
			updatePreviewImageStatus = false;
		}
		else {
			updatePreviewImageStatus = false;
		}
	}
	else {
		updatePreviewImageStatus = false;
	}
}

void CrealityCloudUI::funcGetPrintingGcodeInfoResult(const QByteArray& response)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));

		QJsonObject object = document.object();
		const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
		const QJsonObject aliyunInfo = result.value(QString::fromLatin1("aliyunInfo")).toObject();
		int state = object.value(QString::fromLatin1("code")).toInt();

		if (state == 0)
		{
			const QJsonObject gcodeinfo = result.value(QString::fromLatin1("gcodeInfo")).toObject();
			QString gcodeId = gcodeinfo.value(QString::fromLatin1("id")).toString();

			QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
			QObject* pPrintControlInfoDlg = pTopBar->findChild<QObject*>("PrintControlInfoDlg");
			QMetaObject::invokeMethod(pPrintControlInfoDlg, "setCurPrintingGcodeInfo", Q_ARG(QVariant, QVariant::fromValue(gcodeId)));
		}
	}
}

void CrealityCloudUI::funcGetAliyunInfoInitCameraLicenseResult(const QByteArray& response)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));

		QJsonObject object = document.object();
		const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
		const QJsonObject aliyunInfo = result.value(QString::fromLatin1("aliyunInfo")).toObject();
		int state = object.value(QString::fromLatin1("code")).toInt();

		if (state == 0)
		{
			QString accessKeyId = aliyunInfo.value(QString::fromLatin1("accessKeyId")).toString();
			QString secretAccessKey = aliyunInfo.value(QString::fromLatin1("secretAccessKey")).toString();
			QString sessionToken = aliyunInfo.value(QString::fromLatin1("sessionToken")).toString();

			QMap<QString, QString> para;
			//public
			QString timeTmp = currentTimeToUTC();
			para.insert("Action", "SetDeviceDesiredProperty");
			para.insert("Format", "JSON");
			para.insert("Version", "2018-01-20");
			para.insert("SignatureMethod", "HMAC-SHA1");
			para.insert("SignatureNonce", QString::number(randTime()));
			para.insert("SignatureVersion", "1.0");
			para.insert("AccessKeyId", accessKeyId);
			para.insert("SecurityToken", percentEncodeToken(percentEncodeUrl(sessionToken)));
			para.insert("Timestamp", percentEncodeToken(percentEncodeUrl(timeTmp)));
			para.insert("RegionId", "cn-shanghai");

			//request
			para.insert("ProductKey", initCameraLicenseProduceKey);
			para.insert("DeviceName", initCameraLicenseProductName);
			para.insert("IotId", initCameraLicenseIotId);
			QString syInitString = "EBGCEBBBKLJDHFJFEAGFENENHLMMGINMGDEJAECLBEILLLLACFBFCBOMHBLNJAKLADMOLFDBOBMKAMCAJGNMIDAJ";
			QString strContent = QString("{\"DIDString\":\"%1\",\"APILicense\":\"%2\", \"InitString\":\"%3\"}").arg(initCameraLicenseDid).arg(initCameraLicenseLicense).arg(syInitString);
			para.insert("Items", percentEncodeToken(percentEncodeUrl(strContent)));
			para.insert("Versions", percentEncodeToken(percentEncodeUrl("{}")));

			QString signString = generate("GET", para, secretAccessKey);

			para.insert("Signature", signString);
			para.insert("SecurityToken", sessionToken);
			para.insert("Timestamp", timeTmp);
			para.insert("Items", strContent);
			para.insert("Versions", "{}");

			QString strurl = "https://iot.cn-shanghai.aliyuncs.com/?" + generateQueryString(para, true);

			std::function<void(QByteArray)> call_back = std::bind(
				&CrealityCloudUI::funcInitCameraLicenseResult, this, std::placeholders::_1);
			QMap<QString, QString> data;
			data.clear();
			data.insert("__REQUEST_URl__", strurl);
			data.insert("__REQUEST_TYPE__", "3");
			creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
		}
		else {
			initCameraLicenseStatus = false;
		}
	}
	else {
		initCameraLicenseStatus = false;
	}
}

void CrealityCloudUI::funcInitCameraLicenseResult(const QByteArray& response)
{
	initCameraLicenseStatus = false;
}

void CrealityCloudUI::funcGetAliyunInfoUpdateDeviceData(const QByteArray& response)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));

		QJsonObject object = document.object();
		const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
		const QJsonObject aliyunInfo = result.value(QString::fromLatin1("aliyunInfo")).toObject();
		int state = object.value(QString::fromLatin1("code")).toInt();

		if (state == 0)
		{
			QString accessKeyId = aliyunInfo.value(QString::fromLatin1("accessKeyId")).toString();
			QString secretAccessKey = aliyunInfo.value(QString::fromLatin1("secretAccessKey")).toString();
			QString sessionToken = aliyunInfo.value(QString::fromLatin1("sessionToken")).toString();

			QMap<QString, QString> para;
			//public
			QString timeTmp = currentTimeToUTC();
			para.insert("Action", "QueryDevicePropertyStatus");
			para.insert("Format", "JSON");
			para.insert("Version", "2018-01-20");
			para.insert("SignatureMethod", "HMAC-SHA1");
			para.insert("SignatureNonce", QString::number(randTime()));
			para.insert("SignatureVersion", "1.0");
			para.insert("AccessKeyId", accessKeyId);
			para.insert("SecurityToken", percentEncodeToken(percentEncodeUrl(sessionToken)));
			para.insert("Timestamp", percentEncodeToken(percentEncodeUrl(timeTmp)));
			para.insert("RegionId", "cn-shanghai");

			//request
			para.insert("ProductKey", updateDeviceDataProductKey);
			para.insert("DeviceName", updateDeviceDataProductName);
			para.insert("IotId", updateDeviceDataIotId);

			QString signString = generate("GET", para, secretAccessKey);

			para.insert("Signature", signString);
			para.insert("SecurityToken", sessionToken);
			para.insert("Timestamp", timeTmp);

			QString strurl = "https://iot.cn-shanghai.aliyuncs.com/?" + generateQueryString(para, true);

			std::function<void(QByteArray)> call_back = std::bind(
				&CrealityCloudUI::funcUpdateDeviceDataResult, this, std::placeholders::_1);
			QMap<QString, QString> data;
			data.clear();
			data.insert("__REQUEST_URl__", strurl);
			data.insert("__REQUEST_TYPE__", "3");
			creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
		}
		else {
		}
	}
	else {
	}
}

void CrealityCloudUI::funcUpdateDeviceDataResult(const QByteArray& response)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));
		setDeviceInformation(strJson.toStdString(), updateDeviceDataProductName.toStdString(), false);
	}
}

void CrealityCloudUI::funcUpdateDeviceDataNewIotResult(const std::string& response)
{
	if (!response.empty()) {
		QString strJson(response.c_str());
		setDeviceInformation(strJson.toStdString(), updateDeviceDataProductName.toStdString(), true);
	}
}

void CrealityCloudUI::funcGetCloudSliceListResult(const QByteArray& response)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));

		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
		QMetaObject::invokeMethod(pUserInfoDlg, "setCloudSliceList", Q_ARG(QVariant, QVariant::fromValue(strJson)), Q_ARG(QVariant, QVariant::fromValue(getCloudSliceListPage)));
	}

	getCloudSliceListStatus = false;
}

void CrealityCloudUI::funcGetRecommendModelAlwaysShowDlgResult(const std::string& response)
{
	//let operate bar to pick
//    AbstractKernelUI::getSelf()->switchPickMode();

	if (!response.empty()) {
		QString strJson(response.c_str());
		QObject* mainObj = AbstractKernelUI::getSelf()->getUI("UIAppWindow");
		QObject* pModelAlwaysShow = mainObj->findChild<QObject*>("idModelAlwaysShowDlg");
		QMetaObject::invokeMethod(pModelAlwaysShow, "setAlwaysShowItemData", Q_ARG(QVariant, QVariant::fromValue(strJson)));
        QMetaObject::invokeMethod(mainObj, "modelAlwaysPopReallyShow");
	}
}

void CrealityCloudUI::funcGetPurchasedListResult(const QByteArray& response)
{
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));
		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
		QMetaObject::invokeMethod(pUserInfoDlg, "setPurchasedList", Q_ARG(QVariant, QVariant::fromValue(strJson)), Q_ARG(QVariant, QVariant::fromValue(getPurchasedListPage)));
	}
	getPurchasedListStatus = false;
}

void CrealityCloudUI::funconGetClusterGcodeListResult(const std::string& response, const int& page)
{
	if (!response.empty())
	{
		QString strJson(response.c_str());

		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("GroupPrintDlg");
		QMetaObject::invokeMethod(pUserInfoDlg, "setClusterGcodeListData", Q_ARG(QVariant, QVariant::fromValue(strJson)), Q_ARG(QVariant, QVariant::fromValue(page)));
	}
}

void CrealityCloudUI::funcAddClusterTaskResult(const bool& response)
{
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("GroupPrintDlg");
	QMetaObject::invokeMethod(pUserInfoDlg, "addClusterPrintTaskRet", Q_ARG(QVariant, QVariant::fromValue(true)));
}

void CrealityCloudUI::funcClusterConformIdleResult(const std::string& response)
{
	QString strJson(response.c_str());
}

void CrealityCloudUI::funcClusterCancelPrintTaskResult(const std::string& response)
{
	QString strJson(response.c_str());
}

void CrealityCloudUI::funcClusterTaskReportResult(const std::string& response)
{
	if (!response.empty()) {
		QString strJson(response.c_str());

		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");

		QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("GroupPrintDlg");
		QMetaObject::invokeMethod(pUserInfoDlg, "setTaskPrintReportData", Q_ARG(QVariant, QVariant::fromValue(strJson)));
	}
}

void CrealityCloudUI::funcClusterGcodeSliceTypeListResult(const std::string& response)
{
	if (!response.empty()) {
		QString strJson(response.c_str());

		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");

		QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("GroupPrintDlg");
		QMetaObject::invokeMethod(pUserInfoDlg, "setClusterGcodeSliteTypeList", Q_ARG(QVariant, QVariant::fromValue(strJson)));
	}
}

void CrealityCloudUI::funcClsterResetGcodeSliceTypeResult(const QByteArray& response)
{
	//update
	onGetClusterGcodeList(1, 100);
}

void CrealityCloudUI::funcGetServerRequestOverseas(const QByteArray& response)
{
	QString urlStr = "http://model-dev.crealitygroup.com";
	QString strStartType = "0";
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));
		QJsonObject object = document.object();
		int state = object.value(QString::fromLatin1("code")).toInt();

		if (state == 0)
		{
			const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
			QString qstrUrl = result.value(QString::fromLatin1("apiUrl")).toString();
			QString qstrCountry = result.value(QString::fromLatin1("country")).toString();

			QSettings setting;
			setting.beginGroup("profile_setting");
			strStartType = setting.value("service_type", "-1").toString();
			if (strStartType == "-1")
			{
				if (qstrCountry == "China")
				{
					strStartType = "0";
					setting.setValue("service_type", "0");
				}
				else
				{
					strStartType = "1";
					setting.setValue("service_type", "1");
				}

			}
			setting.endGroup();

			if (strStartType == "0")
			{
#ifdef CLOUD_BETA_URL
				urlStr = "https://www.crealitycloud.cn";
#endif
			}
			else if (strStartType == "1")
			{
#ifdef CLOUD_BETA_URL
				urlStr = "https://www.crealitycloud.com";
#endif
			}
		}
		else
		{
			QString errorMsg = object.value(QString::fromLatin1("msg")).toString();
		}

		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* plogindlg = pTopBar->findChild<QObject*>("LoginDlg");
		QObject* pServerSelectDlg = pTopBar->findChild<QObject*>("ServerSelectDlg");
		QMetaObject::invokeMethod(pServerSelectDlg, "setServerType", Q_ARG(QVariant, strStartType));
		QMetaObject::invokeMethod(plogindlg, "setWebUrl", Q_ARG(QVariant, urlStr));
		QMetaObject::invokeMethod(plogindlg, "setServerType", Q_ARG(QVariant, strStartType));
	}
}

void CrealityCloudUI::funcGetServerRequestChina(const QByteArray& response)
{
	QString urlStr = "http://model-dev.crealitygroup.com";
	QString strStartType = "0";
	QJsonParseError json_error;
	QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
	if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
		QString strJson(document.toJson(QJsonDocument::Compact));
		QJsonObject object = document.object();
		int state = object.value(QString::fromLatin1("code")).toInt();

		if (state == 0)
		{
			const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
			QString qstrUrl = result.value(QString::fromLatin1("apiUrl")).toString();
			QString qstrCountry = result.value(QString::fromLatin1("country")).toString();

			QSettings setting;
			setting.beginGroup("profile_setting");
			strStartType = setting.value("service_type", "-1").toString();
			if (strStartType == "-1")
			{
				if (qstrCountry == "China")
				{
					strStartType = "0";
					setting.setValue("service_type", "0");
				}
				else
				{
					strStartType = "1";
					setting.setValue("service_type", "1");
				}

			}
			setting.endGroup();

			if (strStartType == "0")
			{
#ifdef CLOUD_BETA_URL
				urlStr = "https://www.crealitycloud.cn";
#endif
			}
			else if (strStartType == "1")
			{
#ifdef CLOUD_BETA_URL
				urlStr = "https://www.crealitycloud.com";
#endif
			}
		}
		else
		{
			QString errorMsg = object.value(QString::fromLatin1("msg")).toString();
		}

		QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
		QObject* plogindlg = pTopBar->findChild<QObject*>("LoginDlg");
		QObject* pServerSelectDlg = pTopBar->findChild<QObject*>("ServerSelectDlg");
		QMetaObject::invokeMethod(pServerSelectDlg, "setServerType", Q_ARG(QVariant, strStartType));
		QMetaObject::invokeMethod(plogindlg, "setWebUrl", Q_ARG(QVariant, urlStr));
		QMetaObject::invokeMethod(plogindlg, "setServerType", Q_ARG(QVariant, strStartType));
	}
}

void CrealityCloudUI::getAliyunInfoStartPrintGcode()
{
	QString strContent = "{}";
	QString strurl = "/api/cxy/account/v2/getAliyunInfo";
	std::function<void(QByteArray)> call_back = std::bind(
		&CrealityCloudUI::funcGetAliyunInfoStartPrintGcodeResult, this, std::placeholders::_1);
	QMap<QString, QString> data;
	data.clear();
	data.insert("__REQUEST_URl__", strurl);
	data.insert("__REQUEST_TYPE__", "1");
	data.insert("__REQUEST_BODY__", strContent.toUtf8());
	data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
	data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
	creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
}

void CrealityCloudUI::getAliyunInfoControlPrintGcode()
{
	QString strContent = "{}";
	QString strurl = "/api/cxy/account/v2/getAliyunInfo";
	std::function<void(QByteArray)> call_back = std::bind(
		&CrealityCloudUI::funcGetAliyunInfoControlPrintGcodeResult, this, std::placeholders::_1);
	QMap<QString, QString> data;
	data.clear();
	data.insert("__REQUEST_URl__", strurl);
	data.insert("__REQUEST_TYPE__", "1");
	data.insert("__REQUEST_BODY__", strContent.toUtf8());
	data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
	data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
	creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
}

QString CrealityCloudUI::getSetDeviceDesiredPropertyUrl(QString accessKeyId, QString sessionToken, QString secretAccessKey, QString productName, QString productKey, QString iotId, QString info, int value)
{
	QMap<QString, QString> para;
	//public
	QString timeTmp = currentTimeToUTC();
	para.insert("Action", "SetDeviceDesiredProperty");
	para.insert("Format", "JSON");
	para.insert("Version", "2018-01-20");
	para.insert("SignatureMethod", "HMAC-SHA1");
	para.insert("SignatureNonce", QString::number(randTime()));
	para.insert("SignatureVersion", "1.0");
	para.insert("AccessKeyId", accessKeyId);
	para.insert("SecurityToken", percentEncodeToken(percentEncodeUrl(sessionToken)));
	para.insert("Timestamp", percentEncodeToken(percentEncodeUrl(timeTmp)));
	para.insert("RegionId", "cn-shanghai");

	//request
	para.insert("ProductKey", productKey);
	para.insert("DeviceName", productName);
	para.insert("IotId", iotId);
	QString strContent = "";
	if (info == "nozzletargettemp")
	{
		strContent = QString("{\"nozzleTemp2\":%1}").arg(value); //nozzle target temp
	}
	else if (info == "hotbedtargettemp")
	{
		strContent = QString("{\"bedTemp2\":%1}").arg(value); //hot bed target temp
	}
	else if (info == "printspeedtarget")
	{
		strContent = QString("{\"setFeedratePct\":%1}").arg(value);//print speed target
	}
	else if (info == "pause")
	{
		strContent = QString("{\"pause\":%1}").arg(1);
	}
	else if (info == "continue")
	{
		strContent = QString("{\"pause\":%1}").arg(0);
	}
	else if (info == "stop")
	{
		strContent = QString("{\"stop\":%1}").arg(1);
	}
	else if (info == "ReqPrinterPara")
	{
		strContent = QString("{\"ReqPrinterPara\":%1}").arg(value);
	}
	else if (info == "gcodeCmdG0X")
	{
		if (value == 0)
		{
			value = 0;
		}
		else {
			value = value / 100;
		}
		strContent = QString("{\"gcodeCmd\":\"G0 X%1\"}").arg(value);
	}
	else if (info == "gcodeCmdG0Y")
	{
		if (value == 0)
		{
			value = 0;
		}
		else {
			value = value / 100;
		}
		strContent = QString("{\"gcodeCmd\":\"G0 Y%1\"}").arg(value);
	}
	else if (info == "gcodeCmdG0Z")
	{
		if (value == 0)
		{
			value = 0;
		}
		else {
			value = value / 100;
		}
		strContent = QString("{\"gcodeCmd\":\"G0 Z%1\"}").arg(value);
	}
	else if (info == "gcodeCmdG28XY")
	{
		strContent = QString("{\"gcodeCmd\":\"G28 X Y\"}");
	}
	else if (info == "gcodeCmdG28Z")
	{
		strContent = QString("{\"gcodeCmd\":\"G28 Z\"}");
	}
	else if (info == "fanswitch")
	{
		strContent = QString("{\"fan\":%1}").arg(value);
	}
	para.insert("Items", percentEncodeToken(percentEncodeUrl(strContent)));
	para.insert("Versions", percentEncodeToken(percentEncodeUrl("{}")));

	QString signString = generate("GET", para, secretAccessKey);

	para.insert("Signature", signString);
	para.insert("SecurityToken", sessionToken);
	para.insert("Timestamp", timeTmp);
	para.insert("Items", strContent);
	para.insert("Versions", "{}");

	return "https://iot.cn-shanghai.aliyuncs.com/?" + generateQueryString(para, true);
}

void CrealityCloudUI::getAliyunInfoControlPrintTempSpeed()
{
	QString strContent = "{}";
	QString strurl = "/api/cxy/account/v2/getAliyunInfo";
	std::function<void(QByteArray)> call_back = std::bind(
		&CrealityCloudUI::funcGetAliyunInfoControlPrintTempSpeedResult, this, std::placeholders::_1);
	QMap<QString, QString> data;
	data.clear();
	data.insert("__REQUEST_URl__", strurl);
	data.insert("__REQUEST_TYPE__", "1");
	data.insert("__REQUEST_BODY__", strContent.toUtf8());
	data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
	data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
	creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
}

void CrealityCloudUI::getAliyunInfoInitCameraLicense()
{
	QString strContent = "{}";
	QString strurl = "/api/cxy/account/v2/getAliyunInfo";
	std::function<void(QByteArray)> call_back = std::bind(
		&CrealityCloudUI::funcGetAliyunInfoInitCameraLicenseResult, this, std::placeholders::_1);
	QMap<QString, QString> data;
	data.clear();
	data.insert("__REQUEST_URl__", strurl);
	data.insert("__REQUEST_TYPE__", "1");
	data.insert("__REQUEST_BODY__", strContent.toUtf8());
	data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
	data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
	creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
}

void CrealityCloudUI::getAliyunInfoUpdateDeviceData()
{
	QString strContent = QString("{}");
	QString strurl = "/api/cxy/account/v2/getAliyunInfo";
	std::function<void(QByteArray)> call_back = std::bind(
		&CrealityCloudUI::funcGetAliyunInfoUpdateDeviceData, this, std::placeholders::_1);
	QMap<QString, QString> data;
	data.clear();
	data.insert("__REQUEST_URl__", strurl);
	data.insert("__REQUEST_TYPE__", "1");
	data.insert("__REQUEST_BODY__", strContent.toUtf8());
	data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
	data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
	creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
}

void CrealityCloudUI::getServerRequestOverseas()
{
	QString strContent = QString("{}");
	QString strurl = QString("https://model-admin2.creality.com") + "/api/cxy/v2/common/getAddrress";
	std::function<void(QByteArray)> call_back = std::bind(
		&CrealityCloudUI::funcGetServerRequestOverseas, this, std::placeholders::_1);
	QMap<QString, QString> data;
	data.clear();
	data.insert("__REQUEST_URl__", strurl);
	data.insert("__REQUEST_TYPE__", "4");
	data.insert("__REQUEST_BODY__", strContent.toUtf8());
	creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
}

void CrealityCloudUI::getServerRequestChina()
{
	QString strContent = QString("{}");
	QString strurl = QString("https://model-admin.crealitygroup.com") + "/api/cxy/v2/common/getAddrress";
	std::function<void(QByteArray)> call_back = std::bind(
		&CrealityCloudUI::funcGetServerRequestChina, this, std::placeholders::_1);
	QMap<QString, QString> data;
	data.clear();
	data.insert("__REQUEST_URl__", strurl);
	data.insert("__REQUEST_TYPE__", "4");
	data.insert("__REQUEST_BODY__", strContent.toUtf8());
	creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
}

void CrealityCloudUI::slotLanguageChanged()
{
	onGetCategoryList(7);//update model type list
}

void CrealityCloudUI::onLogin()
{
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	QString param = "menu";
	QMetaObject::invokeMethod(pTopBar, "showLoginDialog", Q_ARG(QVariant, QVariant::fromValue(param)));
}

void CrealityCloudUI::onLogout()
{
	//logout
	creative_kernel::getcxNetworkManager()->setloginInfo(0, "", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
	QSettings setting;
	setting.beginGroup("profile_setting");
	setting.setValue("login_auto", "0");
	setting.setValue("login_token", "");
	setting.setValue("login_userid", "");
	setting.endGroup();
}

void CrealityCloudUI::loginUpdate()
{
	creative_kernel::getcxNetworkManager()->getQrLoginInfo(std::bind(
		&CrealityCloudUI::funcQrLogin, this, std::placeholders::_1, std::placeholders::_2));
}

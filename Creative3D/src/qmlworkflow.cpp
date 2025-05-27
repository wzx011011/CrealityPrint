#include "qmlworkflow.h"
#include <QtCore/QFileInfo>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlProperty>

#include "qtuserqml/gl/glquickitem.h"

#include "kernel/kernel.h"
#include "kernel/kernelui.h"
#include "kernel/builtinui.h"
#include "kernel/rendercenter.h"
#include "kernel/cxxglobal.h"

#include "kernel/tips.h"
#include "creativeplugincenter.h"
#include "qtusercore/module/systemutil.h"

#include "interface/eventinterface.h"
#include "interface/qmlinterface.h"
#include "interface/spaceinterface.h"

#include "qtuser3d/camera/cameracontroller.h"
#include "kernel/printerentityuse.h"
#include "kernel/reuseablecache.h"
#include "interface/modelinterface.h"
#include "qtusercore/string/resourcesfinder.h"
#include "buildinfo.h"
#include "inout/cxopenandsavefilemanager.h"
#include <QSettings>
#include "interface/reuseableinterface.h"
#include "interface/gadatainterface.h"
QMLWorkflow::QMLWorkflow(QObject* parent)
	:QObject(parent)
	, m_kernel(nullptr)
	, m_kernelUI(nullptr)
{
	APP_RESOURCES.addSearchPath("./resources/");
	APP_RESOURCES.addSearchPath(BIN_RESOURCES_DIR);

	m_kernel = getKernel();
	m_kernelUI = getKernelUI();
	m_renderCenter = getRenderCenter();

	m_cxxGlobal = new CXXGlobal(this);
	PrinterEntityUse* printer = new PrinterEntityUse();
	m_kernel->reuseableCache()->setPrinterEntity(printer);

	m_creativePluginCenter = new CreativePluginCenter(this);
	m_creativePluginCenter->load();

    QString currentVersion = QString("V%1.%2.%3.%4").arg(PROJECT_VERSION_MAJOR).arg(PROJECT_VERSION_MINOR).arg(PROJECT_VERSION_PATCH).arg(PROJECT_BUILD_ID);
    m_pUpdateManager = new UpdateManager(RemoteType::RemoteTypeBaiduYun, BUNDLE_NAME,"",currentVersion, APP_TYPE, BUILD_OS);

	disconnect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
	connect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));

}


QMLWorkflow::~QMLWorkflow()
{
    delete m_pUpdateManager;
    m_pUpdateManager=nullptr;
}

void QMLWorkflow::initialize()
{
	qDebug() << "workflow initialize";
	m_kernel->intialize();
	__initKernelData();

	m_kernelUI->registerEvent();
	registerBuiltin();

	m_creativePluginCenter->initialize();

	qmlEngine(this)->rootContext()->setContextProperty("CXXGlobal", QVariant::fromValue(m_cxxGlobal));
	qDebug() << "workflow initialize over";
	showSysMemory();

	m_kernelUI->visibleAll(false);//
	QSettings setting;	
	setting.beginGroup("themecolor_config");
	int nThemeType = setting.value("themecolor_config", -1).toInt();
	setting.endGroup();
	if (nThemeType == -1)
	{
		nThemeType = 0;
		setting.setValue("themecolor_config", nThemeType);
	}
	m_kernelUI->changeTheme(nThemeType);//by TCJ

	creative_kernel::setStartFinish(true);
}

void QMLWorkflow::__initKernelData()
{
	addResizeEventHandler(m_kernel->cameraController());
	addRightMouseEventHandler(m_kernel->cameraController());
	addMidMouseEventHandler(m_kernel->cameraController());
	addWheelEventHandler(m_kernel->cameraController());

	qtuser_3d::Box3D baseBox = getModelSpace()->baseBoundingBox();
	m_kernel->cameraController()->home(baseBox,1);
}


void QMLWorkflow::loadOver()
{
	qDebug() << "load over. deal open params";
	CXFILE.openWithParams(qApp->arguments());
	showSysMemory();
}

void QMLWorkflow::startWizard()
{
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
	if (pTopBar)
	{
		QSettings setting;
		setting.beginGroup("profile_setting");
		QString strStartType = setting.value("first_start", "0").toString();

		creative_kernel::sendDataToGA("Login", "start");

		QObject* pMachineCommbox = pTopBar->findChild<QObject*>("AddPrinterDlg");
		QQmlProperty::write(pMachineCommbox, "isFristStart", QVariant::fromValue(strStartType));

		if (strStartType == "0")
		{
			qInfo() << "startWizardComp first_start";
			QMetaObject::invokeMethod(pTopBar, "startFirstConfig");
			setting.setValue("first_start", "1");
		}
		else
		{
			QObject* uiappwindow = AbstractKernelUI::getSelf()->getUI("uiappwindow");
			if (uiappwindow)
			{
				QMetaObject::invokeMethod(uiappwindow, "letModelAlwaysPopShow");
			}
		}
		setting.endGroup();


		QSettings setting1;
		setting1.beginGroup("profile_setting");
		QString strAutoLogin = setting1.value("login_auto", "0").toString();
		QString strLoginToken = setting1.value("login_token", "").toString();
		QString strLoginUserid = setting1.value("login_userid", "").toString();
		QString strMailAddr = setting1.value("login_mail_accont", "").toString();
		QString strPhoneNum = setting1.value("login_mobile_accont", "").toString();
		QString strPhoneAreaCode = setting1.value("login_mobile_areaCode", "").toString();
		setting1.endGroup();
		QObject* plogindlg = pTopBar->findChild<QObject*>("LoginDlg");
		QMetaObject::invokeMethod(plogindlg, "setAccont", Q_ARG(QVariant, strMailAddr), Q_ARG(QVariant, strPhoneNum), Q_ARG(QVariant, strPhoneAreaCode));

		if (strAutoLogin == "1" && strLoginToken != "" && strLoginUserid != "")
		{
			//loginSuccess(strLoginToken, strLoginUserid);
			QMetaObject::invokeMethod(pTopBar, "getAutoLogin", Q_ARG(QVariant, strLoginToken), Q_ARG(QVariant, strLoginUserid));
		}
		// else {
			emit sigInitAdvanceParam();
		// }
	}
}

bool QMLWorkflow::checkUnsavedChanges()
{
	return true;
}

void QMLWorkflow::uninitialize()
{
	m_renderCenter->uninitialize();
	m_creativePluginCenter->uninitialize();
	m_kernel->uninitialize();
}

QObject* QMLWorkflow::getPluginByName(QString name)
{
    return (QObject*)(m_creativePluginCenter->getPluginByName(name));
}

GLQuickItem* QMLWorkflow::glQuickItem()
{
	return nullptr;
}

void QMLWorkflow::setGLQuickItem(GLQuickItem* glQuickItem)
{
	m_renderCenter->setGLQuickItem(glQuickItem);
	m_kernelUI->setRoot(glQuickItem);
	showSysMemory();
	qDebug() << "set GLQuickItem";
}

KernelUI* QMLWorkflow::kernelUI()
{
	return m_kernelUI;
}

QObject* QMLWorkflow::tips()
{
	return qmlTips();
}

QObject* QMLWorkflow::jobs()
{
	return qmlJobExecutor();
}
QObject* QMLWorkflow::updateManager()
{
    return m_pUpdateManager;
}

void QMLWorkflow::slotThemeChanged()
{
	QSettings setting;
	setting.beginGroup("themecolor_config");
	int nThemeType = setting.value("themecolor_config", 0).toInt();
	setting.endGroup();

	qtuser_3d::PrinterEntity* entity = getCachedPrinterEntity();
	entity->onThemeChanged(nThemeType);


	Qt3DRender::QEffect *modelEffect = getCachedModelEffect();

	QVector3D clearColor(0.27, 0.27, 0.27);
	if (nThemeType == 1) {
		clearColor = QVector3D(1.0, 1.0, 1.0);
	}

	Qt3DRender::QParameter* clearParam = nullptr;

	QVector<Qt3DRender::QParameter*> params = modelEffect->parameters();
	for (Qt3DRender::QParameter* param : params) {
		if (param->name() == "clearColor") {
			clearParam = param;
			break;
		}
	}

	if (clearParam) {
		clearParam->setValue(clearColor);
	} else {
		Qt3DRender::QParameter* param = new Qt3DRender::QParameter("clearColor", clearColor);
		modelEffect->addParameter(param);
	}
}

void QMLWorkflow::autoTask()
{
	///1.need to auto open file
	QStringList arguments = QCoreApplication::arguments();
	int modelCount = arguments.count();
	if (modelCount < 3)
	{
		return;
	}
	QString strExcu = arguments.at(1).toLower();
	if (strExcu == "-e")
	{
		for (int i = 2; i < modelCount; i++)
		{
			QString file = arguments.at(i);
			QFileInfo info(file);
			QString sufixx = info.suffix();
			if (sufixx.isEmpty()) continue;
			CXFILE.openWithName(file);
		}
		qDebug() << "arguments.at(1) " << strExcu;

	}

}





#include "localnetplugin.h"
#include "localnetcommand.h"

#include <QtQml/QQmlComponent>
#include <QQmlApplicationEngine>
#include <QSettings>
//#include "kernel/abstractkernel.h"
#include "interface/camerainterface.h"
#include "interface/selectorinterface.h"
#include "interface/spaceinterface.h"

#include "qtuser3d/camera/cameracontroller.h"
#include "qtuserqml/property/qmlpropertysetter.h"

#include "kernel/translator.h"
#include "kernel/kernelui.h"
#include <QtQml/QQmlContext>
#include "cuswifimanagermodel.h"
#include "kernel/abstractkernel.h"
#include "cuslistmodel.h"
#include "cusproxymodel.h"
#include <menu/ccommandlist.h>
using namespace creative_kernel;
using namespace qtuser_3d;
using namespace qtuser_qml;

LocalNetPlugin::LocalNetPlugin(QObject* parent)
	:QObject(parent)
	, m_root(nullptr)
    , m_localNetUI(nullptr)
    , m_localnetcomponent(nullptr)
    , m_localNetCmd(nullptr)
{
    CusWifiManagerModel* cwModel = new CusWifiManagerModel();
     QQmlApplicationEngine *engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();
     engine->rootContext()->setContextProperty("cwModel", cwModel);
     m_PrinterList = new CusListModel(this);
     CusProxyModel* proModel = new CusProxyModel(this);
     proModel->setSourceModel(m_PrinterList);
     engine->rootContext()->setContextProperty("printerListModel", proModel);
}

LocalNetPlugin::~LocalNetPlugin()
{
}

QString LocalNetPlugin::name()
{
	return "InfoPlugin";
}

QString LocalNetPlugin::info()
{
	return "";
}

void LocalNetPlugin::initialize()
{
//    m_root = AbstractKernelUI::getSelf()->getUI("uiappwindow");   // getUIRoot();
//    QQmlContext* context = qmlContext(m_root);
//    QQmlEngine* engine = qmlEngine(m_root);
//    //注册wifi和创想云模型
//    if (!m_localnetcomponent)
//	{
//        m_localnetcomponent = new QQmlComponent(engine, QUrl::fromLocalFile(":/localNet/CusPrinterListLocal.qml"), this);
//        m_localNetUI = m_localnetcomponent->create(context);
//	}
//    if(m_localNetUI)
//    {

//        qDebug()<<"localdlg item create success";
//        m_localNetUI->setObjectName("localWifiObj");
//        m_localNetUI->setParent(m_root);
//    }

//    if (!m_localNetCmd)
//        m_localNetCmd = new LocalNetCommand();
//    m_localNetCmd->setParent(this);

//    //AbstractKernelUI::getSelf()->addActionCommand(m_localNetCmd,"");
//    CCommandList::getInstance()->addActionCommad(m_localNetCmd, "");
}

void LocalNetPlugin::uninitialize()
{

}


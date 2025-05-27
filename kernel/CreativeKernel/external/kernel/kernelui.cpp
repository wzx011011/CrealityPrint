#include "kernel/kernelui.h"
#include "qtuserqml/plugin/toolcommandcenter.h"
#include "qtuserqml/plugin/toolcommand.h"
#include "qtuserqml/plugin/treemodel.h"
//#include "qtuserqml/plugin/cusModelListModel.h"
#include "slice/cusModelListModel.h"
#include "utils/meshloaderwrapper.h"
#include "qtusercore/module/systemutil.h"
#include "internal/menu/ccommandsdata.h"
#include "qtuser3d/event/eventsubdivide.h"
#include "kernel/kernelevent.h"
#include "interface/eventinterface.h"
#include "interface/loadinterface.h"

#include <QQmlProperty>

////2020-08-10 liuhong add for inout
#include "inout/cxopenandsavefilemanager.h"

#include <QtQml/QQmlEngine>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtCore/QSettings>
#include <QtCore/QDebug>
#include <Qt>
#include "kernel/translator.h"
#include "interface/uiinterface.h"
#include "interface/modelinterface.h"

#include "menu/ccommandlist.h"
#include "kernel/projectinfoui.h"

SINGLETON_IMPL(KernelUI)
KernelUI::KernelUI()
    : m_root(nullptr)
    , m_appWindow(nullptr)
    , m_leftToolbar(nullptr)
    ,m_rightPanel(nullptr)
    ,m_glMainView(nullptr)
    , m_leftToolbarModelList(nullptr)
    , m_meshLoaderWrapper(nullptr)
    , m_sliceUI(nullptr)
{
    // model list
    m_leftToolbarModelList = new qtuser_qml::ToolCommandCenter(this);

    m_ModelList = new CusModelListModel();
    m_modelList = new qtuser_qml::TreeModel(this);
    m_tableModelList = new TableModel(this);
    m_meshLoaderWrapper = new MeshLoaderWrapper(this);
    connect(m_meshLoaderWrapper, &MeshLoaderWrapper::sigSupportFiltersChanged, this, &KernelUI::slotMeshLoadFiltersChanged);

    m_meshLoadTypes.clear();
    m_meshLoadTypes += m_meshLoaderWrapper->supportFilters();

    //   m_supportOpenSaveTypes += typeList;

    m_translator = nullptr;
    m_currentVersion = "";

    setAddModelFun(modelfaceAddModel);
    setAddGroupFun(modelfaceAddGroup);
    setDelModelFun(modelfaceDelModel);

    m_appSettings = new creative_kernel::AppSetting();

    qDebug() << "kernelUI constructor over";
    showSysMemory();

    m_self = this;

    //m_sliceUI = new SliceUI(this);
}

KernelUI::~KernelUI()
{
    if (m_sliceUI != nullptr)
    {
        delete m_sliceUI;
        m_sliceUI = nullptr;
    }
    if (m_appSettings != nullptr)
    {
        delete m_appSettings;
        m_appSettings = nullptr;
    }
    if(m_ModelList)
    {
        delete(m_ModelList);
        m_ModelList = nullptr;
    }
}

void KernelUI::addModel(QObject* model, QObject* group)
{
    //
}

void KernelUI::delModel(QObject* model)
{
    //
}

void KernelUI::addGroup(int index, QObject* group)
{
    //
}

void KernelUI::switchPickMode()
{
    QMetaObject::invokeMethod(m_leftToolbar, "switchPickMode", Qt::ConnectionType::QueuedConnection);
}

void KernelUI::showAdaptShow(QObject* datafrom)
{
    /*QMetaObject::invokeMethod(m_footer, "showAdaptShow", Q_ARG(QVariant, QVariant::fromValue(datafrom)));*/
}

void KernelUI::switchPopupMode()
{
    //  QQmlProperty::write(m_leftToolbar, "showPop", QVariant::fromValue(true));
    //QMetaObject::invokeMethod(m_leftToolbar, "switchPopupMode", Qt::ConnectionType::QueuedConnection,Q_ARG(QVariant, QVariant::fromValue(false)));
}
void KernelUI::switchShowPop(bool bVis)
{
    emit sigLeftToolBarVisible(bVis);

    //    QMetaObject::invokeMethod(m_leftToolbar, "switchShowPopup", Q_ARG(QVariant, QVariant::fromValue(bVis)));

    //    //wzx: when change to supportMode, disable leftToolbar and mouseRightBtnClickedEvent
    //    m_isSupportChecked = bVis;
    //    QQmlProperty::write(m_leftToolbar, "visible", QVariant::fromValue(bVis));
    
}
void KernelUI::refreshPickPanel()
{
    QMetaObject::invokeMethod(m_leftToolbar, "refreshPick");
}

void KernelUI::executeCommand(QString cmdname, QObject* receiver, QString menuObjName)
{
    cmdname = cmdname.toLower();
    if (cmdname == "requestmenudialog")
    {
        this->requestMenuDialog(receiver, menuObjName);
    }
    else if (cmdname == "requestload")
    {
        requestLoad();
    }
    else if (cmdname == "requestcloseaction")
    {
        
        QMetaObject::invokeMethod(getUI("topbar"), "exitGetDeviceTask", Qt::ConnectionType::QueuedConnection);
        this->requestCloseAction();
    }
}

QObject* KernelUI::getUI(QString uiname)
{
    uiname = uiname.toLower();
    if (uiname == "uiroot")
    {
        return this->root();
    }
    else if (uiname == "uiappwindow")
    {
        return this->appWindow();
    }
    else if (uiname == "uirightpanel" || uiname == "rightpanel")
    {
        return this->rightPanel();
    }
    else if (uiname == "footer")
    {
        return this->footer();
    }
    else if (uiname == "glmainview")
    {
        return this->glMainView();
    }
    else if (uiname == "sliceui")
    {
        return m_sliceUI;
    }else if (uiname == "topbar")
    {
        return this->topBar();
    }else if(uiname=="lefttoolbar")
    {
        return this->leftToolbar();
    }
    else
    {
        return nullptr;
    }
}

int KernelUI::addToolCommand(ToolCommand* command, QString key, int index)
{
    if(this->lCenter())
        this->lCenter()->addCommand(command, index);
    return 0;
}

int KernelUI::removeToolCommand(ToolCommand* command, QString key)
{
    if (this->lCenter())
    {
        this->lCenter()->removeCommand(command);
    }

    return 0;
}

int KernelUI::addActionCommand(ActionCommand* command, QString key)
{
    command->setParent(this);
    CCommandList::getInstance()->addActionCommad(command);
    return 1;
}

int KernelUI::removeActionlCommand(ActionCommand* command, QString key)
{
    CCommandList::getInstance()->removeActionCommand(command);
    return 1;
}



void KernelUI::registerEvent()
{
    addRightMouseEventHandler(getKernelEvent());
}

void KernelUI::setLeftToolbar(QObject* leftToolbar)
{
    m_leftToolbar = leftToolbar;
}

QObject* KernelUI::leftToolbar()
{
    return m_leftToolbar;
}

QObject* KernelUI::root()
{
    return m_root;
}

void KernelUI::setRoot(QObject* root)
{
    m_root = root;
}

QObject* KernelUI::appWindow()
{
    return m_appWindow;
}

QObject* KernelUI::rightPanel()
{
    return m_rightPanel;
}

QObject* KernelUI::glMainView()
{
    return m_glMainView;

}

void KernelUI::setGLMainView(QObject* glMainView)
{

    m_glMainView = glMainView;
}

bool KernelUI::isSupportBtnChecked()
{
    return m_isSupportChecked;
}

QString KernelUI::currentVersion()
{
    return m_currentVersion;
}

void KernelUI::setCurrentVersion(QString version)
{
    m_currentVersion = version;
}

void KernelUI::openFileWithDrop(QList<QUrl> urls)
{
    //if (urls.size() > 0)
    //    CXFILE.openWithUrl(urls.at(0));

    CXFILE.openWithUrls(urls);
};

QObject* KernelUI::footer()
{
    return m_footer;
}

void KernelUI::setFooter(QObject* footer)
{
    m_footer = footer;
}
QObject* KernelUI::topBar()
{
    return m_topbar;
}
void KernelUI::setTopBar(QObject* topbar)
{
    m_topbar = topbar;
}
void KernelUI::setAppWindow(QObject* appWindow)
{
    m_appWindow = appWindow;
    ///// 2020-08-11 add for inout
    CXFILE.init(m_appWindow);
}

void KernelUI::setRightPanel(QObject* rightPanel)
{
    m_rightPanel = rightPanel;
}


void KernelUI::visibleAll(bool visible)
{
    m_leftToolbar->setProperty("visible", visible);
}

QAbstractListModel* KernelUI::lmodel()
{
    return m_leftToolbarModelList;
}

qtuser_qml::ToolCommandCenter* KernelUI::lCenter()
{
    return m_leftToolbarModelList;
}

void KernelUI::requestOpenFileDialog(QObject* receiver)
{
    QMetaObject::invokeMethod(m_appWindow, "requestOpenFileDialog", Q_ARG(QVariant, QVariant::fromValue(receiver)));
}

MeshLoaderWrapper *KernelUI::getMeshLoaderWrapper()const
{
    return m_meshLoaderWrapper;
}

qtuser_qml::TreeModel* KernelUI::treeModel()
{
    return m_modelList;
}
TableModel* KernelUI::tableModel()
{
    return m_tableModelList;
}
BasicSliceUI* KernelUI::createSliceUI()
{
    if (m_sliceUI == nullptr)
    {
        m_sliceUI = new SliceUI(this);
    }
    return m_sliceUI;
}

CusModelListModel *KernelUI::listModel()
{
    return m_ModelList;
}

creative_kernel::AbstractAppSetting* KernelUI::getAppSettings()
{
    return m_appSettings;
}


void KernelUI::requestMenuDialog(QObject* receiver,QString menuObjName)
{
    QObject *pMenu = m_appWindow->findChild<QObject *>("allMenuDialog");
    if(pMenu)
    {
        QMetaObject::invokeMethod(pMenu, "requestMenuDialog", Q_ARG(QVariant, QVariant::fromValue(receiver))
                                  ,Q_ARG(QVariant, QVariant::fromValue(menuObjName)));

    }
}

void KernelUI::initLanguage()
{
    CXFILE.registerOpenHandler(m_meshLoaderWrapper);

    QSettings setting;
    setting.beginGroup("language_perfer_config");
    QString strLanguageType = setting.value("language_type","zh_CN.ts").toString();
    QString strPath = ":/translations/" + strLanguageType;
    Translator::getInstance()->setAppWindow(this->appWindow());
    Translator::getInstance()->loadLanguage_ts(strPath);
}

void KernelUI::requestUpdateLanguage(QString value,int languageType)
{
    if(m_translator)
    {
        QGuiApplication::removeTranslator(m_translator);
    }
    QSettings setting;
    setting.beginGroup("language_perfer_config");
    setting.setValue("language_type", value);
    setting.endGroup();
    initLanguage();
    emit sigChangeMenuLanguage();
}

void KernelUI::requestCloseAction()
{
    ProjectInfoUI::instance()->clearSecen();
    emit sigColseAction();
}
void KernelUI:: beforeCloseWindow()
{
    emit closeWindow();
}

void KernelUI::changeTheme(int themeType)
{
    QMetaObject::invokeMethod(m_appWindow, "changeThemeColor", Q_ARG(QVariant, QVariant::fromValue(themeType)));
    emit sigChangeThemeColor();
}

void KernelUI::changeMenuTheme()
{
    //emit sigChangeMenuLanguage();
}

void KernelUI::slotMeshLoadFiltersChanged(QStringList newExtensions)
{
    m_meshLoadTypes += newExtensions;

    CXFILE.registerOpenHandler(m_meshLoaderWrapper);
}


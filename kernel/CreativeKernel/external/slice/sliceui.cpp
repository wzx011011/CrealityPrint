#include "sliceui.h"

#include "modelselectionproxy.h"

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlProperty>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QQmlContext>
#include <QDir>
#include <QImage>

#include "us/configurationmanager.h"
#include "us/slicermanager.h"
#include "us/profilemanager.h"

#include "kernel/kernelui.h"
#include "interface/selectorinterface.h"
#include "interface/spaceinterface.h"
#include "data/modeln.h"
#include "kernel/abstractkernel.h"
#include "qtuserqml/plugin/treemodel.h"
#include "interface/reuseableinterface.h"
#include "interface/visualsceneinterface.h"
#include "data/softfuncmanager.h"
#include <QDebug>
#include "inout/cxopenandsavefilemanager.h"
#include "interface/modelinterface.h"
#include "crealitycloud/cxnetworkmanager.h"

using namespace creative_kernel;



SliceUI::SliceUI(QObject* parent)
    :BasicSliceUI(parent)
    , m_root(AbstractKernelUI::getSelf()->getUI("UIRightPanel"))
    , m_qmlUI(nullptr)
    , m_selectionProxy(nullptr)
    , m_uiInited(false)
{
    QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();
    engine->rootContext()->setContextProperty("modelListData", QVariant::fromValue(AbstractKernelUI::getSelf()->listModel()));
    engine->rootContext()->setContextProperty("sliceUI", this);

    m_parameterSettingUI = new ParameterSettingUI();
    engine->rootContext()->setContextProperty("parameterUI", m_parameterSettingUI);

    qDebug() << "load xml UI";

    m_imgProvider = new ImgProvider();

    QQmlComponent component(engine, QUrl::fromLocalFile(":/slice/slice/SlicePanel.qml"), nullptr);
    m_qmlUI = component.create(qmlContext(m_root));
    if (!m_qmlUI)
    {
        qDebug() << "init qmlUI failed! please check the component in slicePanel.qml";
        return;
    }
//    QObject* obj = getKernelUI()->appWindow()->findChild<QObject*>("previewPanelWrapper");
//    m_qmlUI->setParent(obj);
    m_qmlUI->setObjectName("sliceUiobj");

    m_paraSettingUI = m_qmlUI;
    m_addCommand = new AddCommand(this);

    m_selectionProxy = new ModelSelectionProxy();

    /////2020-08-28 liuhong add for  qml preview picture provider
    engine->addImageProvider(QLatin1String("imgProvider"), m_imgProvider);

    //use to qml get GlMainobj
    QQmlProperty::write(m_qmlUI, "mainObj", QVariant::fromValue<QObject*>(AbstractKernelUI::getSelf()->getUI("GLMainView")));
    QQmlEngine::setObjectOwnership(m_qmlUI, QQmlEngine::CppOwnership);
    //updateMachineList();

    
    //QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
    //if (pRightPanel)
    //{
    //    QQmlProperty::write(m_qmlUI, "parentObj", QVariant::fromValue<QObject*>(AbstractKernelUI::getSelf()->listModel()));
    //    QObject* pModelList = pRightPanel->findChild<QObject*>("selectModel");
    //    if (pModelList)
    //    {
    //        m_modelSelection = pModelList->findChild<QObject*>("selectModelSelection");
    //        QQmlProperty::write(pModelList, "model", QVariant::fromValue<QObject*>(AbstractKernelUI::getSelf()->listModel()));
    //    }
    //}
    ////
    
    qDebug() << "load xml UI success";

    m_selectionProxy->setSelectionProxy(m_modelSelection);

    bool res = connect(m_selectionProxy, SIGNAL(selectChanged()), AbstractKernelUI::getSelf()->listModel(), SLOT(slotModelSelectionChanged()));
}


SliceUI::~SliceUI()
{
}


void SliceUI::setObject(QString objname, QObject* object)
{
    BasicSliceUI::setObject(objname, object);
    QQmlProperty::write(m_qmlUI, objname, QVariant::fromValue<QObject*>(object));
}

void SliceUI::setParam(QString objname, void* param)
{
    int ncount = (int)(size_t)param;
    emit nozzleCountChange(ncount);
}

int SliceUI::initialize()
{
    if (!m_uiInited)
    {
        m_uiInited = true;

        setObject("load", m_addCommand);

        m_parameterSettingUI->setqmlUI(getParasetting());
        //初始化配置文件列表
        //m_parameterSettingUI->initProfileList();

        QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();

        m_crealityCloudUI = new CrealityCloudUI();
        m_crealityCloudUI->initCxNetWort();
        m_crealityCloudUI->setqmlUI(getParasetting());

    }
    return 0;
}

int SliceUI::uninitialize()
{
    show(false);
    return 0;
}

int SliceUI::onSliceSuccess(QObject* object, void* param)
{
    if(!object)
        return -1;
    qDebug() << "SliceUI::onSliceSuccess";

    emit sigSliceSuccess(object);
    return 0;
}

int SliceUI::onSliceFailed(QObject* object, void* param)
{
    enableStartSliceBtn();
    return 0;
}

int SliceUI::handle(QObject* object, void* param)
{
    invokeFunc("disEnablePreview", nullptr);
    return 0;
}

int SliceUI::onExporSuccess(QObject* object, void* param)
{
    bool isOnlinePrint = (bool)param;
    if (!isOnlinePrint)
    {
        m_strMessageText = tr("Save Finished, Open Local Folder");
        AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "messageDlg");
    }
    return 0;
}

void SliceUI::show(bool show)
{
    QQmlProperty::write(m_qmlUI, "parent", QVariant::fromValue<QObject*>(show ? m_root : nullptr));
    
    if (show)
    {
        setObject("softwarefunction", softFuncManager::instance().GetSoftFunc());
        addSelectTracer(m_selectionProxy);
    }
    else removeSelectorTracer(m_selectionProxy);
}

void SliceUI::update()
{
    setInfoShowObj();
}

void SliceUI::preview()
{
    qDebug() << "SliceUI::preview";
    //m_kernelUI->visibleAll(false);

    QString currentMachine = SlicerManager::instance().getCurrentMachine();
    QString dType;
    //if (ProfileManager::instance().getMachineType(currentMachine) == "FDM")
    //{
    //    dType = "FDM";
    //}
    //else
    //{
    //    dType = "LD";
    //}

    dType = "FDM";
    emit sigStartPreview(dType);

    //invokeFunc("setPreviewMode");
}

void SliceUI::unpreview()
{
    qDebug() << "SliceUI unpreview";
    //m_kernelUI->visibleAll(true);
    invokeFunc("cancelPreview");
    qDebug() << "SliceUI after call cancelPreview";
    requestVisUpdate(false);
}

void SliceUI::enableStartSliceBtn()
{
    QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
    if (pRightPanel)
    {
        QMetaObject::invokeMethod(pRightPanel, "enableSliceBtn");
    }
}

void SliceUI::profileListCallBack(std::list<std::string> profileList)
{
    QStringList res;
    foreach(auto data, profileList)
    {
        res << QString::fromStdString(data);
    }
    emit sigGetProfileList(res);
}

void SliceUI::getProfileList(const QString &printerName)
{
    QString printerNameT = "Ender-220";
    creative_kernel::getcxNetworkManager()->getProfileList(printerNameT.toStdString(), QString().toStdString(),std::bind(&SliceUI::profileListCallBack, this, std::placeholders::_1));
    //    cxNetworkManager
}

QString SliceUI::getMessageText()
{
    return m_strMessageText;
}

void SliceUI::accept()
{
    CXFILE.openLastSaveFolder();
}

QString SliceUI::curMachineType()
{
    QString type;
    QString currentMachine = SlicerManager::instance().getCurrentMachine();
    if (ProfileManager::instance().getMachineType(currentMachine) == "FDM")
    {
        type = "FDM";
    }
    else
    {
       type = "LD";
    }
    return type;
}

QObject *SliceUI::mainObj()
{
    return  AbstractKernelUI::getSelf()->getUI("GLMainView");
}

void SliceUI::layoutAll()
{
    m_addCommand->layoutAll();
}

void SliceUI::deleteModel(QList<int> indexs)
{
//    QModelIndexList list;
//    foreach (auto indexItem, indexs)
//    {
//        QModelIndex modelItem(QModelIndex(), indexItem.row(), 0);
//        list.append(modelItem);
//    }
    m_addCommand->remove();
}

void SliceUI::selectModel(QList<int> indexs)
{
//    QModelIndexList list;
    m_addCommand->selects(indexs);
}

void SliceUI::cancelSelect(QList<int> indexs)
{
    m_addCommand->cancelSelect(indexs);
}

void SliceUI::setModelVisible(int index, bool isVisible)
{
    m_addCommand->setModelVisible(index, false);
}

void SliceUI::invokeFunc(const char* func, QObject* report)
{
    if (!report)
    {
        QMetaObject::invokeMethod(m_qmlUI, func);
        return;
    }
    QMetaObject::invokeMethod(m_qmlUI, func, Q_ARG(QVariant, QVariant::fromValue(report)));
}

void SliceUI::invokeProgress(float r)
{
    //QMetaObject::invokeMethod(m_qmlUI, "sliceProgress", Q_ARG(QVariant, r));
}

QObject* SliceUI::getParasetting()
{
    return m_paraSettingUI;
}

void SliceUI::layerImgChange(const QImage& img)
{
    QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
    QObject* pDLPPreviewPanel = m_qmlUI->findChild<QObject*>("DLPPreviewPanel");
    QObject* pPreview2DPanel = pDLPPreviewPanel->findChild<QObject*>("DLPPreview2DPanel");

    m_imgProvider->setImage(img);
    QMetaObject::invokeMethod(pPreview2DPanel, "updatePrevImg");
}


void SliceUI::setInfoShowObj()
{
    QObject* pInfoShowObj = AbstractKernelUI::getSelf()->getUI("UIRoot")->findChild<QObject*>("infoshowObj");

    if (pInfoShowObj)
    {
        QQmlProperty::write(m_qmlUI, "modelinfoshowObj", QVariant::fromValue<QObject*>(pInfoShowObj));
    }

    QObject* pZsliderObj = AbstractKernelUI::getSelf()->getUI("UIRoot")->findChild<QObject*>("zsliderObj");

    if (pZsliderObj)
    {
        qDebug()<<"pZsliderObj ======";
        QQmlProperty::write(m_qmlUI, "sliderShowObj", QVariant::fromValue<QObject*>(pZsliderObj));
    }

    QObject* pStatusObj = AbstractKernelUI::getSelf()->getUI("UIAppWindow")->findChild<QObject*>("StatusBarObj");
    if(pStatusObj)
    {
        QQmlProperty::write(m_qmlUI, "statusObj", QVariant::fromValue<QObject*>(pStatusObj));
    }
}

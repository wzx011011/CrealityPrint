#include "parametersettingsui.h"

#include <QDebug>
#include "us/slicermanager.h"

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlProperty>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QtCore/QSettings>
#include "us/configurationmanager.h"
#include "us/profilemanager.h"
#include "interface/spaceinterface.h"
#include "data/modeln.h"
#include "kernel/abstractkernel.h"
#include "kernel/kernelui.h"
#include "qtuserqml/plugin/treemodel.h"
#include "interface/reuseableinterface.h"
#include "interface/visualsceneinterface.h"
#include "us/extruderprofile.h"
#include "qtusercore/module/systemutil.h"
#include "interface/machineinterface.h"
#include "def/settingdef.h"
#include <QStandardPaths>
#include "inout/cxopenandsavefilemanager.h"
#include "interface/selectorinterface.h"
#include "interface/camerainterface.h"
#include "interface/modelinterface.h"
#include "qtuser3d/camera/screencamera.h"
#include "interface/spaceinterface.h"
#include "qjsondocument.h"
#include "zlib.h"
#include "kernel/translator.h"

#include <QNetworkReply>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QUuid>
#include <QNetworkInterface>
#include <QCursor>
#include <QClipboard>
#include <QApplication>
#include <QMimeData>
#include <QDesktopServices>

#include "queryequipmentinfotask.h"

#include "interface/gadatainterface.h"
#include "kernel/printerentityuse.h"
#include"us/configurationmanager.h"

#include "crealitycloud/cxnetworkmanager.h"
#include "cusaddprintermodel.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>


class DownImageTask;
using namespace creative_kernel;
ParameterSettingUI::ParameterSettingUI()
    :m_qmlUI(nullptr)
{
    m_configRoot = us::ConfigurationManager::instance().getConfigRoot();
    m_editprofile = new Editprofile();
    m_file = new ParameterFile(m_configRoot);
    isFirstStart = false;
    //  m_extruderfile = ExtruderProfile::instance();//new ExtruderProfile();
    disconnect(Translator::getInstance(), SIGNAL(languageChanged()), this, SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(), SIGNAL(languageChanged()), this, SLOT(slotLanguageChanged()));

    //初始化设备列表
    m_printerList = us::ConfigurationManager::instance().getPresetMachineTypes();
    QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();

    //注册配置文件
    m_ProfileMode = ProfileManager::instance().getProfileModel();
    engine->rootContext()->setContextProperty("profileListModel", m_ProfileMode);
    qDebug() << "999999999999";

    //注册this
    engine->rootContext()->setContextProperty("paraSettingUI", this);
}

ParameterSettingUI::~ParameterSettingUI()
{
    //    if(m_extruderfile)
    //    {
    //        delete m_extruderfile;
    //        m_extruderfile = nullptr;
    //    }
}

void ParameterSettingUI::setqmlUI(QObject* obj)
{
    m_qmlUI = obj;
    //updateMachineList();

    QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
    QQmlProperty::write(pTopBar, "winPos", QVariant::fromValue(this));
    if (pTopBar)
    {
        QObject::connect(pTopBar, SIGNAL(machineManager()), this, SLOT(onMachineManager()));
        QObject::connect(pTopBar, SIGNAL(changeLanguage(int)), this, SLOT(onChangeLanguage(int)));
        QMetaObject::invokeMethod(pTopBar, "setCurLanguage", Q_ARG(QVariant, QVariant::fromValue(getCurrentLanguage())));
    }

    qDebug() << "setqmlUI over";
    showSysMemory();

    QStringList machines = us::ConfigurationManager::instance().getPresetMachineTypes();
    if (machines.size() == 0)
    {
        onMachineAdded("Ender-3");
        isFirstStart = true;
    }
    else
    {
        updateMachineList();
    }
    //
    //	//get the machine list from Json file
    //	QString fileName = "machineList";
    //#ifdef MACHINE_FILE_SUBNAME
    //	fileName = fileName + "_sub";
    //#endif // MACHINE_FILE_SUBNAME
    //
    //	fileName = m_configRoot +"/default/" + fileName + ".json";
    //	QFile file(fileName);
    //	if (!file.open(QIODevice::ReadOnly)) {
    //		qDebug() << "File open failed!";
    //	}
    //	else {
    //		qDebug() << "File open successfully!";
    //	}
    //	QJsonParseError* error = new QJsonParseError;
    //	QJsonDocument jdc = QJsonDocument::fromJson(file.readAll(), error);
    //
    //	//check json file
    //	if (error->error != QJsonParseError::NoError)
    //	{
    //		qDebug() << "parseJson:" << error->errorString();
    //	}
    //
    //	QObject* pMachineCommbox = pTopBar->findChild<QObject*>("AddPrinterDlg");
    //	QMetaObject::invokeMethod(pMachineCommbox, "updateListMode", Q_ARG(QVariant, QString(jdc.toJson())));

    CusAddPrinterModel* cwAddPrinterModel = new CusAddPrinterModel();
    QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();
    engine->rootContext()->setContextProperty("cwAddPrinterModel", cwAddPrinterModel);
    engine->rootContext()->setContextProperty("paramSetUI", this);

    cwAddPrinterModel->addPrinterInfoListFromJson();
}

void ParameterSettingUI::updateMachineList(bool isDel, QString machineName)
{
    m_printerList = us::ConfigurationManager::instance().getPresetMachineTypes();
    emit printerListChanged();
}

void ParameterSettingUI::onFDMView()
{
    QObject *glView = AbstractKernelUI::getSelf()->getUI("glmainview");
    QQmlProperty::write(glView, "visible", QVariant::fromValue<bool>(true));
    if (SlicerManager::instance().getCurrentMachine() == "CR-10 Inspire")
    {

    }
    else
    {
        QList<ModelN*> models = modelns();
        if (models.size() > 0)
        {
            ModelN* pickable = models.at(0);
            selectOne((qtuser_3d::Pickable*)pickable);
            AbstractKernelUI::getSelf()->switchShowPop(true);
            AbstractKernelUI::getSelf()->switchPickMode();
        }
    }
    //    QObject *appWindow = glView->parent();//AbstractKernelUI::getSelf()->getUI("uiappwindow");
    //    if(appWindow)
    //    {
    //        QObject* mainObj = AbstractKernelUI::getSelf()->getUI("UIAppWindow");
    //        QMetaObject::invokeMethod(mainObj, "showModelAlwaysIcom", Q_ARG(QVariant, true));
    //        QObject* pLaserPanel = appWindow->findChild<QObject*>("laserView");
    //        if(pLaserPanel)
    //        {
    //            QQmlProperty::write(pLaserPanel, "visible", QVariant::fromValue<bool>(false));
    //            QObject* pOtherRightItem = AbstractKernelUI::getSelf()->getUI("glmainview")->parent()->findChild<QObject*>("switchModelLayout")->findChild<QObject*>("OtherRightItem");
    //            if(pOtherRightItem)
    //            {
    //                QQmlProperty::write(pOtherRightItem, "visible", QVariant::fromValue<bool>(false));

    //            }

    //        }
    //        QObject* pPlotterPanel = appWindow->findChild<QObject*>("plotterView");
    //        if(pPlotterPanel)
    //        {
    //            QQmlProperty::write(pPlotterPanel, "visible", QVariant::fromValue<bool>(false));
    //        }
    //    }
    CXFILE.setWorkModel("fdm");
}
void ParameterSettingUI::onLaserView()
{
    //    QObject *glView = AbstractKernelUI::getSelf()->getUI("glmainview");
    //    QQmlProperty::write(glView, "visible", QVariant::fromValue<bool>(false));
    //    QObject* leftToolBar = AbstractKernelUI::getSelf()->getUI("lefttoolbar");
    ////    QQmlProperty::write(leftToolBar, "visible", QVariant::fromValue<bool>(false));
    //    QObject *appWindow = glView->parent();//AbstractKernelUI::getSelf()->getUI("uiappwindow");
    //    if(appWindow)
    //    {
    //        QObject* mainObj = AbstractKernelUI::getSelf()->getUI("UIAppWindow");
    //        QMetaObject::invokeMethod(mainObj, "showModelAlwaysIcom", Q_ARG(QVariant, false));
    //        QObject* pLaserPanel = appWindow->findChild<QObject*>("laserView");
    //        if(pLaserPanel)
    //        {
    //            QQmlProperty::write(pLaserPanel, "visible", QVariant::fromValue<bool>(false));//激光机型切换时刷新平台大小
    //            QQmlProperty::write(pLaserPanel, "visible", QVariant::fromValue<bool>(true));
    //            QQmlProperty::write(pLaserPanel, "focus", QVariant::fromValue<bool>(true));
    //            QObject* pOtherRightItem = AbstractKernelUI::getSelf()->getUI("glmainview")->parent()->findChild<QObject*>("switchModelLayout")->findChild<QObject*>("OtherRightItem");
    //            if(pOtherRightItem)
    //            {
    //                QQmlProperty::write(pOtherRightItem, "visible", QVariant::fromValue<bool>(true));
    //                QObject* pLaserItem = pOtherRightItem->findChild<QObject*>("objLaserItem");
    //                if(pLaserItem)
    //                {
    //                    QQmlProperty::write(pLaserItem, "visible", QVariant::fromValue<bool>(true));
    //                }
    //                QObject* pPlotterItem = pOtherRightItem->findChild<QObject*>("objPlotterItem");
    //                if(pPlotterItem)
    //                {
    //                    QQmlProperty::write(pPlotterItem, "visible", QVariant::fromValue<bool>(false));
    //                }
    //            }


    //        }
    //        QObject* pPlotterPanel = appWindow->findChild<QObject*>("plotterView");
    //        if(pPlotterPanel)
    //        {
    //            QQmlProperty::write(pPlotterPanel, "visible", QVariant::fromValue<bool>(false));
    //        }
    //    }
    CXFILE.setWorkModel("laser");
}
void ParameterSettingUI::onPlotterView()
{
    //    QObject *glView = AbstractKernelUI::getSelf()->getUI("glmainview");
    //    QQmlProperty::write(glView, "visible", QVariant::fromValue<bool>(false));
    //    QObject* leftToolBar = AbstractKernelUI::getSelf()->getUI("lefttoolbar");
    ////    QQmlProperty::write(leftToolBar, "visible", QVariant::fromValue<bool>(false));
    //    QObject *appWindow = glView->parent();//AbstractKernelUI::getSelf()->getUI("uiappwindow");
    //    if(appWindow)
    //    {
    //        QObject* mainObj = AbstractKernelUI::getSelf()->getUI("UIAppWindow");
    //        QMetaObject::invokeMethod(mainObj, "showModelAlwaysIcom", Q_ARG(QVariant, false));
    //        QObject* pPlotterPanel = appWindow->findChild<QObject*>("plotterView");
    //        if(pPlotterPanel)
    //        {
    //            QQmlProperty::write(pPlotterPanel, "visible", QVariant::fromValue<bool>(true));
    //            QQmlProperty::write(pPlotterPanel, "focus", QVariant::fromValue<bool>(true));
    //            QObject* pOtherRightItem = AbstractKernelUI::getSelf()->getUI("glmainview")->parent()->findChild<QObject*>("switchModelLayout")->findChild<QObject*>("OtherRightItem");
    //            if(pOtherRightItem)
    //            {
    //                QQmlProperty::write(pOtherRightItem, "visible", QVariant::fromValue<bool>(true));
    //                QObject* pLaserItem = pOtherRightItem->findChild<QObject*>("objLaserItem");
    //                if(pLaserItem)
    //                {
    //                    QQmlProperty::write(pLaserItem, "visible", QVariant::fromValue<bool>(false));
    //                }
    //                QObject* pPlotterItem = pOtherRightItem->findChild<QObject*>("objPlotterItem");
    //                if(pPlotterItem)
    //                {
    //                    QQmlProperty::write(pPlotterItem, "visible", QVariant::fromValue<bool>(true));
    //                }

    //            }

    //            /*QQmlEngine* engine = qmlEngine(pLaserPanel);
    //            QQmlComponent component(engine, QUrl::fromLocalFile(":/laser/laser/LaserLayout.qml"));
    //                   m_qmlUI = component.create(qmlContext(pLaserPanel));
    //                   m_qmlUI->setParent(pLaserPanel);
    //                   m_qmlUI->setObjectName("xxxxx");*/
    //        }
    //        QObject* pLaserPanel = appWindow->findChild<QObject*>("laserView");
    //        if(pLaserPanel)
    //        {
    //            QQmlProperty::write(pLaserPanel, "visible", QVariant::fromValue<bool>(false));
    //        }
    //    }
    CXFILE.setWorkModel("plotter");
}

void ParameterSettingUI::onShowAddPrinterDlg()
{
    if (isFirstStart)
    {
        //invokeFunc("showWizards");
        QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
        if (pTopBar)
        {
            QMetaObject::invokeMethod(pTopBar, "showAddDlg");
        }
    }
    isFirstStart = false;
}

void ParameterSettingUI::onMachineAdded(QString text, bool needUpdate)
{
    creative_kernel::sendDataToGA("Add & Manage 3D Printer", "Add Printer " + text);
    QStringList machines = us::ConfigurationManager::instance().getPresetMachineTypes();
    //QString currentMachine = SlicerManager::instance().getCurrentMachine();
    QString path = QDir::currentPath();

    int res = machines.indexOf(text);
    if(machines.indexOf(text)>=0)
    {
        SlicerManager::instance().setCurrentMachine(text);
        return;
    }
    if (machines.indexOf(text) < 0)
    {
        //Create folder
        m_file->copyBaseQuality(text);
    }
    us::ConfigurationManager::instance().setPresetMachineType(text);

    //    //set currentIndex
    //    qDebug()<<"text =" <<text;
    SlicerManager::instance().setCurrentMachine(text);
    //for init install
    if (machines.size() == 0)
    {
        SlicerManager::instance().setCurrentMachine(text);
        us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();
        /*int machine_width = globalsetting->settings().value("machine_width")->value().toInt();
        int machine_height = globalsetting->settings().value("machine_height")->value().toInt();
        int machine_depth = globalsetting->settings().value("machine_depth")->value().toInt();*/
        QString qstr = globalsetting->settings().value("machine_slicer_type")->value().toString();
        if (qstr == "FDM")SlicerManager::instance().setCurrentSlicerType(SlicerManager::SLICERTYPE::FDM);
        else SlicerManager::instance().setCurrentSlicerType(SlicerManager::SLICERTYPE::DLP);

    }

    if (needUpdate)
    {
        updateMachineList();
    }

    machines = us::ConfigurationManager::instance().getPresetMachineTypes();
    setPrinterList(machines);

    //添加设备的时候，调用当前设备更改的逻辑，去刷新材料和配置文件
    onMachineChanged(text);
    m_file->updateProfileByMachineName(text);
}

void ParameterSettingUI::onSelectMachineChanged(QString text)
{
    QString name = text;
    QString path = m_configRoot + "/default/";

    us::ConfigurationManager::instance().setProfilePath(path);
    USettings* settings = MACHINECONF(text);

    int machine_width = settings->settings().value("machine_width")->value().toInt();
    int machine_height = settings->settings().value("machine_height")->value().toInt();
    int machine_depth = settings->settings().value("machine_depth")->value().toInt();
    float machine_NozzleSize = 0.0;
    if (settings->settings().find("machine_nozzle_size") != settings->settings().end())
    {
        machine_NozzleSize = settings->settings().value("machine_nozzle_size")->value().toFloat();
    }

    QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
    if (pTopBar)
    {
        QObject* pSelectMachine = pTopBar->findChild<QObject*>("AddPrinterDlg");
        if (pSelectMachine)
        {
            QQmlProperty::write(pSelectMachine, "machineWidth", QVariant::fromValue(machine_width));
            QQmlProperty::write(pSelectMachine, "machineDepth", QVariant::fromValue(machine_depth));
            QQmlProperty::write(pSelectMachine, "machineHeight", QVariant::fromValue(machine_height));
            QQmlProperty::write(pSelectMachine, "machineNozzleSize", QVariant::fromValue(machine_NozzleSize));
        }
    }
}

void ParameterSettingUI::onMachineDeleted(QString text)
{
    ////remove extruderfile 2020-10-09
    // m_extruderfile->deletefile();
    ExtruderProfile::instance()->deletefile();
    //
    QStringList machines = us::ConfigurationManager::instance().getPresetMachineTypes();
    if (machines.indexOf(text) < 0)
    {
        //Create folder
        return;
    }
    m_file->delMachinesFile();
    us::ConfigurationManager::instance().removeMachineType(text);
    updateMachineList(true, text);
}

// in combox change machine
void ParameterSettingUI::onMachineChanged(QString text)
{
    qDebug()<<"onMachineChanged text=" <<text ;
    if (text.isEmpty())
    {
        text = SlicerManager::instance().getCurrentMachine();
    }
    SlicerManager::instance().clearGlobalCache();
    SlicerManager::SLICERTYPE oldtype = SlicerManager::instance().getCurrentSlicerType();

    QList<ModelN*> ModelNs = modelns();
    QVector<QVector3D> vctdistance;
    for (size_t i = 0; i < ModelNs.size(); i++)
    {
        QVector3D oldboxcenter = baseBoundingBox().center();
        QVector3D qdistance = baseBoundingBox().center() - ModelNs.at(i)->globalSpaceBox().center();
        qdistance.setZ(0.0);
        vctdistance.push_back(qdistance);
    }

    m_file->copyBaseMachine(text);
    //m_file->copyBaseQuality(text);
    SlicerManager::instance().setCurrentMachine(text);
    //ProfileManager::instance().createMaterialsByMachineName(text);
    m_file->updateProfileByMachineName(text);

    //init profile index
    //m_ProfileMode->setSelectIndex(1);
    //    QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
    //    if(pRightPanel)
    //    {
    //        QObject* pProfileList = pRightPanel->findChild<QObject*>("selectProfile");
    //        if (pProfileList)
    //        {
    //            QQmlProperty::write(pProfileList, "currentItemindex", QVariant::fromValue(1));
    //            QMetaObject::invokeMethod(pRightPanel, "updateProfileModel");
    //        }
    //    }

    us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();
    int machine_extruder_count = globalsetting->settings().value("machine_extruder_count")->value().toInt();
    int machine_width = globalsetting->settings().value("machine_width")->value().toInt();
    int machine_height = globalsetting->settings().value("machine_height")->value().toInt();
    int machine_depth = globalsetting->settings().value("machine_depth")->value().toInt();
    QString machine_support_slicer_type = globalsetting->settings().value("machine_support_slicer_type")->value().toString();
    QString qstr = globalsetting->settings().value("machine_slicer_type")->value().toString();
    QStringList machine_support_types = machine_support_slicer_type.split("-");
    QObject* switchModel = AbstractKernelUI::getSelf()->getUI("glmainview")->parent()->findChild<QObject*>("switchModelLayout")->findChild<QObject*>("switchModel");

    SlicerManager::instance().updateExtruder();
    if (machine_extruder_count == 1)
    {
        QList<ModelN*> models = modelns();
        for (size_t i = 0; i < models.size(); i++)
        {
            ModelN* m = models.at(i);
            m->setNozzle(0);
        }
    }

    //    if(switchModel)
    //    {
    //        if(machine_support_types.size()>1)
    //        {
    //            QQmlProperty::write(switchModel, "visible", QVariant::fromValue(true));
    //            QQmlProperty::write(switchModel, "isShow", QVariant::fromValue(true));
    //            QQmlProperty::write(switchModel, "currentModel", "laser");

    //            if (machine_support_slicer_type == "FDM-LASER-CNC")
    //            {
    //                QQmlProperty::write(switchModel, "hasCNC", QVariant::fromValue(true));
    //            }
    //            else
    //            {
    //                QQmlProperty::write(switchModel, "hasCNC", QVariant::fromValue(false));
    //            }
    //        }
    //        else
    //        {
    //            QQmlProperty::write(switchModel, "visible", QVariant::fromValue(false));
    //            QQmlProperty::write(switchModel, "isShow", QVariant::fromValue(false));
    //        }
    //    }
    //    if (qstr == "FDM")
    //    {
    //        SlicerManager::instance().setCurrentSlicerType(SlicerManager::SLICERTYPE::FDM);
    //        if(machine_support_types.size()>1)
    //        {
    onLaserView();
    //        }
    //        else
    //        {
    onFDMView();
    //        }
    //        QString strPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Machines/";
    //        us::ConfigurationManager::instance().setProfilePath(strPath);
    //        USettings* settings = MACHINECONF(text);
    //        QHash<QString, us::SettingItemDef*> hashItemDef = us::SettingDef::instance().getHashItemDef();

    //        for (const auto val : hashItemDef)
    //        {
    //            val->showValueStr = "";
    //        }

    //        for (const auto val : settings->settings())
    //        {
    //            hashItemDef[val->key()]->showValueStr = val->value().toString();
    //        }
    //    }
    //    else
    //    {
    //        SlicerManager::instance().setCurrentSlicerType(SlicerManager::SLICERTYPE::DLP);
    //    }

    if (machine_width < 1)
    {
        machine_width = 1;
    }
    if (machine_depth < 1)
    {
        machine_depth = 1;
    }
    if (machine_height < 1)
    {
        machine_height = 1;
    }

    //热床分模块加热底图
    qtuser_3d::PrinterEntity* entity = getCachedPrinterEntity();
    if (SlicerManager::instance().globalsettings()->settings().value("machine_name")->value()=="Creality CR-10 Inspire Pro")
    {
        entity->visibleBottomColor(1);
        entity->drawBedFaces(bedType::CR_10_Inspire_Pro);
    }
    else if (SlicerManager::instance().globalsettings()->settings().value("machine_name")->value() == "Creality CR-GX")
    {
        entity->visibleBottomColor(0);
        entity->drawBedFaces(bedType::CR_GX);
    }
    else if (SlicerManager::instance().globalsettings()->settings().value("machine_name")->value() == "Creality CR-10 H1")
    {
        entity->visibleBottomColor(0);
        entity->drawBedFaces(bedType::CR_10H1);
    }
    else
    {
        entity->visibleBottomColor(0);
        entity->drawBedFaces(bedType::None);
    }


    //qtuser_3d::Box3D box(QVector3D(0, 0, 0), QVector3D(machine_width, machine_depth, machine_height));
    //creative_kernel::setBaseBoundingBox(box);
    bool isZero = SlicerManager::instance().globalsettings()->settings().value("machine_center_is_zero")->value().toBool();
    qtuser_3d::Box3D box;

    if (isZero)
    {
        float MAX_X = machine_width / 2.0;
        float MAX_Y = machine_depth / 2.0;
        box = qtuser_3d::Box3D(QVector3D(-MAX_X, -MAX_Y, 0), QVector3D(MAX_X, MAX_Y, machine_height));
        creative_kernel::setBaseBoundingBox(box);
    }
    else
    {
        box = qtuser_3d::Box3D(QVector3D(0, 0, 0), QVector3D(machine_width, machine_depth, machine_height));
        creative_kernel::setBaseBoundingBox(box);
    }


    //machine_is_belt
    bool isBelt = SlicerManager::instance().globalsettings()->settings().value("machine_is_belt")->value().toBool();
    qtuser_3d::ScreenCamera* aScreencamera = visCamera();
    if (isBelt)
    {
        Qt3DRender::QCamera* aCamera = aScreencamera->camera();
        //setFarPlane.set
        QVector3D size = box.size();
        qtuser_3d::Box3D b;
        b += box.min;
        QVector3D bmax = box.max;
        bmax.setY(1000.0f);
        b += bmax;
        aScreencamera->fittingBoundingBox(b);
        aCamera->lens()->setFarPlane(machine_depth);
        aScreencamera->setUpdateNearFarRuntime(false);
    }
    else
    {
        aScreencamera->setUpdateNearFarRuntime(true);
        aScreencamera->fittingBoundingBox(box);
    }



    creative_kernel::setModelEffectBox(box.min, box.max);

    QSettings setting;
    setting.beginGroup("machine_setting");
    setting.setValue("machine_name", text);
    setting.endGroup();

    SlicerManager::SLICERTYPE newtype = SlicerManager::instance().getCurrentSlicerType();
    if (oldtype != newtype)
    {
        machineTypeChanged((int)oldtype, (int)newtype);
    }

    for (size_t i = 0; i < ModelNs.size(); i++)
    {
        QVector3D NewCenter = baseBoundingBox().center() - vctdistance[i];
        NewCenter.setZ(0.0);
        QVector3D newLocal = ModelNs.at(i)->mapGlobal2Local(NewCenter);
        ModelNs.at(i)->setLocalPosition(newLocal);
    }

    checkModelRange();
    checkBedRange();
    onMachineSelectChange();
    showSysMemory();
}

void ParameterSettingUI::onEditProfiled()
{
    //getCXLocalNetworkManager()->findDevices();


    QString strPath = m_file->getCoachPath() + SlicerManager::instance().getCurrentMachine() + "/";
    us::ConfigurationManager::instance().setProfilePath(strPath);
    SlicerManager::instance().setCurrentQuality(ProfileManager::instance().getcurrentProfileName());

    //SlicerManager::instance().mergeSettingCache();
    //QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
    //if (pRightPanel)
    {
        QString currentMachine = SlicerManager::instance().getCurrentMachine();

        QObject* pSelectMachine = nullptr;
        std::map<QString, QString> value;
        if (m_file->getMachineType(currentMachine) == "FDM")
        {
            //QQmlProperty::write(pRightPanel, "currentMachineType", "FDM");
            //pSelectMachine = pRightPanel->findChild<QObject*>("editprofileobj");
            //if(pSelectMachine)
            {
                QHash<QString, us::SettingItemDef*> hashItemDef = us::SettingDef::instance().getHashItemDef();

                //清空展示值
                for (const auto val : hashItemDef)
                {
                    val->showValueStr = "";
                }

                ////将机器参数赋值给展示值
                QString strMachinesPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Machines/";
                us::ConfigurationManager::instance().setProfilePath(strMachinesPath);
                USettings* settings = MACHINECONF(SlicerManager::instance().getCurrentMachine());
                for (const auto val : settings->settings())
                {
                    hashItemDef[val->key()]->showValueStr = val->value().toString();
                }

                ////将用户设置参数赋值给展示值（low.default / mid.default / high.default）
                QString strProfilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Profiles/" + SlicerManager::instance().getCurrentMachine() + "/";
                us::ConfigurationManager::instance().setProfilePath(strProfilePath);
                USettings* userSetting = ProfileManager::instance().getSelectUSettings();
                QHash<QString, USetting*> temp = userSetting->settings();
                for (const auto specval : userSetting->settings())
                {
                    QString qstrKey = specval->key();
                    hashItemDef[qstrKey]->showValueStr = userSetting->settings().value(qstrKey)->value().toString();
                }

            }
        }

        if (nullptr != pSelectMachine)
        {
            QString ProfileName = ProfileManager::instance().getcurrentProfileName();
            auto index = ProfileName.lastIndexOf("-");
            QString materialName = "PLA";
            if (index != -1)
            {
                materialName = ProfileName.left(index);
            }
            //SlicerManager::instance().setCurrentQuality(ProfileName);
            QQmlProperty::write(pSelectMachine, "profileName", ProfileName);
            QStringList materials = m_file->getAllfile("", 1);
            QQmlProperty::write(pSelectMachine, "modelmaterial", materials);
            int index0 = -1;
            for (int i = 0; i < materials.length(); ++i)
            {
                if (materialName == materials[i])
                {
                    index0 = i;
                    break;
                }
            }
            QQmlProperty::write(pSelectMachine, "modelmaterialIndex", QVariant::fromValue(index0));
        }
    }

    SlicerManager::instance().setSliceParamDirty(true);
}

void ParameterSettingUI::onEditAddProfiled()
{
    QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
    if (pRightPanel)
    {
        QString currentMachine = SlicerManager::instance().getCurrentMachine();
        QString currentMachineType = m_file->getMachineType(currentMachine);
        if (currentMachineType == "FDM")
        {
            QQmlProperty::write(pRightPanel, "currentMachineType", "FDM");
        }
        else
        {
            QQmlProperty::write(pRightPanel, "currentMachineType", "LD");
        }

        QObject* pSelectMachine = pRightPanel->findChild<QObject*>("addeditprofileobj");
        if (pSelectMachine)
        {
            QStringList machines = us::ConfigurationManager::instance().getPresetMachineTypes();
            QQmlProperty::write(pSelectMachine, "model", QVariant::fromValue(machines));
            //select machine
            for (int i=0; i<machines.size();i++)
            {
                if (m_file->getMachineType(machines[i]) != currentMachineType)
                {
                    machines.removeAt(i);
                    i--;
                }
            }

            int currentIndex = machines.indexOf(SlicerManager::instance().getCurrentMachine());
            if (currentIndex < 0)
            {
                currentIndex = 0;
            }
            else
            {
                SlicerManager::instance().setCurrentMachine(machines.at(currentIndex));
            }
            QQmlProperty::write(pSelectMachine, "currentIndex", QVariant::fromValue(currentIndex));


            QVariant strMachine = QQmlProperty::read(pSelectMachine, "currentText");

            QStringList oldprofile = m_file->getAllfile(strMachine.toString());
            if (oldprofile.size() == 0)
            {
                m_file->copyBaseQuality(strMachine.toString());
                oldprofile = m_file->getAllfile(strMachine.toString());
            }
            QQmlProperty::write(pSelectMachine, "modelprofile", QVariant::fromValue(oldprofile));

            int index =oldprofile.indexOf(ProfileManager::instance().getcurrentProfileName());
            QQmlProperty::write(pSelectMachine, "modelprofileIndex", QVariant::fromValue(index));

            QStringList materialprofile = m_file->getAllfile(strMachine.toString(), 1);
            QQmlProperty::write(pSelectMachine, "modelmaterial", QVariant::fromValue(materialprofile));

            int index0 = materialprofile.size() > 1 ? 1 : 0;
            QQmlProperty::write(pSelectMachine, "modelmaterialIndex", QVariant::fromValue(index0));
        }
    }
}

void ParameterSettingUI::onPrinterchanged(QString currenttext)
{
    QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
    if (pRightPanel)
    {
        QObject* pSelectMachine = pRightPanel->findChild<QObject*>("addeditprofileobj");
        if (pSelectMachine)
        {
            //profile
            QStringList oldprofile = m_file->getAllfile(currenttext);
            if (oldprofile.size() == 0)
            {
                m_file->copyBaseQuality(currenttext);
                oldprofile = m_file->getAllfile(currenttext);
            }
            QQmlProperty::write(pSelectMachine, "modelprofile", QVariant::fromValue(oldprofile));

            //material
            QStringList materialprofile = m_file->getAllfile(currenttext, 1);
            QQmlProperty::write(pSelectMachine, "modelmaterial", QVariant::fromValue(materialprofile));
        }
    }
}

bool ParameterSettingUI::onConfirmNewName(QString currenttext)
{
    QStringList oldprofile = m_file->getAllfile(SlicerManager::instance().getCurrentMachine());
    QVariant rValue;
    QString msg = "new";
    bool res = false;
    for (const auto val : oldprofile)
    {
        if (val == currenttext)
        {
            msg = "Existed";
            res = true;
            break;
        }
    }

    emit sigNewProfileCheck(msg);
    return res;
}

void ParameterSettingUI::onSaveProfile(QString newProfile, QString printer, QString material)
{
    QString strQuality = "";
    Profile* profile = m_ProfileMode->selectProfile();
    QString oldProfile = profile->fullName();
    if (oldProfile.contains("high"))
    {
        strQuality = "high";
    }
    else if (oldProfile.contains("middle"))
    {
        strQuality = "middle";
    }
    else if (oldProfile.contains("low"))
    {
        strQuality = "low";
    }
    else if (oldProfile.contains("fast"))
    {
        strQuality = "fast";
    }
    newProfile = newProfile + "_" + strQuality;
    m_file->addNewProfile(newProfile, printer, material, oldProfile);
    m_ProfileMode->addProfile(material, newProfile, strQuality);

    //SlicerManager::instance().setCurrentQuality(newProfile);
    QString strPath = m_file->getCoachPath() + SlicerManager::instance().getCurrentMachine() + "/";
    us::ConfigurationManager::instance().setProfilePath(strPath);
    //SlicerManager::instance().setCurrentQuality(newProfile);

    QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
    if (pRightPanel)
    {
        QString currentMachine = SlicerManager::instance().getCurrentMachine();
        QObject* pSelectMachine = nullptr;
        std::map<QString, QString> value;
        //std::map<QString, QString> value_dlp;
        if (m_file->getMachineType(currentMachine) == "FDM")
        {
            pSelectMachine = pRightPanel->findChild<QObject*>("editprofileobj");
            m_editprofile->getALLprofile(value);
            for (const auto val : value)
            {
                QQmlProperty::write(pSelectMachine, val.first, val.second);
            }
            QQmlProperty::write(pSelectMachine, "profileName", newProfile);
        }
    }
}

void ParameterSettingUI::onResetbtn(const bool& isFactory)
{
    QString curMachine = SlicerManager::instance().getCurrentMachine();
    QString strPath = m_file->getCoachPath() + curMachine + "/";
    QString profileName = ProfileManager::instance().getcurrentProfileName();
    us::ConfigurationManager::instance().setProfilePath(strPath);
    if (isFactory)
    {
        QString srcProfileName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/sliceconfig/default/" + curMachine + "/" + profileName + ".default";
        QString dstProfileName = strPath + profileName + ".default";
        QFile::remove(dstProfileName);
        QFile::copy(srcProfileName, dstProfileName);
    }
    SlicerManager::instance().setCurrentQuality(profileName);

    //SlicerManager::instance().setCurrentQuality(SlicerManager::instance().getCurrentQuality());
    //QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
    //if (pRightPanel)
    {
        QString currentMachine = SlicerManager::instance().getCurrentMachine();
        QObject* pSelectMachine = nullptr;
        std::map<QString, QString> value;
        //if (m_file->getMachineType(currentMachine) == "FDM")
        //{
        //    pSelectMachine = pRightPanel->findChild<QObject*>("editprofileobj");
        //    //m_editprofile->getALLprofile(value);
        //}

        //if (nullptr != pSelectMachine)
        {
            QHash<QString, us::SettingItemDef*> hashItemDef = us::SettingDef::instance().getHashItemDef();

            //清空展示值
            for (const auto val : hashItemDef)
            {
                val->showValueStr = "";
            }

            ////将用户设置参数赋值给展示值（low.default / mid.default / high.default）
            USettings* userSetting = ProfileManager::instance().getSelectUSettings();
            QHash<QString, USetting*> temp = userSetting->settings();
            for (const auto specval : userSetting->settings())
            {
                QString qstrKey = specval->key();
                hashItemDef[qstrKey]->showValueStr = userSetting->settings().value(qstrKey)->value().toString();
            }

            ////将机器参数赋值给展示值
            QString strMachinesPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Machines/";
            us::ConfigurationManager::instance().setProfilePath(strMachinesPath);
            USettings* settings = MACHINECONF(SlicerManager::instance().getCurrentMachine());
            for (const auto val : settings->settings())
            {
                hashItemDef[val->key()]->showValueStr = val->value().toString();
            }
        }
    }

    SlicerManager::instance().setSliceParamDirty(true);
}

void ParameterSettingUI::onSaveProfilebtn()
{
    SlicerManager::instance().mergeSettingCache();
    m_file->saveProfile();

    m_file->updateProfileLayerHeight("");

    //m_file->updateProfileByMachineName(SlicerManager::instance().getCurrentMachine()); //bug 1012347
}

void ParameterSettingUI::onValchanged(QString key, QString value)
{
    if (key == "machine_start_gcode" || key == "machine_end_gcode")
    {
        value = value.replace("\n", "\\n");
    }
    m_editprofile->settingProfile(key, value);
}

bool ParameterSettingUI::onDeleteProfilebtn()
{
    bool bRemove = m_file->delFile(ProfileManager::instance().getcurrentProfileName());
    if (bRemove)
        m_ProfileMode->deleteCurFile();
    else
        qDebug() << QString("删除%1文件失败！").arg(ProfileManager::instance().getcurrentProfileName());

    return bRemove;
}

//保存数据
void ParameterSettingUI::onSaveManageConfig(const QString& machineName)
{
    SlicerManager::instance().mergeSettingCache();
    m_file->saveMachineProfile(machineName);
    //us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();
    int machine_width = SlicerManager::instance().globalsettings_modify()->settings().value("machine_width")->value().toInt();
    int machine_height = SlicerManager::instance().globalsettings_modify()->settings().value("machine_height")->value().toInt();
    int machine_depth = SlicerManager::instance().globalsettings_modify()->settings().value("machine_depth")->value().toInt();
    QString qstr = SlicerManager::instance().globalsettings_modify()->settings().value("machine_slicer_type")->value().toString();
    if (qstr == "FDM")SlicerManager::instance().setCurrentSlicerType(SlicerManager::SLICERTYPE::FDM);
    else SlicerManager::instance().setCurrentSlicerType(SlicerManager::SLICERTYPE::DLP);

    //qtuser_3d::Box3D box(QVector3D(0, 0, 0), QVector3D(machine_width, machine_depth, machine_height));
    //creative_kernel::setBaseBoundingBox(box);
    us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();
    qtuser_3d::Box3D box;
    if (globalsetting->settings().value("machine_center_is_zero")->value().toBool())
    {
        float MAX_X = machine_width / 2.0;
        float MAX_Y = machine_depth / 2.0;
        box = qtuser_3d::Box3D(QVector3D(-MAX_X, -MAX_Y, 0), QVector3D(MAX_X, MAX_Y, machine_height));
        creative_kernel::setBaseBoundingBox(box);
    }
    else
    {
        box = qtuser_3d::Box3D(QVector3D(0, 0, 0), QVector3D(machine_width, machine_depth, machine_height));
        creative_kernel::setBaseBoundingBox(box);
    }

    creative_kernel::setModelEffectBox(box.min, box.max);

    //machine_is_belt
    //us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();
    bool isBelt = globalsetting->settings().value("machine_is_belt")->value().toBool();
    qtuser_3d::ScreenCamera* aScreencamera = visCamera();
    if (isBelt)
    {
        Qt3DRender::QCamera* aCamera = aScreencamera->camera();
        //setFarPlane.set
        QVector3D size = box.size();
        qtuser_3d::Box3D b;
        b += box.min;
        QVector3D bmax = box.max;
        bmax.setY(1000.0f);
        b += bmax;
        aScreencamera->fittingBoundingBox(b);
        aCamera->lens()->setFarPlane(machine_depth);
        aScreencamera->setUpdateNearFarRuntime(false);
    }
    else
    {
        aScreencamera->setUpdateNearFarRuntime(true);
        aScreencamera->fittingBoundingBox(box);
    }
}
//保存extruder 数据,根据1，2分别处理
void ParameterSettingUI::onSaveExtruderConfig(int nExtruder)
{
    qDebug()<<"onSaveExtruderConfig =" << nExtruder;
    if(nExtruder == 1)
    {
        // m_extruderfile->deletefile();
        //ExtruderProfile::instance()->deletefile();
        ExtruderProfile::instance()->saveExtruderData(0);
    }
    else if(nExtruder == 2)
    {
        //        m_extruderfile->saveExtruderData(0);
        //        m_extruderfile->saveExtruderData(1);

        ExtruderProfile::instance()->saveExtruderData(0);
        ExtruderProfile::instance()->saveExtruderData(1);
    }

    updateMachineList();
}

void ParameterSettingUI::onValuechanged_Extruder(QString key,QString value,int type)
{
    // m_extruderfile->setKeyValue(key,value,type);
    ExtruderProfile::instance()->setKeyValue(key,value,type);
}

void ParameterSettingUI::onExtruderCntChanged(int extruderCnt)
{
    qDebug() << "onExtruderCntChanged " << extruderCnt;
    if (extruderCnt < 1)
    {
        return;
    }
    us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();

    QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
    if (pRightPanel)
    {
        QObject* pSelectMachine = nullptr;
        std::map<QString, QString> value;

        pSelectMachine = pRightPanel->findChild<QObject*>("fdmMachineParemeter");
        if (pSelectMachine)
        {
            //do extruder data
            if (extruderCnt >= 1)
            {
                // m_extruderfile->getALLExtruderfile(value,0);
                ExtruderProfile::instance()->getALLExtruderfile(value, 0);
                for (auto val : value)
                {
                    qDebug() << "bExtruder0 val.first =" << val.first;
                    qDebug() << "bExtruder0 val.second =" << val.second;
                    QString strTmp = val.second.replace("\\n", "\n");
                    QQmlProperty::write(pSelectMachine, val.first + "_0", strTmp);
                }
            }

            if (extruderCnt >= 2)
            {
                //m_extruderfile->getALLExtruderfile(value,1);
                ExtruderProfile::instance()->getALLExtruderfile(value, 1);
                for (auto val : value)
                {
                    qDebug() << "bExtruder1 val.first =" << val.first;
                    qDebug() << "bExtruder1 val.second =" << val.second;
                    QString strTmp = val.second.replace("\\n", "\n");
                    QQmlProperty::write(pSelectMachine, val.first + "_1", strTmp);
                }
            }

            QQmlProperty::write(pSelectMachine, "currentTabIndex", "0");
        }
    }
}

void ParameterSettingUI::onChangeLanguage(int nType)
{
    switch (nType) {
    case 0: getKernelUI()->requestUpdateLanguage("en.ts", 0);
        break;
    case 1: getKernelUI()->requestUpdateLanguage("zh_CN.ts", 0);
        break;
    case 2: getKernelUI()->requestUpdateLanguage("zh_TW.ts", 0);
        break;
    }
}


void ParameterSettingUI::onMachineManager()
{
    QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
    QVariant model;
    QVariant currentIndex;
    if (pTopBar)
    {
        QObject* pSelectMachine = pTopBar->findChild<QObject*>("selectMachine");
        if (pSelectMachine)
        {
            QObject* pCombox = pSelectMachine->findChild<QObject*>("machineCommbox");
            if (pCombox)
            {
                model = QQmlProperty::read(pCombox, "model");
                currentIndex = QQmlProperty::read(pCombox, "currentIndex");
            }
        }
    }
    QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
    if (pRightPanel)
    {
        QMetaObject::invokeMethod(pRightPanel, "showManagerMachine2", Q_ARG(QVariant, currentIndex), Q_ARG(QVariant, model));
    }
}

void ParameterSettingUI::onAddNewPrinter(QString newprinter)
{
    QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
    if (pRightPanel)
    {
        QObject* pCombox = pRightPanel->findChild<QObject*>("addeditprinterobj");
        if (pCombox)
        {
            QVariant strMachine = QQmlProperty::read(pCombox, "newprintertext");
            QVariant strMachineold = QQmlProperty::read(pCombox, "currentText_printer");
            m_file->addNewMachine(strMachine.toString(), strMachineold.toString());
            QString type = m_file->getMachineType(strMachine.toString());
            if (!type.isEmpty())
            {
                onMachineAdded(strMachine.toString());
                SlicerManager::instance().setCurrentMachine(strMachine.toString());
                updateMachineList();
                //	onManagerMachine();
                QStringList machines = us::ConfigurationManager::instance().getPresetMachineTypes();

                // 				QString currentMachine = SlicerManager::instance().getCurrentMachine();
                // 				QString currentMachineType = m_file->getMachineType(currentMachine);
                // 				//select machine
                // 				for (int i = 0; i < machines.size(); i++)
                // 				{
                // 					if (m_file->getMachineType(machines[i]) != currentMachineType)
                // 					{
                // 						machines.removeAt(i);
                // 						i--;
                // 					}
                // 				}

                QQmlProperty::write(pRightPanel, "currentPrinterIndex", machines.indexOf(SlicerManager::instance().getCurrentMachine()));
            }
        }
    }
}

void ParameterSettingUI::onAddProfileClicked()
{
    creative_kernel::sendDataToGA("Configuration Profile", "New Profile");
}

void ParameterSettingUI::onEditProfileClicked()
{
    creative_kernel::sendDataToGA("Configuration Profile", "Edit Profile");
}

void ParameterSettingUI::onMangerPrinterClicked()
{
    creative_kernel::sendDataToGA("Add & Manage 3D Printer", "Manage Printer");
}

//读参数 global
void ParameterSettingUI::onManagerMachine(const QString& machineName, bool isReset)
{
    QString currentMachine;
    if (machineName == QString())
    {
        currentMachine = SlicerManager::instance().getCurrentMachine();

    }
    else {
        currentMachine = machineName;
    }
    m_file->copyBaseMachine(currentMachine, isReset);

    qDebug()<<"onManagerMachine currentMachine ==" <<currentMachine;
    QString strPath = m_file->getCoachPath(2);
    us::ConfigurationManager::instance().setProfilePath(strPath);
    SlicerManager::instance().setMachineData(currentMachine);

    //新逻辑
    QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();
    QObject* pRoot = engine->rootObjects().first();
    QObject* pSelectMachine = pRoot->findChild<QObject*>("fdmMachineParemeter");

    //QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
    //if (pRightPanel)
    {

        //        QObject* pSelectMachine = nullptr;
        std::map<QString, QString> value;
        //if (m_file->getMachineType(currentMachine) == "DLP")
        //{
        //    QQmlProperty::write(pRightPanel, "currentMachineType", "LD");
        //    pSelectMachine = pRightPanel->findChild<QObject*>("dlpMachineParemeter");
        //    if (pSelectMachine)
        //    {
        //        m_editprofile->getALLMachineprofile_DLP(value);

        //        for (const auto val : value)
        //        {
        //            QQmlProperty::write(pSelectMachine, val.first, val.second);
        //        }
        //        QQmlProperty::write(pRightPanel, "managerDlpvisable", "true");
        //        QQmlProperty::write(pRightPanel, "managerFdmvisable", "false");
        //    }
        //}
        //else
        {
            QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();
            QObject *pRoot = engine->rootObjects().first();
            //QQmlProperty::write(pRightPanel, "currentMachineType", "FDM");
            pSelectMachine = pRoot->findChild<QObject*>("fdmMachineParemeter");
            if (pSelectMachine)
            {
                int extruderCnt = 0;
                QObject::disconnect(pSelectMachine, SIGNAL(valChanged_Extruder(QString,QString,int)), this, SLOT(onValuechanged_Extruder(QString,QString,int)));
                QObject::disconnect(pSelectMachine, SIGNAL(extruderCntChanged(int)), this, SLOT(onExtruderCntChanged(int)));
                m_editprofile->getALLMachineprofile(value);
                bool bExtruder = false;
                for (auto val : value)
                {
                    qDebug()<<"val.first =" <<val.first;
                    qDebug()<<"val.second =" <<val.second;

                    if (val.first == "machine_start_gcode" || val.first == "machine_end_gcode")
                    {
                        val.second = val.second.replace("\\n", "\n");
                    }

                    QQmlProperty::write(pSelectMachine, val.first, val.second);

                    QHash<QString, us::SettingItemDef*> hashItemDef = us::SettingDef::instance().getHashItemDef();
                    hashItemDef[val.first]->showValueStr = val.second;

                    if(val.first == "machine_extruder_count" && val.second.toInt() >= 1)
                    {
                        bExtruder = true;
                        extruderCnt = val.second.toInt();
                    }
                }
                /* QQmlProperty::write(pRightPanel, "managerFdmvisable", "true");
                QQmlProperty::write(pRightPanel, "managerDlpvisable", "false");*/

                //do extruder data
                if(bExtruder)
                {
                    if (extruderCnt >= 2)
                    {
                        // m_extruderfile->getALLExtruderfile(value,1);
                        ExtruderProfile::instance()->getALLExtruderfile(value, 1);
                        for (auto val : value)
                        {
                            //                        qDebug()<<"bExtruder1 val.first =" <<val.first;
                            //                        qDebug()<<"bExtruder1 val.second =" <<val.second;
                            QString strTmp = val.second.replace("\\n", "\n");
                            QQmlProperty::write(pSelectMachine, val.first + "_1", strTmp);
                        }
                    }

                    if (extruderCnt >= 1)
                    {
                        // m_extruderfile->getALLExtruderfile(value,0);
                        ExtruderProfile::instance()->getALLExtruderfile(value, 0);
                        for (auto val : value)
                        {
                            qDebug() << "bExtruder0 val.first =" << val.first;
                            qDebug() << "bExtruder0 val.second =" << val.second;
                            QString strTmp = val.second.replace("\\n", "\n");
                            QQmlProperty::write(pSelectMachine, val.first + "_0", strTmp);
                        }
                    }
                }
                QQmlProperty::write(pSelectMachine, "currentTabIndex" ,"0");
                QMetaObject::invokeMethod(pSelectMachine, "cmbTranslateUpdate");
                QObject::connect(pSelectMachine, SIGNAL(valChanged_Extruder(QString,QString,int)), this, SLOT(onValuechanged_Extruder(QString,QString,int)));
                QObject::connect(pSelectMachine, SIGNAL(extruderCntChanged(int)), this, SLOT(onExtruderCntChanged(int)));

            }
        }
        if (nullptr != pSelectMachine)
        {
            QString ProfileName = ProfileManager::instance().getcurrentProfileName();
            //SlicerManager::instance().setCurrentQuality(ProfileName);
            QQmlProperty::write(pSelectMachine, "profileName", ProfileName);
        }

        QObject* sObj = pRoot->findChild<QObject*>("printerManagerDlg");

        //QObject::connect(pRightPanel, SIGNAL(saveManagerConfig()), this, SLOT(onSaveManageConfig()));
        //bool res = QObject::connect(pSelectMachine, SIGNAL(machineChange(QString)), this, SLOT(onManagerMachineChanged(QString)));
        /*QObject::disconnect(pRightPanel, SIGNAL(resetManagerConfig()), this, SLOT(onManagerMachine()));
        QObject::disconnect(pRightPanel, SIGNAL(saveManagerConfig()), this, SLOT(onSaveManageConfig()));
        QObject::disconnect(pRightPanel, SIGNAL(managerMachineChange(QString)), this, SLOT(onManagerMachineChanged(QString)));
        QObject::connect(pRightPanel, SIGNAL(resetManagerConfig()), this, SLOT(onManagerMachine()));
        QObject::connect(pRightPanel, SIGNAL(saveManagerConfig()), this, SLOT(onSaveManageConfig()));
        QObject::connect(pRightPanel, SIGNAL(managerMachineChange(QString)), this, SLOT(onManagerMachineChanged(QString)));

        QObject::disconnect(pRightPanel, SIGNAL(saveExtruderConfig(int)), this, SLOT(onSaveExtruderConfig(int)));
        QObject::connect(pRightPanel, SIGNAL(saveExtruderConfig(int)), this, SLOT(onSaveExtruderConfig(int)));*/

    }
}

void ParameterSettingUI::invokeFunc(const char* func, QObject* report)
{
    if (!report)
    {
        QMetaObject::invokeMethod(m_qmlUI, func);
        return;
    }
    QMetaObject::invokeMethod(m_qmlUI, func, Q_ARG(QVariant, QVariant::fromValue(report)));
}

void ParameterSettingUI::onManagerMachineChanged(QString name)
{
    qDebug()<<"onManagerMachineChanged name==="<<name;
    if (name.isEmpty())
    {
        return;
    }
    //SlicerManager::instance().setCurrentMachine(name);

    m_file->copyBaseMachine(name);
    QStringList oldprofile = m_file->getAllfile(name);
    if (oldprofile.size() == 0)
    {
        m_file->copyBaseQuality(name);
    }
    //SlicerManager::instance().setCurrentMachine(name);

    us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings(name);

    QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();
    QObject* pRoot = engine->rootObjects().first();
    //QQmlProperty::write(pRightPanel, "currentMachineType", "FDM");
    QObject* pSelectMachine = pRoot->findChild<QObject*>("fdmMachineParemeter");
    //QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
    //if (pRightPanel)
    {
        //QObject* pSelectMachine = nullptr;
        std::map<QString, QString> value;
        /*if (m_file->getMachineType(name) == "DLP")
        {
            QQmlProperty::write(pRightPanel, "currentMachineType", "LD");
            pSelectMachine = pRightPanel->findChild<QObject*>("dlpMachineParemeter");
            if (pSelectMachine)
            {
                m_editprofile->getALLMachineprofile_DLP(value);

                for (const auto val : value)
                {
                    QQmlProperty::write(pSelectMachine, val.first, val.second);

                }
                QQmlProperty::write(pRightPanel, "managerDlpvisable", "true");
                QQmlProperty::write(pRightPanel, "managerFdmvisable", "false");
            }
        }
        else*/
        {
            /* QQmlProperty::write(pRightPanel, "currentMachineType", "FDM");
            pSelectMachine = pRightPanel->findChild<QObject*>("fdmMachineParemeter");*/
            if (pSelectMachine)
            {
                int extruderCnt = 0;
                m_editprofile->getALLMachineprofile(value);
                bool bExtruder = false;
                for (auto val : value)
                {
                    if (val.first == "machine_start_gcode" || val.first == "machine_end_gcode")
                    {
                        val.second = val.second.replace("\\n", "\n");
                    }

                    QQmlProperty::write(pSelectMachine, val.first, val.second);

                    QHash<QString, us::SettingItemDef*> hashItemDef = us::SettingDef::instance().getHashItemDef();
                    hashItemDef[val.first]->showValueStr = val.second;

                    if(val.first == "machine_extruder_count" && val.second.toInt() >= 1)
                    {
                        bExtruder = true;
                        extruderCnt = val.second.toInt();
                    }
                }
                /*       QQmlProperty::write(pRightPanel, "managerFdmvisable", "true");
                QQmlProperty::write(pRightPanel, "managerDlpvisable", "false");*/

                //do extruder data
                if(bExtruder)
                {
                    if (extruderCnt >= 1)
                    {
                        // m_extruderfile->getALLExtruderfile(value,0);
                        ExtruderProfile::instance()->getALLExtruderfile(value, 0);
                        for (auto val : value)
                        {
                            qDebug() << "bExtruder0 val.first =" << val.first;
                            qDebug() << "bExtruder0 val.second =" << val.second;
                            QString strTmp = val.second.replace("\\n", "\n");
                            QQmlProperty::write(pSelectMachine, val.first + "_0", strTmp);
                        }
                    }

                    if (extruderCnt >= 2)
                    {
                        //m_extruderfile->getALLExtruderfile(value,1);
                        ExtruderProfile::instance()->getALLExtruderfile(value, 1);
                        for (auto val : value)
                        {
                            qDebug() << "bExtruder1 val.first =" << val.first;
                            qDebug() << "bExtruder1 val.second =" << val.second;
                            QString strTmp = val.second.replace("\\n", "\n");
                            QQmlProperty::write(pSelectMachine, val.first + "_1", strTmp);
                        }
                    }
                }

                QQmlProperty::write(pSelectMachine, "currentTabIndex" ,"0");
            }
        }
    }

    SlicerManager::instance().setSliceParamDirty(true);
}

void ParameterSettingUI::slotLanguageChanged()
{
    //QObject* pRightPanel = m_qmlUI->findChild<QObject*>("RightInfoPanel");
    //if (pRightPanel)
    //{
    //    QMetaObject::invokeMethod(pRightPanel, "updateProfileLanguage");
    //}
    emit sigLanguageChanged();
}

QPointF ParameterSettingUI::getWindowPos()
{
    return QCursor::pos();
}

QString ParameterSettingUI::getNewProfileName()
{
    return QString();
}

QStringList ParameterSettingUI::getProfileList()
{
    QString strMachine = SlicerManager::instance().getCurrentMachine();
    QStringList oldprofile = m_file->getAllfile(strMachine);
    if (oldprofile.size() == 0)
    {
        m_file->copyBaseQuality(strMachine);
        oldprofile = m_file->getAllfile(strMachine);
    }
    return oldprofile;
}

int ParameterSettingUI::getCurProfileIndex()
{
    QStringList oldprofile = getProfileList();
    int index = oldprofile.indexOf(ProfileManager::instance().getcurrentProfileName());
    return index;
}

QStringList ParameterSettingUI::getMaterialList()
{
    QString strMachine = SlicerManager::instance().getCurrentMachine();
    QStringList materialprofile = m_file->getAllfile(strMachine, 1);
    return materialprofile;
}

int ParameterSettingUI::getCurMetrailIndex()
{
    QStringList materialprofile = getMaterialList(); 
    int index0 = materialprofile.size() > 1 ? 1 : 0;
    return index0;
}

QStringList ParameterSettingUI::getPrinterList()
{
    QStringList machines = us::ConfigurationManager::instance().getPresetMachineTypes();
    return machines;
}

int ParameterSettingUI::getCurPrinterIndex()
{
    QStringList machines = getPrinterList();
    if (machines.count() == 0)
        return 0;
    int currentIndex = machines.indexOf(SlicerManager::instance().getCurrentMachine());
    if (currentIndex < 0)
    {
        currentIndex = 0;
    }
    //SlicerManager::instance().setCurrentMachine(machines.at(currentIndex));
    return currentIndex;
}

int ParameterSettingUI::getMachineSeries(const QString& machinType)
{
    int series = 0;//1: fmd, 2: fdm-laser, 3: fdm-laser-cnc
    us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();
    QString machine_support_slicer_type = globalsetting->settings().value("machine_support_slicer_type")->value().toString();
    QStringList machine_support_types = machine_support_slicer_type.split("-");
    if(machine_support_types.size()>1)
    {
        if (machine_support_slicer_type == "FDM-LASER-CNC")
        {
            series = 3;
        }
        else if(machine_support_slicer_type == "FDM-LASER")
        {
            series = 2;
        }
    }
    else
    {
        series = 1;
    }
    return series;
}

bool ParameterSettingUI::deleteProfile(QString fileName)
{
    bool res = true;
    if (fileName == QString())
    {
        //删除当前选中
        res = onDeleteProfilebtn();
    }
    else {
        //删除对应名字的配置文件
    }
    return res;
}

QString ParameterSettingUI::getCurProfilePath(QString name)
{
    QString strPath;
    if (name == QString())
    {
        strPath = m_file->getCoachPath() + SlicerManager::instance().getCurrentMachine() + "/" + m_ProfileMode->getCusFileFullName();
    }
    else
    {
        strPath = m_file->getCoachPath() + SlicerManager::instance().getCurrentMachine() + "/" + name;
    }
    return strPath;
}

void ParameterSettingUI::resetProfile(const bool& isFactory)
{
    onResetbtn(isFactory);
}

bool ParameterSettingUI::isRepeatFileName(const QString &fileName)
{
    QStringList oldprofile = m_ProfileMode->profileList();
    QString msg = "new";
    bool res = false;

    //配置文件名格式变化了，要做适配
    for (const QString val : oldprofile)
    {
        if (val == fileName)
        {
            msg = "Existed";
            res = true;
            break;
        }
    }

    return res;
}

bool ParameterSettingUI::resetMachineParam(const QString& machineName)
{
    onManagerMachine(machineName, true);
    return true;
}

void ParameterSettingUI::initProfileList()
{
    m_file->updateProfileByMachineName(SlicerManager::instance().getCurrentMachine());
}

int ParameterSettingUI::getCurrentLanguage()
{
    QSettings setting;
    setting.beginGroup("language_perfer_config");
    QString lang = setting.value("language_type", "zh_cn").toString();
    setting.endGroup();

    if (lang == "en.ts")
    {
        return 0;
    }
    else if (lang == "zh_CN.ts")
    {
        return 1;
    }
    else if (lang == "zh_TW.ts")
    {
        return 2;
    }
    else
    {
        return 0;
    }
}


const QStringList &ParameterSettingUI::printerList() const
{
    return m_printerList;
}

void ParameterSettingUI::setPrinterList(const QStringList &newPrinterList)
{
    if (m_printerList == newPrinterList)
        return;
    m_printerList = newPrinterList;
    emit printerListChanged();
}

void ParameterSettingUI::initProfileListModel()
{
    QStringList fileList =  getProfileList();

}

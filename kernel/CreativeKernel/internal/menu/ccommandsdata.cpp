#include "ccommandsdata.h"
#include <QVariant>
#include "menu/ccommandlist.h"
#include <QVariantList>
#include <QMap>
#include <QString>
#include <QDebug>
#include "kernel/translator.h"

#include "def/settingdef.h"

#include "openfilecommand.h"
#include "submenurecentfiles.h"

#include "undoactioncmd.h"
#include "redoactioncmd.h"

#include "../menu_help/aboutuscommand.h"
#include "../menu_help/updatecommand.h"

#include "menuCmdDataType.h"
#include "../menu_view/modelshowcommand.h"
#include "../menu_view/mirroractioncommand.h"
#include "../menu_file/newprojectcommand.h"
#include "../menu_file/saveascommand.h"
#include "../menu_tool/submenulanguage.h"
#include "../menu_tool/managematerial.h"
#include "../menu_view/submenuviewsshow.h"
#include "../menu_view/resetactioncommand.h"
#include "../menu_view/mergemodelcommand.h"
#include "../menu_tool/manageprinter.h"
#include "../menu_tool/submenuThemeColor.h"
#include "../menu_tool/logview.h"
#include "../menu_group/crealitygroupcommand.h"
#include"../menu_help/usecoursecommand.h"
#include "../menu_help/userfeedbackcommand.h"
#include "../menu_printer_control/submenuprintercontrol.h"
#include "us/slicermanager.h"
#include "external/slice/parameterfile.h"
#include <QStandardPaths>

CCommandsData::CCommandsData(QObject *parent) : QObject(parent)
{
    addCommonCommand();
    createMenuNameMap();
    //menu need to do change language opreator
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}

void CCommandsData::slotLanguageChanged()
{
    m_mapMenuName.clear();
    //make the .ts file effect
    createMenuNameMap();
}
CCommandsData::~CCommandsData()
{}

void CCommandsData:: createMenuNameMap()
{
    //  int index = eMenuType_File;
    m_mapMenuName.clear();
    m_mapMenuName.insert(QString::number(eMenuType_File),tr("File(&F)"));
    m_mapMenuName.insert(QString::number(eMenuType_Edit),tr("Edit(&E)"));
    m_mapMenuName.insert(QString::number(eMenuType_View),tr("View(&V)"));
    //m_mapMenuName.insert(QString::number(eMenuType_Repair),tr("Repair(&R)"));
    //    m_mapMenuName.insert(QString::number(eMenuType_Slice),tr("Slice(&R)"));
    m_mapMenuName.insert(QString::number(eMenuType_Tool),tr("Tool(&T)"));
    m_mapMenuName.insert(QString::number(eMenuType_CrealityGroup),tr("Models(&M)"));
    m_mapMenuName.insert(QString::number(eMenuType_PrinterControl), tr("PrinterControl(&C)"));
    m_mapMenuName.insert(QString::number(eMenuType_Help),tr("Help(&H)"));

}
void CCommandsData::addCommads(ActionCommand *pAction)
{
    if (pAction)
    {
        m_varCommandList.push_back(pAction);
    }
}
QVariantList  CCommandsData::getCommandsList(void)
{
    QVariantList varList;
    varList.clear();
    for (int i = 0; i < m_varCommandList.count(); ++i)
    {
        varList.push_back(QVariant::fromValue(m_varCommandList.at(i)));
    }
    return varList;
}

QString CCommandsData::getMenuNameFromKey(QString key)
{
    //m_mapMenuName.value(key);
    return m_mapMenuName.value(key);
}
void CCommandsData::initMapData(QVariantMap &map)
{
    QVariantList fileList;
    QVariantList editList;
    QVariantList viewList;
    QVariantList repairList;
    QVariantList sliceList;
    QVariantList toolList;
    QVariantList groupList;
    QVariantList printerControlList;
    QVariantList helpList;
    map.insert(QString::number(eMenuType_File),fileList);
    map.insert(QString::number(eMenuType_Edit),editList);
    map.insert(QString::number(eMenuType_View),viewList);
    //map.insert(QString::number(eMenuType_Repair),repairList);
    //    map.insert(QString::number(eMenuType_Slice),sliceList);
    map.insert(QString::number(eMenuType_Tool),toolList);
    map.insert(QString::number(eMenuType_CrealityGroup),groupList);
    map.insert(QString::number(eMenuType_PrinterControl), printerControlList);
    map.insert(QString::number(eMenuType_Help),helpList);
}

void CCommandsData::addCommonCommand()
{
    qDebug() << "start add commands list";
    //file
    //NewProjectCommand* newPro = new NewProjectCommand();
    //CCommandList::getInstance()->addActionCommad(newPro);

    OpenFileCommand* openFile = new OpenFileCommand();
    openFile->setParent(this);
    CCommandList::getInstance()->addActionCommad(openFile);
    m_FileOpenOpt = openFile;

    SubMenuRecentFiles* rectfile = SubMenuRecentFiles::getInstance();
    rectfile->setParent(this);
    CCommandList::getInstance()->addActionCommad(rectfile);
    rectfile->setNumOfRecentFiles(10);

    SaveAsCommand* saveAs = new SaveAsCommand();
    saveAs->setParent(this);
    saveAs->setBSeparator(true);
    CCommandList::getInstance()->addActionCommad(saveAs);

    //edit
    UndoActionCmd* undo = new UndoActionCmd();
    undo->setParent(this);
    CCommandList::getInstance()->addActionCommad(undo);

    RedoActionCmd* redo = new RedoActionCmd();
    redo->setParent(this);
    CCommandList::getInstance()->addActionCommad(redo);

    //view
    ModelShowCommand* lineShow = new ModelShowCommand(eLineShow);
    lineShow->setParent(this);
    CCommandList::getInstance()->addActionCommad(lineShow);
    lineShow->setBSeparator(true);
    ModelShowCommand* faceShow = new ModelShowCommand(eFaceShow);
    faceShow->setParent(this);
    CCommandList::getInstance()->addActionCommad(faceShow);
    ModelShowCommand* faceLineShow = new ModelShowCommand(eFaceLineShow);
    faceLineShow->setParent(this);
    CCommandList::getInstance()->addActionCommad(faceLineShow);

    MirrorCommand* mirror_x = new MirrorCommand(eMirror_X);
    mirror_x->setParent(this);
    CCommandList::getInstance()->addActionCommad(mirror_x);
    mirror_x->setBSeparator(true);

    MirrorCommand* mirror_y = new MirrorCommand(eMirror_Y);
    mirror_y->setParent(this);
    CCommandList::getInstance()->addActionCommad(mirror_y);

    MirrorCommand* mirror_z = new MirrorCommand(eMirror_Z);
    mirror_z->setParent(this);
    CCommandList::getInstance()->addActionCommad(mirror_z);

    MirrorCommand* mirror_reset = new MirrorCommand(eMirroe_Reset);
    mirror_reset->setParent(this);
    CCommandList::getInstance()->addActionCommad(mirror_reset);

    SubMenuViewShow* viewshow = new SubMenuViewShow();
    viewshow->setParent(this);
    CCommandList::getInstance()->addActionCommad(viewshow);
    //    viewshow->setBSeparator(true);

    ResetActionCommand* resetAll = new ResetActionCommand();
    resetAll->setParent(this);
    CCommandList::getInstance()->addActionCommad(resetAll);

    MergeModelCommand* mergeModel = new MergeModelCommand();
    mergeModel->setParent(this);
    CCommandList::getInstance()->addActionCommad(mergeModel);
    resetAll->setBSeparator(true);

    //Tool
    SubMenuLanguage* lang = new SubMenuLanguage();
    lang->setParent(this);
    CCommandList::getInstance()->addActionCommad(lang);

    ManageMaterial* material = new ManageMaterial();
    material->setParent(this);
    CCommandList::getInstance()->addActionCommad(material);
    ManagePrinter* printer = new ManagePrinter();
    printer->setParent(this);
    CCommandList::getInstance()->addActionCommad(printer);
    SubMenuThemeColor* themecolor = new SubMenuThemeColor();
    themecolor->setParent(this);
    CCommandList::getInstance()->addActionCommad(themecolor);
    LogView* logview = new LogView();
    logview->setParent(this);
    CCommandList::getInstance()->addActionCommad(logview);

    //group
    CrealityGroupCommand* group = new CrealityGroupCommand();
    group->setParent(this);
    CCommandList::getInstance()->addActionCommad(group);

    //control
    SubMenuPrinterControl* printer_control = new SubMenuPrinterControl();
    printer_control->setParent(this);
    CCommandList::getInstance()->addActionCommad(printer_control);

    //help
    AboutUsCommand* aboutus = new AboutUsCommand();
    aboutus->setParent(this);
    CCommandList::getInstance()->addActionCommad(aboutus);
    UpdateCommand* update = new UpdateCommand();
    update->setParent(this);
    CCommandList::getInstance()->addActionCommad(update);
    UseCourseCommand* usecourse = new UseCourseCommand();
    usecourse->setParent(this);
    CCommandList::getInstance()->addActionCommad(usecourse);
    UserFeedbackCommand* userfeedback = new UserFeedbackCommand();
    userfeedback->setParent(this);
    CCommandList::getInstance()->addActionCommad(userfeedback);

    qDebug() << "add commands list over. total have add " << m_varCommandList.size() << "  commands";
}

QVariantMap CCommandsData::getCommandsMap(void)
{
    QVariantMap dataMap;
    dataMap.clear();

    initMapData(dataMap);
    //get actionCommand Data


    m_varCommandList = CCommandList::getInstance()->getActionCommandList();
    if(m_varCommandList.isEmpty())
    {
        return dataMap;
    }
    //data write into QVariantMap
    //QVariantMap tmpMap = dataMap;
    foreach (auto var, m_varCommandList)
    {
        QVariantList  list;
        int index = var->parentMenu();
        if(index > eMenuType_Help)continue;
        if(dataMap.isEmpty())
        {
            return dataMap;
        }
        if(dataMap.find(QString::number(index))->isNull())
        {
            list.append(QVariant::fromValue(var));
        }
        else
        {
            list = dataMap.find(QString::number(index)).value().value<QVariantList>();
            list.append(QVariant::fromValue(var));
        }
        dataMap.insert(QString::number(index),list);
    }

    //copy tmpMap data to map
    //    foreach(auto key,dataMap.keys())
    //    {
    //        if(!tmpMap.find(key)->isNull())
    //        {
    //            dataMap[key] = tmpMap.find(key).value().value<QVariantList>();
    //        }
    //    }

    return dataMap;
}


QVariantMap CCommandsData::getJsonMap(void)
{
    QVariantMap dataMap;
    dataMap.clear();

    QHash<QString, us::SettingItemDef*>::const_iterator it;//,it0λ
    for (it = us::SettingDef::instance().getHashItemDef().constBegin(); it != us::SettingDef::instance().getHashItemDef().constEnd(); ++it)
    {
        dataMap.insert(it.key(), QVariant::fromValue(it.value()));
    }

    return dataMap;
}


bool variantLessThan(const QVariant& v1, const QVariant& v2)
{
    return v1.toString() < v2.toString();
}

struct strcutItem
{
    QString qstrKey;
    int nOrder;
};

bool variantLessThanByOrder(strcutItem& v1, strcutItem& v2)
{
    return v1.nOrder > v2.nOrder;
}

QVariantList CCommandsData::getSpecialSettingKey()
{
    QVariantList values;
    USettings* userSetting = ProfileManager::instance().getSelectUSettings();

    for (const auto val : userSetting->settings())
    {
        QString qstrKey = val->key();

        if (qstrKey.contains("special_", Qt::CaseSensitive))
        {
            values.push_back(QVariant::fromValue(qstrKey));
        }
    }
    std::sort(values.begin(), values.end(), variantLessThan);

    return values;
}

void CCommandsData::deleteSpecialSetting()
{
    QVariantList values = getSpecialSettingKey();
    USettings* userSetting = ProfileManager::instance().getSelectUSettings();

    for (QVariant val : values)
    {
        userSetting->deleteValueByKey(val.toString());
    }
}

QVariantList CCommandsData::getGlobalSettingKey()
{
    QVariantList values;
    us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();
    QHash<QString, us::SettingItemDef*> hashItemDef = us::SettingDef::instance().getHashItemDef();
    USettings* userSetting = ProfileManager::instance().getSelectUSettings();

    ParameterFile fileSetting;
    QMap<QString, QString> mapFileSetting = fileSetting.getSettingFile();


    //
    for (const auto val : hashItemDef)
    {
        val->showValueStr = "";
    }
    
    //òlow.default / mid.default / high.default
    QMap<QString, QString>::const_iterator it;
    for (it = mapFileSetting.begin(); it != mapFileSetting.end(); ++it)
    {
        hashItemDef[it.key()]->showValueStr = it.value();
    }

    //
    QString strPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Machines/";
    us::ConfigurationManager::instance().setProfilePath(strPath);
    USettings* settings = MACHINECONF(SlicerManager::instance().getCurrentMachine());
    for (const auto val : settings->settings())
    {
        hashItemDef[val->key()]->showValueStr = val->value().toString();
    }


    //globalsettingkey
    for (const auto val : globalsetting->settings())
    {
        if (hashItemDef.find(val->key()) != hashItemDef.end())
        {
            values.push_back(QVariant::fromValue(val->key()));
        }
    }

    return values;
}

QVariantList CCommandsData::getClassTypeList()
{
    QVariantList values;
    QHash<QString, us::SettingItemDef*> hashItemDef = us::SettingDef::instance().getHashItemDef();
    std::vector<QString> vec;

    strcutItem item;
    std::vector<strcutItem> vecItemDef;

    QHash<QString, us::SettingItemDef*>::const_iterator it;
    for (it = hashItemDef.begin(); it != hashItemDef.end(); ++it)
    {
        if (std::find(vec.begin(), vec.end(), hashItemDef[it.key()]->classType) == vec.end() && hashItemDef[it.key()]->classType != "Machine" && hashItemDef[it.key()]->classType != "Command Line Settings")
        {
            vec.push_back(hashItemDef[it.key()]->classType);
            item.nOrder = hashItemDef[it.key()]->order;
            item.qstrKey = hashItemDef[it.key()]->classType;
            vecItemDef.push_back(item);
        }
    }
    std::sort(vecItemDef.begin(), vecItemDef.end(), variantLessThanByOrder);

    for (int i = 0; i < vecItemDef.size(); i++)
    {
        values.push_back(QVariant::fromValue(vecItemDef[i].qstrKey));
    }

    return values;
}

QVariantList CCommandsData::getAllSettingKey()
{
    QVariantList values;
    QHash<QString, us::SettingItemDef*> hashItemDef = us::SettingDef::instance().getHashItemDef();
    strcutItem item;
    std::vector<strcutItem> vecItemDef;

    QHash<QString, us::SettingItemDef*>::const_iterator it;
    for (it = hashItemDef.begin(); it != hashItemDef.end(); ++it)
    {
        if (hashItemDef[it.key()]->classType == "Machine" || hashItemDef[it.key()]->classType == "Command Line Settings")
        {
            continue;
        }

        item.qstrKey = it.key();
        item.nOrder = hashItemDef[it.key()]->order;
        vecItemDef.push_back(item);
    }

    std::sort(vecItemDef.begin(), vecItemDef.end(), variantLessThanByOrder);

    for (int i = 0; i < vecItemDef.size(); i++)
    {
        values.push_back(QVariant::fromValue(vecItemDef[i].qstrKey));
    }

    return values;
}

QVariant CCommandsData::getFileOpenOpt()
{
    return QVariant::fromValue(m_FileOpenOpt);
}

QVariant CCommandsData::getOpt(const QString& optName)
{
    QList<ActionCommand *> commandList = CCommandList::getInstance()->getActionCommandList();
    QVariant res;
    foreach(auto command, commandList)
    {
        if(command->nameWithout() == optName)
        {
            res = QVariant::fromValue(command);
            break;
        }
    }

    return res;
}

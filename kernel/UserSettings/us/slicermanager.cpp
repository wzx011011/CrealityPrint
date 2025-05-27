#include "slicermanager.h"
#include "extrudersetting.h"
#include "def/settingdef.h"
#include <QSettings>
#include <QtCore/QDebug>
//#include "profilemanager.h"
#include <QStandardPaths>
#include"ccglobal/log.h"


SlicerManager SlicerManager::s_slicerManager;
SlicerManager::SlicerManager(QObject *parent)
    : QObject(parent)
    , m_sliceParamDirty(false)
{
    //us::SettingDef::instance().initialize(":/us/default/fdm.def.json");
}
SlicerManager& SlicerManager::instance()
{
        return s_slicerManager;
}
void SlicerManager::setFDMSlicer(QObject *slicer)
{
    m_pDLPSlicerPlgin = slicer;
}
void SlicerManager::setDLPSlicer(QObject *slicer)
{
    m_pFDMSlicerPlgin = slicer;
}
void SlicerManager::setCurrentMachine(QString machineName)
{
    m_currentMachineName = machineName;

}

void SlicerManager::setCurrentMaterial(QString materialName)
{
    m_currentMaterial = materialName;
}

us::GlobalSetting* SlicerManager::globalsettings(const QString& machineName)
{
    qDebug()<<"globalsettings";
    if(m_globalSettings!=nullptr)
    {
        delete m_globalSettings;
    }

    m_globalSettings = new us::GlobalSetting(this);

	//qDebug() << (m_globalSettings->settings().find("machine_center_is_zero").value()->value().toString());

    //QString _strPath = ":/us/default/";
    //us::ConfigurationManager::instance().setProfilePath(_strPath);
    //m_globalSettings->merge(MACHINECONF(m_currentMachineName));

	//     //init load
	//QString strPath = ProfileManager::instance().getCoachPath() + SlicerManager::instance().getCurrentMachine() + "/";
    QString strPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Profiles/" + SlicerManager::instance().getCurrentMachine() + "/";
	us::ConfigurationManager::instance().setProfilePath(strPath);
	m_globalSettings->merge(ProfileManager::instance().getSelectUSettings());
	//m_globalSettings->merge(m_settingCache);

	strPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Machines/";
    us::ConfigurationManager::instance().setProfilePath(strPath);

    QString mName = machineName == QString() ? m_currentMachineName : machineName;
    m_globalSettings->merge(MACHINECONF(mName));

// 
    //strPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Materials/";
    //us::ConfigurationManager::instance().setProfilePath(strPath);
    //m_globalSettings->merge(MATERIALCONF(m_currentMaterial));

    return m_globalSettings;
}

us::GlobalSetting* SlicerManager::globalsettings_modify()
{
    return m_globalSettings;
}

us::SettingCache* SlicerManager::globalCache()
{
    return m_settingCache;
}

void SlicerManager::clearGlobalCache()
{
    m_settingCache->clear();
}

void SlicerManager::loadDefault()
{
	//us::SettingDef::instance().initialize(":/us/default/fdm.def.json");

    QSettings setting;
    setting.beginGroup("machine_setting");
        m_currentMachineName = setting.value("machine_name","Ender-3").toString();
        m_currentMaterial=setting.value("material","PLA").toString();
        m_currentQuality=setting.value("quality","middle").toString();
    setting.endGroup();
    m_settingCache->loadDefault("");
    m_globalSettings = new us::GlobalSetting(this);
    m_settingCache = new us::SettingCache(this);

    LOGI("SlicerManager::loadDefault() m_currentMachineName:[%s]",m_currentMachineName.toStdString().c_str());

    int extruderNum = MACHINECONF(m_currentMachineName)->settings().value("machine_extruder_count")->value().toInt();
	for (int n = 0; n < extruderNum; n++)
	{
		m_ExtruderSettingList.append(new us::ExtruderSetting(this));
	}
    //m_ExtruderSettingList.append(new us::ExtruderSetting(this));

	QString strPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Machines/";
	us::ConfigurationManager::instance().setProfilePath(strPath);
    //m_globalSettings->merge(MACHINECONF(m_currentMachineName));
	strPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Profiles/" + SlicerManager::instance().getCurrentMachine() + "/";
	us::ConfigurationManager::instance().setProfilePath(strPath);
    //m_globalSettings->merge(QUALITYCONF(m_currentQuality));
    //strPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Materials/";
    //us::ConfigurationManager::instance().setProfilePath(strPath);

}
void SlicerManager::setMachineData(QString machineName)
{
    m_globalSettings->merge(MACHINECONF(machineName));
}

////get the value of key
QString SlicerManager::getSetVal(QString datakey, QString defaultvalue)
{
    us::GlobalSetting* globalSettings = globalsettings_modify();
    return globalSettings->value(datakey, defaultvalue);
}

bool SlicerManager::isDLPMachine()
{
    return ProfileManager::instance().getMachineType(m_currentMachineName) == "DLP";
}

void SlicerManager::setCurrentQuality(QString qualityName)
{
    if (qualityName != nullptr)
    {
        m_globalSettings->merge(QUALITYCONF(qualityName));
    }
    else
    {
        qDebug() <<"CurrentQuality null" ;
    }     
}

void SlicerManager::setCurrentSlicerType(SLICERTYPE slicetype)
{
    m_currentSlicerType = slicetype;
    emit SliceTypeChanged(slicetype);
}


void SlicerManager::insertSettingCache(QString key, QString value)
{
    us::USetting* setting = us::SettingDef::instance().create(key, value);
    m_settingCache->insert(setting);
}

void SlicerManager::mergeSettingCache()
{
    m_globalSettings->merge(m_settingCache);
}

void SlicerManager::updateExtruder()
{
    //m_ExtruderSettingList.clear();
	int extruderNum = globalsettings()->settings().value("machine_extruder_count")->value().toInt();
	for (int n = m_ExtruderSettingList.size(); n < extruderNum; n++)
	{
		m_ExtruderSettingList.append(new us::ExtruderSetting(this));
	}
}

void SlicerManager::setDoingSlice(bool isDoing)
{
    m_isDoingSlice = isDoing;
}

void SlicerManager::setSliceParamDirty(bool dirty)
{
    m_sliceParamDirty = dirty;
}

bool SlicerManager::sliceParamDirty()
{
    return m_sliceParamDirty;
}

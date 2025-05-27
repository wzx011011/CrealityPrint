#include "configurationmanager.h"
#include <QDir>
#include "machinetypesetting.h"
#include <QSettings>
#include <QDebug>
#include<QCoreApplication>
#include"../../buildinfo.h"
#include"ccglobal/log.h"
#include "qtusercore/string/resourcesfinder.h"
#include"qtusercore/module/systemutil.h"


namespace us
{
	ConfigurationManager ConfigurationManager::s_configurationManager;
	ConfigurationManager::ConfigurationManager(QObject* parent)
		:QObject(parent)
	{
        m_configRoot = "";
        m_strPath = "";
	}
	ConfigurationManager::~ConfigurationManager()
	{
	}

	ConfigurationManager& ConfigurationManager::instance()
	{
		return s_configurationManager;
	}

    QString ConfigurationManager::getConfigRoot()
    {
       return qtuser_core::getOrCreateAppDataLocation("sliceconfig");
    }
    void ConfigurationManager::setPresetMachineType(QString machineName)
    {
        QStringList machines = instance().getPresetMachineTypes();
        if(machines.indexOf(machineName)>=0)
        {
            return;
        }
        machines.append(machineName);
          QSettings settings;
          //settings.beginGroup("Machine");
          settings.beginWriteArray("PresetMachines");
          for (int i = 0; i < machines.size(); ++i) {
              settings.setArrayIndex(i);
              settings.setValue("name", machines[i]);
          }
          settings.endArray();
          //settings.endGroup();
    }
    void ConfigurationManager::removeMachineType(QString machineName)
    {
        QStringList machines = instance().getPresetMachineTypes();
        if(machines.indexOf(machineName)<0)
        {
            return;
        }
        machines.removeOne(machineName);
        QSettings settings;
        QStringList recentFileList = settings.value("PresetMachines").toStringList();

        settings.remove("PresetMachines");
        settings.beginWriteArray("PresetMachines");
        for (int i = 0; i < machines.size(); ++i) {
            settings.setArrayIndex(i);
            settings.setValue("name", machines[i]);
        }
        settings.endArray();
    }
    void ConfigurationManager::setProfilePath(QString& strPath)
    {
		m_strPath = strPath;
    }

    QStringList ConfigurationManager::getPresetMachineTypes()
    {
          QStringList machines;
          QSettings settings;
          int size = settings.beginReadArray("PresetMachines");
          for (int i = 0; i < size; ++i) {
              settings.setArrayIndex(i);
              QString machine =settings.value("name").toString();
              if (!machine.isEmpty())
              {
                  machines.append(machine);
              }
          }
          settings.endArray();
          if (machines.size() == 0)
          {
              machines.append("Ender-3");
          }
          return machines;
    }

    QStringList ConfigurationManager::enumMachineTypes()
	{
        QStringList files;
        MachineTypeSetting machineTypes;
        std::unordered_map<std::string, std::string> types = machineTypes.GetMachineType();
        std::unordered_map<std::string, std::string>::iterator it = types.begin();
        while (it !=types.end()) {
            files.append(QString::fromStdString(it->first));
            it++;
        }
        return files;
	}

	USettings* ConfigurationManager::settingsFromMachineType(const QString& type)
	{
        //return search(type, m_machineConfigurations);
        //return search(type, m_qualityConfigurations);
        return search_modify(type, m_machineConfigurations);
	}

	QList<QString> ConfigurationManager::enumMaterialType()
	{
		return QList<QString>();
	}

	USettings* ConfigurationManager::settingsFromMaterialType(const QString& type)
	{
		return search(type, m_materialConfigurations);
	}

	QList<QString> ConfigurationManager::enumQualityType()
	{
		return QList<QString>();
	}

	USettings* ConfigurationManager::settingsFromQualityType(const QString& type)
	{
        //return search(type, m_qualityConfigurations);
        return search_modify(type, m_qualityConfigurations);
	}

	USettings* ConfigurationManager::search(const QString& name, QHash<QString, USettings*>& cache)
	{
		QHash<QString, USettings*>::iterator it = cache.find(name);
		if (it != cache.end())
			return it.value();         //find it

		//load
		//QString fileName = ":/us/default/" + name + ".default";
		QString fileName = m_strPath + name + ".default";
		USettings* settings = new USettings(this);
		settings->loadDefault(fileName);
		if (settings->settings().size() == 0)  // empty
		{
		}

		cache.insert(name, settings);   // insert
		return settings;
	}

    us::USettings* ConfigurationManager::search_modify(const QString& name, QHash<QString, USettings*>& cache)
    {
        QHash<QString, USettings*>::iterator it = cache.find(name);
        //load
        //QString fileName = ":/us/default/" + name + ".default";
        QString fileName = m_strPath + name + ".default";
        if (it != cache.end())
        {         
            it.value()->loadDefault(fileName);
            return it.value();
        }
        else
        {
            USettings* settings = new USettings(this);
            settings->loadDefault(fileName);
            if (settings->settings().size() == 0)  // empty
            {
                qDebug()<<"no key value";
            }
            cache.insert(name, settings);   // insert
            return settings;
        }
    }
}

#ifndef _US_CONFIGURATIONMANAGER_1590265134542_H
#define _US_CONFIGURATIONMANAGER_1590265134542_H
#include "usersettingsexport.h"
#include <QtCore/QHash>
#include "us/usettings.h"


#define CONF_MACHINE_PREFIX "machine"
#define CONF_MATERIAL_PREFIX "quality"

namespace us
{
	class USER_SETTINGS_API ConfigurationManager: public QObject
	{
		ConfigurationManager(QObject* parent = nullptr);
	public:
		virtual ~ConfigurationManager();

		static ConfigurationManager& instance();

        QStringList enumMachineTypes();
		USettings* settingsFromMachineType(const QString& type);
		QString getConfigRoot();

		QList<QString> enumMaterialType();
		USettings* settingsFromMaterialType(const QString& type);

		QList<QString> enumQualityType();
		USettings* settingsFromQualityType(const QString& type);
        QStringList getPresetMachineTypes();
        void setPresetMachineType(QString machineName);

		void setProfilePath(QString& strPath);

        void removeMachineType(QString machineName);
	protected:
		USettings* search(const QString& name, QHash<QString, USettings*>& cache);
		USettings* search_modify(const QString& name, QHash<QString, USettings*>& cache);
	protected:
		static ConfigurationManager s_configurationManager;

		QHash<QString, USettings*> m_machineConfigurations;
		QHash<QString, USettings*> m_materialConfigurations;
		QHash<QString, USettings*> m_qualityConfigurations;

		QString m_strPath;
		QString m_configRoot;
	};
}

#define MACHINECONF(x) us::ConfigurationManager::instance().settingsFromMachineType(x)
#define MATERIALCONF(x) us::ConfigurationManager::instance().settingsFromMaterialType(x)
#define QUALITYCONF(x) us::ConfigurationManager::instance().settingsFromQualityType(x)
#endif // _US_CONFIGURATIONMANAGER_1590265134542_H

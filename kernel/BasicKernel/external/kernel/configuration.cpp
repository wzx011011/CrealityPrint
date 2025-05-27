#include "configuration.h"
#include <QtCore/QSettings>
#include <QtCore/QtCore>

namespace creative_kernel
{
	SINGLETON_IMPL(Configuration)
	Configuration::Configuration()
		:QObject()
	{
		m_defaultIni = QCoreApplication::applicationDirPath() + QString("/c3d.ini");
	}
	
	Configuration::~Configuration()
	{
	}

	void Configuration::saveGlobal(const QString& key, const QVariant& value)
	{
		QSettings settings(m_defaultIni, QSettings::IniFormat);
		settings.setValue(key, value);

	}

	QVariant Configuration::loadGlobal(const QString& key, const QVariant& defaultValue)
	{
		QSettings settings(m_defaultIni, QSettings::IniFormat);
		return settings.value(key, defaultValue);
	}
}
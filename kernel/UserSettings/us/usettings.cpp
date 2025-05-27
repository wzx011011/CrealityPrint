#include "us/usettings.h"
#include "def/defaultloader.h"
#include <QFile>
#include <QDebug>
#include"ccglobal/log.h"
namespace us
{
	USettings::USettings(QObject* parent)
		:QObject(parent)
	{
	}

	USettings::~USettings()
	{
	}

	void USettings::insert(USetting* setting)
	{
		if (setting)
		{
			setting->setParent(this);
			QHash<QString, us::USetting*>::iterator insertIt = m_hashsettings.find(setting->key());
			if (insertIt != m_hashsettings.end())
			{
				insertIt.value() = setting;
			}
			else
			{
				m_hashsettings.insert(setting->key(), setting);
			}
		}
	}

	void USettings::merge(USettings* settings)
	{
		if (settings)
		{
			const QHash<QString, USetting*>& mergeSettings = settings->m_hashsettings;
			for (QHash<QString, us::USetting*>::const_iterator it = mergeSettings.begin(); it != mergeSettings.end(); ++it)
			{
				QHash<QString, us::USetting*>::iterator insertIt = m_hashsettings.find(it.key());
				if (insertIt != m_hashsettings.end())
				{
					insertIt.value()->setValue(it.value()->value());   // replace value
				}
				else
				{
					USetting* setting = it.value()->clone();
					setting->setParent(this);
					m_hashsettings.insert(it.key(), setting);     //insert
				}
			}
		}
	}

	void USettings::mergeNewItem(USettings* settings)
	{
		if (settings)
		{
			const QHash<QString, USetting*>& mergeSettings = settings->m_hashsettings;
			for (QHash<QString, us::USetting*>::const_iterator it = mergeSettings.begin(); it != mergeSettings.end(); ++it)
			{
				QHash<QString, us::USetting*>::iterator insertIt = m_hashsettings.find(it.key());
				if (insertIt != m_hashsettings.end())
				{
					//insertIt.value()->setValue(it.value()->value());   // replace value
				}
				else
				{
					USetting* setting = it.value()->clone();
					setting->setParent(this);
					m_hashsettings.insert(it.key(), setting);     //insert
				}
			}
		}
	}


    void USettings::clear()
    {
        m_hashsettings.clear();
    }
	
	void USettings::deleteValueByKey(QString key)
	{
		m_hashsettings.remove(key);
	}

    const QHash<QString, USetting*>& USettings::settings() const
	{
		return m_hashsettings;
	}

	void USettings::loadDefault(const QString& file)
	{
		
		DefaultLoader loader;
		loader.loadDefault(file, this);
	}

	QString USettings::value(const QString& key, const QString& defaultValue) const
	{
		QString valueStr = defaultValue;
		auto it = m_hashsettings.find(key);
		if (it != m_hashsettings.end())
		{
			valueStr = it.value()->value().toString();
		}
		else
		{
			qDebug() << "USettings value" << key << " failed ! use default value " << defaultValue;
		}
		
		return std::move(valueStr);
	}
	void USettings::update(const USettings* settings)
	{
		if (!settings) return;
		for (auto& ms : m_hashsettings)
		{
			QString val = settings->value(ms->key(), "");
			if (val != "")
			{
				ms->setValue(std::move(val));
			}
		}
	}
    void USettings::save(const QString& fileName)
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            file.close();
        }
    }
}

#include "def/settingdef.h"
#include "def/jsondef.h"
#include "us/usetting.h"

#include <functional>

#include <QtCore/QDebug>
namespace us
{
	SettingDef SettingDef::s_settingDef;
	SettingDef::SettingDef(QObject* parent)
		:QObject(parent)
	{
		//initialize(":/us/default/fdm.def.json");
	}

	SettingDef::~SettingDef()
	{
	}

	SettingDef& SettingDef::instance()
	{
		return s_settingDef;
	}

	USetting* SettingDef::create(const QString& key)
	{
		QHash<QString, SettingItemDef*>::iterator it = m_hashItemDef.find(key);
		if (it != m_hashItemDef.end())
		{
			USetting* setting = new USetting(it.value());
			return setting;
		}

		//qDebug() << "Unkown setting key " << key;
		return nullptr;
	}

	USetting* SettingDef::create(const QString& key, const QString& value)
	{
		USetting* setting = create(key);
		if (setting) setting->setValue(value);
		return setting;
	}

	void SettingDef::initialize(const QString& file)
	{
		m_hashItemDef.clear();
		for (SettingGroupDef* def : m_settingGroupDefs)
			delete def;
		m_settingGroupDefs.clear();

		JsonDef jsonDef;
		jsonDef.parse(file, m_settingGroupDefs, this);

		buildHash();

		qDebug() << "settings initialize over";
	}

	void SettingDef::buildHash()
	{
		std::function<void(SettingItemDef* itemDef)> buildFunc;
		buildFunc = [this, &buildFunc](SettingItemDef* itemDef) {
#ifdef _DEBUG
			QHash<QString, SettingItemDef*>::iterator it = m_hashItemDef.find(itemDef->name);
			if (it != m_hashItemDef.end())
			{
				qDebug() << "same keys setting item.";
			}
#endif
			m_hashItemDef.insert(itemDef->name, itemDef);

			for (SettingItemDef* childItemDef : itemDef->items)
				buildFunc(childItemDef);
		};

		for(SettingGroupDef* groupDef : m_settingGroupDefs)
		{
			for (SettingItemDef* itemDef : groupDef->items)
			{
				buildFunc(itemDef);
			}
		}

		for (SettingItemDef* itemSetting : m_hashItemDef)
		{
			itemSetting->process();
		}
	}

	QHash<QString, SettingItemDef*> SettingDef::getHashItemDef()
	{
		return m_hashItemDef;
	}
}

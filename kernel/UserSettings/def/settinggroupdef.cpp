#include "def/settinggroupdef.h"

namespace us
{
	SettingGroupDef::SettingGroupDef(QObject* parent)
		:QObject(parent)
	{
	}

	SettingGroupDef::~SettingGroupDef()
	{
	}

	void SettingGroupDef::addSettingItemDef(SettingItemDef* settingItemDef)
	{
		items.push_back(settingItemDef);
	}
}

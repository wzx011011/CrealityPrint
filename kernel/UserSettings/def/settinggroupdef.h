#ifndef _US_SETTINGGROUPDEF_1589438667491_H
#define _US_SETTINGGROUPDEF_1589438667491_H
#include "usersettingsexport.h"
#include "def/settingitemdef.h"

namespace us
{
	class USER_SETTINGS_API SettingGroupDef: public QObject
	{
	public:
		SettingGroupDef(QObject* parent = nullptr);
		virtual ~SettingGroupDef();

		void addSettingItemDef(SettingItemDef* settingItemDef);

		QList<SettingItemDef*> items;
		QString label;
		QString description;
		QString type;
	};
}
#endif // _US_SETTINGGROUPDEF_1589438667491_H

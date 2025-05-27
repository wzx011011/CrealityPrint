#ifndef _US_SETTINGDEF_1589436632667_H
#define _US_SETTINGDEF_1589436632667_H
#include "usersettingsexport.h"
#include "def/settinggroupdef.h"
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QHash>

namespace us
{
	class USetting;
	class USER_SETTINGS_API SettingDef: public QObject
	{
		SettingDef(QObject* parent = nullptr);
	public:
		virtual ~SettingDef();

		static SettingDef& instance();

		USetting* create(const QString& key);
		USetting* create(const QString& key, const QString& value);

		void initialize(const QString& file);

		QHash<QString, SettingItemDef*> getHashItemDef();

	private:
		void buildHash();
	protected:
		static SettingDef s_settingDef;

		QMap<QString, SettingGroupDef*> m_settingGroupDefs;
		QHash<QString, SettingItemDef*> m_hashItemDef;
	};
}

#define SETTING(x) us::SettingDef::instance().create(x)
#define SETTING2(x, y) us::SettingDef::instance().create(x, y)
#endif // _US_SETTINGDEF_1589436632667_H

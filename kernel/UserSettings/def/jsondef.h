#ifndef _US_JSONDEF_1589439749372_H
#define _US_JSONDEF_1589439749372_H
#include "usersettingsexport.h"
#include "def/settinggroupdef.h"
#include <QtCore/QMap>

namespace us
{
	class USER_SETTINGS_API JsonDef: public QObject
	{
	public:
		JsonDef(QObject* parent = nullptr);
		virtual ~JsonDef();

		void parse(const QString& file, QMap<QString, SettingGroupDef*>& defs, QObject* parent);
	};
}
#endif // _US_JSONDEF_1589439749372_H

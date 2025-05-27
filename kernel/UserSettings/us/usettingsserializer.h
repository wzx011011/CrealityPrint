#ifndef _US_USETTINGSSERIALIZER_1589417303819_H
#define _US_USETTINGSSERIALIZER_1589417303819_H
#include "usersettingsexport.h"

namespace us
{
	class USettings;
	class USER_SETTINGS_API USettingsSerializer: public QObject
	{
	public:
		USettingsSerializer(QObject* parent = nullptr);
		virtual ~USettingsSerializer();

		static void load(USettings* settings, const QString& file);

	protected:
		static void loadFromIni();
	};
}
#endif // _US_USETTINGSSERIALIZER_1589417303819_H

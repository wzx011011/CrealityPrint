#ifndef _US_MODELLISTSETTING_1589352306445_H
#define _US_MODELLISTSETTING_1589352306445_H
#include "usersettingsexport.h"
#include "us/usettings.h"

namespace us
{
	class USER_SETTINGS_API ModelListSetting : public USettings
	{
	public:
		ModelListSetting(QObject* parent = nullptr);
		~ModelListSetting();
	};
}
#endif // _US_MODELLISTSETTING_1589352306445_H

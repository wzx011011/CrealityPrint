#ifndef _US_GLOBALSETTING_1589336489314_H
#define _US_GLOBALSETTING_1589336489314_H
#include "usersettingsexport.h"
#include "us/usettings.h"

namespace us
{
	class USER_SETTINGS_API GlobalSetting: public USettings
	{
	public:
		GlobalSetting(QObject* parent = nullptr);
		virtual ~GlobalSetting();
	};
}
#endif // _US_GLOBALSETTING_1589336489314_H

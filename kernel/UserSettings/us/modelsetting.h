#ifndef _US_MODELSETTING_1589252221535_H
#define _US_MODELSETTING_1589252221535_H
#include "usersettingsexport.h"
#include "us/usettings.h"

namespace us
{
	class USER_SETTINGS_API ModelSetting : public USettings
	{
	public:
		ModelSetting(QObject* parent = nullptr);
		~ModelSetting();
	};
}
#endif // _US_MODELSETTING_1589252221535_H

#ifndef _US_EXTRUDERSETTING_1589252221550_H
#define _US_EXTRUDERSETTING_1589252221550_H
#include "usersettingsexport.h"
#include "us/usettings.h"
namespace us
{
	class USER_SETTINGS_API ExtruderSetting:public USettings
	{
	public:
		ExtruderSetting(QObject* parent = nullptr);
		virtual ~ExtruderSetting();
	};
}
#endif // _US_EXTRUDERSETTING_1589252221550_H

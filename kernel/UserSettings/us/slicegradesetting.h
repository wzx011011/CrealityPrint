#ifndef _US_SLICEGRADESETTING_1589879654022_H
#define _US_SLICEGRADESETTING_1589879654022_H
#include "usersettingsexport.h"
#include "us/usettings.h"
#include"us/configurationmanager.h"

namespace us
{
	class USER_SETTINGS_API SliceGradeSetting: public USettings
	{
	public:
		SliceGradeSetting(QObject* parent = nullptr);
		virtual ~SliceGradeSetting();

		void getSliceGrade(QString name= us::ConfigurationManager::instance().getConfigRoot() + "/default/middle.default");//high middle low
	};
}
#endif // _US_SLICEGRADESETTING_1589879654022_H

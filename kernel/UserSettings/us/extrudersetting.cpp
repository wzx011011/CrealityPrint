#include "us/extrudersetting.h"
#include"us/configurationmanager.h"
namespace us
{
	ExtruderSetting::ExtruderSetting(QObject* parent)
	{
		loadDefault(us::ConfigurationManager::instance().getConfigRoot() + "/default/extrudersettings.default");
	}
	ExtruderSetting::~ExtruderSetting()
	{
	}
}

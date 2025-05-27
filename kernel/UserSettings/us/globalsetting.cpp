#include "globalsetting.h"
#include"us/configurationmanager.h"
#include"ccglobal/log.h"
namespace us
{
	GlobalSetting::GlobalSetting(QObject* parent)
	{
		QString filePath = us::ConfigurationManager::instance().getConfigRoot() + "/default/globalsettings.default";
		LOGI("GlobalSetting::GlobalSetting  begin LoadDefault file:[%s] ", filePath.toStdString().c_str());
		loadDefault(filePath);
	}
	GlobalSetting::~GlobalSetting()
	{
	}
}

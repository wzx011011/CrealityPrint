#include "us/modelsetting.h"
#include"us/configurationmanager.h"
namespace us
{
	ModelSetting::ModelSetting(QObject* parent)
		:USettings(parent)
	{
		loadDefault(us::ConfigurationManager::instance().getConfigRoot() + "/default/modelsettings.default");
	}

	ModelSetting::~ModelSetting()
	{
	}
}

#include "modellistsetting.h"
#include"us/configurationmanager.h"
namespace us
{
	ModelListSetting::ModelListSetting(QObject* parent)
		:USettings(parent)
	{
		loadDefault(us::ConfigurationManager::instance().getConfigRoot() + "/default/modellistsettings.default");
	}

	ModelListSetting::~ModelListSetting()
	{
	}
}

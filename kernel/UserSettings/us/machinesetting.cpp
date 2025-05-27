#include "machinesetting.h"
namespace us
{
	MachineSetting::MachineSetting(QObject* parent)
	{
	}
	MachineSetting::~MachineSetting()
	{
	}
	void MachineSetting::getMachineSetting(QString name)
	{
		loadDefault(name);
	}
}

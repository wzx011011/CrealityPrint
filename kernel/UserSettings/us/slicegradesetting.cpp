#include "slicegradesetting.h"
namespace us
{
	SliceGradeSetting::SliceGradeSetting(QObject* parent)
	{
	}
	SliceGradeSetting::~SliceGradeSetting()
	{
	}
	void SliceGradeSetting::getSliceGrade(QString name)
	{
		loadDefault(name);
	}
}

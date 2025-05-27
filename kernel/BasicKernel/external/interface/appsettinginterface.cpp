#include "appsettinginterface.h"
#include "kernel/abstractkernel.h"

namespace creative_kernel
{
	QColor getSettingColor(QString tag)
	{
		return AbstractKernelUI::getSelf()->getAppSettings()->getColor(tag);
	}

	QVector4D getSettingVector4D(QString tag)
	{
		return AbstractKernelUI::getSelf()->getAppSettings()->getVector4D(tag);
	}

	QVariantList getShaderSetting(QString tag)
	{
		return AbstractKernelUI::getSelf()->getAppSettings()->getShaderSetting(tag);
	}
}
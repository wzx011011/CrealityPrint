#ifndef CREATIVE_KERNEL_APP_SETTING_INTERFACE_1593676766939_H
#define CREATIVE_KERNEL_APP_SETTING_INTERFACE_1593676766939_H
#include "basickernelexport.h"
#include <QColor>
#include <QString>

namespace creative_kernel
{
	BASIC_KERNEL_API QColor getSettingColor(QString tag);
	BASIC_KERNEL_API QVector4D getSettingVector4D(QString tag);
	BASIC_KERNEL_API QVariantList getShaderSetting(QString tag);

}
#endif // CREATIVE_KERNEL_APP_SETTING_INTERFACE_1593676766939_H
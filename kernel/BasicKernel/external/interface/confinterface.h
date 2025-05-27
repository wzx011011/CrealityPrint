#ifndef _CONFINTERFACE_1603956346303_H
#define _CONFINTERFACE_1603956346303_H
#include "basickernelexport.h"
#include <QtCore/QVariant>

namespace creative_kernel
{
	BASIC_KERNEL_API void c3dSaveConfS(const QString& key, const QString& value);
	BASIC_KERNEL_API QString c3dLoadConfS(const QString& key);
}

#endif // _CONFINTERFACE_1603956346303_H
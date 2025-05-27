#include "confinterface.h"
#include "kernel/configuration.h"

namespace creative_kernel
{
	void c3dSaveConfS(const QString& key, const QString& value)
	{
		SAVECONF(key, value);
	}

	QString c3dLoadConfS(const QString& key)
	{
		return LOADCONF(key, QString()).toString();
	}
}
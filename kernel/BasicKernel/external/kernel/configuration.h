#ifndef CREATIVE_KERNEL_CONFIGURATION_1603955508504_H
#define CREATIVE_KERNEL_CONFIGURATION_1603955508504_H
#include "basickernelexport.h"
#include "qtusercore/module/singleton.h"
#include <QtCore/QVariant>

namespace creative_kernel
{
	class BASIC_KERNEL_API Configuration : public QObject
	{
		Q_OBJECT
		SINGLETON_DECLARE(Configuration)
	public:
		virtual ~Configuration();

		void saveGlobal(const QString& key, const QVariant& value);
		QVariant loadGlobal(const QString& key, const QVariant & = QVariant());
	protected:
		QString m_defaultIni;
	};
	
	SINGLETON_EXPORT(BASIC_KERNEL_API, Configuration)
}

#define SAVECONF(x, y) creative_kernel::getConfiguration()->saveGlobal(x, y)
#define LOADCONF(x, y) creative_kernel::getConfiguration()->loadGlobal(x, y)
#endif // CREATIVE_KERNEL_CONFIGURATION_1603955508504_H
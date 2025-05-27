#ifndef CREATIVE_KERNEL_ABSTRACT_APP_SETTING_1603955508504_H
#define CREATIVE_KERNEL_ABSTRACT_APP_SETTING_1603955508504_H
#include "basickernelexport.h"
#include <QtCore/QVariant>
#include <QColor>
#include <QVector4D>
#include <QtCore/QVariantList>

namespace creative_kernel
{
	class AbstractAppSetting /*: public QObject*/
	{
		//Q_OBJECT
	public:

		virtual QColor getColor(QString tag) = 0;
		virtual QVector4D getVector4D(QString tag) = 0;

		virtual QVariantList getShaderSetting(QString tag) = 0;

	};
	
}

#endif // CREATIVE_KERNEL_ABSTRACT_APP_SETTING_1603955508504_H
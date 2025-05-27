#ifndef CREATIVE_KERNEL_APP_SETTING_1603955508504_H
#define CREATIVE_KERNEL_APP_SETTING_1603955508504_H
#include "creativekernelexport.h"
#include "kernel/abstractappsetting.h"
#include "qtusercore/module/singleton.h"
#include <QtCore/QVariant>
#include <QColor>
#include <QVector4D>
#include <QtCore/QVariantList>

namespace creative_kernel
{
	class AppSetting : public AbstractAppSetting
	{
		//Q_OBJECT
	public:
		AppSetting();
		virtual ~AppSetting();

		QColor getColor(QString tag) override;
		QVector4D getVector4D(QString tag) override;

		QVariantList getShaderSetting(QString tag) override;

	};
	
}

#endif // CREATIVE_KERNEL_APP_SETTING_1603955508504_H
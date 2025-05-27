#ifndef _CREATIVE_KERNEL_SOFTWAREFUNCTION_1594621223172_H
#define _CREATIVE_KERNEL_SOFTWAREFUNCTION_1594621223172_H
#include "basickernelexport.h"
#include <QtCore/QObject>

class BASIC_KERNEL_API SoftwareFunction: public QObject
{
	Q_OBJECT
public:
	SoftwareFunction(QObject* parent = nullptr);
	virtual ~SoftwareFunction();

	Q_INVOKABLE void getData(QString name, QString value);

	bool modelAdaption();

private:
	bool m_modelAdaption;//模型自适应平台大小
};

#endif // _CREATIVE_KERNEL_SOFTWAREFUNCTION_1594621223172_H

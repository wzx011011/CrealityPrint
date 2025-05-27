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
	bool m_modelAdaption;//ģ������Ӧƽ̨��С
};

#endif // _CREATIVE_KERNEL_SOFTWAREFUNCTION_1594621223172_H

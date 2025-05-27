#ifndef CREATIVE_KERNEL_CXXGLOBAL_1610004301083_H
#define CREATIVE_KERNEL_CXXGLOBAL_1610004301083_H
#include "creativekernelexport.h"
#include <QtCore/QObject>

class CREATIVE_KERNEL_API CXXGlobal : public QObject
{
	Q_OBJECT
public:
	CXXGlobal(QObject* parent = nullptr);
	virtual ~CXXGlobal();

	Q_INVOKABLE void previewMain();
};

#endif // CREATIVE_KERNEL_CXXGLOBAL_1610004301083_H
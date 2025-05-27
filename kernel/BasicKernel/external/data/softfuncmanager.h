#ifndef _NULLSPACE_SOFTFUNCMANAGER_1594718209260_H
#define _NULLSPACE_SOFTFUNCMANAGER_1594718209260_H
#include <QtCore/QObject>
#include "softwarefunction.h"

class BASIC_KERNEL_API softFuncManager: public QObject
{
	Q_OBJECT
public:
	explicit softFuncManager(QObject* parent = nullptr);
	static softFuncManager& instance();
	SoftwareFunction* GetSoftFunc();

	virtual ~softFuncManager();

protected:
	static softFuncManager s_Manager;

private:
	SoftwareFunction* m_SoftFunc;
};
#endif // _NULLSPACE_SOFTFUNCMANAGER_1594718209260_H

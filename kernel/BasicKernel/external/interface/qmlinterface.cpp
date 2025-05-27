#include "qmlinterface.h"
#include "qtusercore/module/jobexecutor.h"
#include "external/kernel/tips.h"
#include "kernel/kernel.h"

namespace creative_kernel
{
	QObject* qmlJobExecutor()
	{
		return getKernel()->jobExecutor();
	}

	QObject* qmlTips()
	{
		return getKernel()->tips();
	}
}
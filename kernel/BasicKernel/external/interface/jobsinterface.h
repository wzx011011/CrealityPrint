#ifndef _JOBSINTERFACE_1592716998224_H
#define _JOBSINTERFACE_1592716998224_H
#include "basickernelexport.h"
#include "qtusercore/module/jobexecutor.h"

using namespace qtuser_core;
namespace creative_kernel
{
	BASIC_KERNEL_API bool jobExecutorAvaillable();
	BASIC_KERNEL_API bool executeJobs(QList<JobPtr> jobs);
	BASIC_KERNEL_API bool executeJob(JobPtr job);
}
#endif // _JOBSINTERFACE_1592716998224_H
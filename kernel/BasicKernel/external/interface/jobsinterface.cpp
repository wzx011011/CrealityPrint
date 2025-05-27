#include "jobsinterface.h"
#include "external/kernel/kernel.h"

namespace creative_kernel
{
	bool jobExecutorAvaillable()
	{
		return getKernel()->jobExecutor()->isRunning() == false;
	}

	bool executeJobs(QList<JobPtr> jobs)
	{
		return getKernel()->jobExecutor()->execute(jobs);
	}

	bool executeJob(JobPtr job)
	{
		return getKernel()->jobExecutor()->execute(job);
	}
}
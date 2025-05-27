#ifndef _CREATIVE_KERNEL_MESHLOADJOB_1590984808257_H
#define _CREATIVE_KERNEL_MESHLOADJOB_1590984808257_H
#include "basickernelexport.h"
#include "qtusercore/module/job.h"

class MeshLoadInfo;
namespace creative_kernel
{
	class BASIC_KERNEL_API MeshLoadJob: public qtuser_core::Job
	{
	public:
		MeshLoadJob(QObject* parent = nullptr);
		virtual ~MeshLoadJob();

		MeshLoadInfo* m_meshLoadInfo;
	protected:
		QString name();
		QString description();
		void failed();                        // invoke from main thread
		void successed(qtuser_core::Progressor* progressor);                     // invoke from main thread
		void work(qtuser_core::Progressor* progressor);    // invoke from worker thread
	};
}
#endif // _CREATIVE_KERNEL_MESHLOADJOB_1590984808257_H

#ifndef _CREATIVE_KERNEL_REMOVEDUMPLICATEVERTEXJOB_1591623850305_H
#define _CREATIVE_KERNEL_REMOVEDUMPLICATEVERTEXJOB_1591623850305_H
#include "basickernelexport.h"
#include "qtusercore/module/job.h"

class MeshLoadInfo;
namespace creative_kernel
{
	class BASIC_KERNEL_API RemoveDumplicateVertexJob: public qtuser_core::Job
	{
	public:
		RemoveDumplicateVertexJob(QObject* parent = nullptr);
		virtual ~RemoveDumplicateVertexJob();

		MeshLoadInfo* m_meshLoadInfo;
	protected:
		QString name();
		QString description();
		void failed();                        // invoke from main thread
		void successed(qtuser_core::Progressor* progressor);                     // invoke from main thread
		void work(qtuser_core::Progressor* progressor);    // invoke from worker thread

		void hash_(qtuser_core::Progressor* progressor);
	};
}
#endif // _CREATIVE_KERNEL_REMOVEDUMPLICATEVERTEXJOB_1591623850305_H

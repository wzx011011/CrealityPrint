#ifndef _CREATIVE_KERNEL_GEOMETRY_GEN_JOB_H
#define _CREATIVE_KERNEL_GEOMETRY_GEN_JOB_H
#include "qtusercore/module/job.h"
#include "basickernelexport.h"

class MeshLoadInfo;


namespace creative_kernel
{

	class BASIC_KERNEL_API GeometryGenJob: public qtuser_core::Job
	{
	public:
		GeometryGenJob(QObject* parent = nullptr);
		virtual ~GeometryGenJob();

		MeshLoadInfo* m_meshLoadInfo;
		bool m_needLayout;
	protected:
		QString name();
		QString description();
		void failed();                        // invoke from main thread
		void successed(qtuser_core::Progressor* progressor);                     // invoke from main thread
		void work(qtuser_core::Progressor* progressor);    // invoke from worker thread

	};
}
#endif // _CREATIVE_KERNEL_MESHLOADJOB_1590984808257_H

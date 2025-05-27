#ifndef _CREATIVE_KERNEL_CALCULATENORMALJOB_1591097818427_H
#define _CREATIVE_KERNEL_CALCULATENORMALJOB_1591097818427_H
#include "basickernelexport.h"
#include <QtCore/QObject>
#include "qtusercore/module/job.h"

class MeshLoadInfo;
namespace creative_kernel
{
	class BASIC_KERNEL_API CalculateNormalJob: public qtuser_core::Job
	{
	public:
		CalculateNormalJob(QObject* parent = nullptr);
		virtual ~CalculateNormalJob();

		MeshLoadInfo* m_meshLoadInfo;
	protected:
		QString name();
		QString description();
		void failed();                        // invoke from main thread
		void successed(qtuser_core::Progressor* progressor);                     // invoke from main thread
		void work(qtuser_core::Progressor* progressor);    // invoke from worker thread
	protected:
	};
}
#endif // _CREATIVE_KERNEL_CALCULATENORMALJOB_1591097818427_H

#ifndef CREATIVE_KERNEL_MACHINESELECTOR_1595659762117_H
#define CREATIVE_KERNEL_MACHINESELECTOR_1595659762117_H
#include "basickernelexport.h"

namespace qtuser_3d
{
	class MachineTracer;
}

namespace creative_kernel
{
	class ModelN;
	class BASIC_KERNEL_API MachineSelector : public QObject
	{
		Q_OBJECT
	public:
		MachineSelector(QObject* parent = nullptr);
		virtual ~MachineSelector();

		void addTracer(qtuser_3d::MachineTracer* tracer);
		void removeTracer(qtuser_3d::MachineTracer* tracer);

		void onMachineChanged(QString oldmachine, QString newmachine);
		void onMachineTypeChanged(int oldmachinetype, int newmachinetype);

	protected:
		
		QList<qtuser_3d::MachineTracer*> m_selectorTracers;

	};
}
#endif // CREATIVE_KERNEL_MACHINESELECTOR_1595659762117_H
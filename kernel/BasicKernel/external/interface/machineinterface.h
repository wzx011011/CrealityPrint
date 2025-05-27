#ifndef CREATIVE_KERNEL_SUPPORTINTERFACE_1597797400962_H
#define CREATIVE_KERNEL_SUPPORTINTERFACE_1597797400962_H
#include "basickernelexport.h"


namespace qtuser_3d
{
	class MachineTracer;
}

namespace creative_kernel
{
	BASIC_KERNEL_API void addMachineTracer(qtuser_3d::MachineTracer* tracer);
	BASIC_KERNEL_API void removeMachineTracer(qtuser_3d::MachineTracer* tracer);

	BASIC_KERNEL_API void machineChanged(QString oldmachine, QString newmachine);
	BASIC_KERNEL_API void machineTypeChanged(int oldmachinetype, int newmachinetype);

}

#endif // CREATIVE_KERNEL_SUPPORTINTERFACE_1597797400962_H
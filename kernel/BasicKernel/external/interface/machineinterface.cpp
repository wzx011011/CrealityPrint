#include "machineinterface.h"
#include "kernel/kernel.h"
#include "kernel/machineselector.h"
#include "qtuser3d/module/machinetracer.h"
using namespace qtuser_3d;
namespace creative_kernel
{

	void addMachineTracer(MachineTracer* tracer)
	{
		getKernel()->machineSelector()->addTracer(tracer);
	}

	void removeMachineTracer(MachineTracer* tracer)
	{
		getKernel()->machineSelector()->removeTracer(tracer);
	}

	void machineChanged(QString oldmachine, QString newmachine)
	{
		getKernel()->machineSelector()->onMachineChanged(oldmachine, newmachine);
	}

	void machineTypeChanged(int oldmachinetype, int newmachinetype)
	{
		getKernel()->machineSelector()->onMachineTypeChanged(oldmachinetype, newmachinetype);
	}

}
#include "machineselector.h"
#include "qtuser3d/module/machinetracer.h"

using namespace qtuser_3d;
namespace creative_kernel
{
	MachineSelector::MachineSelector(QObject* parent)
		:QObject(parent)
	{
	}
	
	MachineSelector::~MachineSelector()
	{
	}

	void MachineSelector::addTracer(qtuser_3d::MachineTracer* tracer)
	{
		if (tracer) m_selectorTracers.push_back(tracer);
	}

	void MachineSelector::removeTracer(qtuser_3d::MachineTracer* tracer)
	{
		if (tracer) m_selectorTracers.removeOne(tracer);
	}

	void MachineSelector::onMachineChanged(QString oldmachine, QString newmachine)
	{
		for (MachineTracer* tracer : m_selectorTracers)
		{
			tracer->onMachineChanged(oldmachine, newmachine);
		}
	}

	void MachineSelector::onMachineTypeChanged(int oldmachinetype, int newmachinetype)
	{
		for (MachineTracer* tracer : m_selectorTracers)
		{
			tracer->onMachineTypeChanged(oldmachinetype, newmachinetype);
		}
	}

}
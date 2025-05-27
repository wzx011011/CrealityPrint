#include "localnetworkinterface/devicephase.h"
#include "data/interface.h"
#include <interface/commandinterface.h>
#include <interface/uiinterface.h>
#include <qglobal.h>
namespace creative_kernel
{
	DevicePhase::DevicePhase(QObject* parent)
		:QObject(parent)
	{

	}

	DevicePhase::~DevicePhase()
	{

	}

	void DevicePhase::onStartPhase()
	{
		qWarning() << "DevicePhase::onStartPhase";
		QMetaObject::invokeMethod(getUIAppWindow(), "startPlayVideo");
	}

	void DevicePhase::onStopPhase()
	{
		qWarning() << "DevicePhase::onStopPhase";
		QMetaObject::invokeMethod(getUIAppWindow(), "stopPlayVideo");
	}
	void DevicePhase::onModelAdded(ModelN* model)
	{
		setKernelPhase(KernelPhaseType::kpt_prepare);
	}
}
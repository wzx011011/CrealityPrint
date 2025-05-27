#include "managersetting.h"
#include <QtCore/QFile>
namespace us
{
	managersetting::managersetting(QObject* parent)
		:QObject(parent)
	{
		m_MachineTypeSetting = new MachineTypeSetting();
		m_SliceGradeSetting = new SliceGradeSetting();
		m_MachineSetting = new MachineSetting();
	}
	managersetting::~managersetting()
	{
		if (m_MachineTypeSetting)
		{
			delete m_MachineTypeSetting;
			m_MachineTypeSetting = nullptr;
		}
		if (m_SliceGradeSetting)
		{
			delete m_SliceGradeSetting;
			m_SliceGradeSetting = nullptr;
		}
		if (m_MachineSetting)
		{
			delete m_MachineSetting;
			m_MachineSetting = nullptr;
		}

	}

	const QHash<QString, USetting*>& managersetting::initSliceGradeSetting(const QString& Grade)
	{
		m_SliceGradeSetting->getSliceGrade(Grade);
		return m_SliceGradeSetting->settings();
	}


	const QHash<QString, USetting*>& managersetting::initMachineSetting(const QString& machineType)
	{
		m_MachineSetting->getMachineSetting(machineType);
		return m_MachineSetting->settings();
	}

}

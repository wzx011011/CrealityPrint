#include "ansycworker.h"
#include "slicesource.h"

#include "sliceattain.h"
#include "dllansycslicer.h"

#include "core/sliceresult.h"
#include "utils/matrixutil.h"
#include "us/slicermanager.h"
#include "us/modelgroupinput.h"

#include <QtCore/QTimeLine>
#include <QtCore/QTime>
#include<QStandardPaths>

#include "interface/confinterface.h"
#include "interface/spaceinterface.h"

#ifdef _DEBUG
#include "../../buildinfo.h"
#endif 

using namespace creative_kernel;
AnsycWorker::AnsycWorker(QObject* parent)
	:Job(parent)
	, m_source(nullptr)
	, m_ansycSlicer(nullptr)
	, m_globalSettings(nullptr)
	, m_sliceAttain(nullptr)
{
	m_source = new SliceSource(this);
	m_ansycSlicer = new DLLAnsycSlicer(this);
}

AnsycWorker::~AnsycWorker()
{
}

void AnsycWorker::setGlobalSettings(us::GlobalSetting* globalSettings)
{
	m_globalSettings = globalSettings;
}

void AnsycWorker::setRemainAttain(SliceAttain* attain)
{
	m_remainAttain.reset(attain);
}

QString AnsycWorker::name()
{
	return "Slice";
}

QString AnsycWorker::description()
{
	return "";
}

void AnsycWorker::failed()
{
	qInfo() << "AnsycWorker failed";
	m_ansycSlicer->stop();
	emit sliceFailed();
}

void AnsycWorker::successed(qtuser_core::Progressor* progressor)
{
	qInfo() << "AnsycWorker success";
	if (!m_sliceAttain)
	{
		if (m_remainAttain.data())
		{
			emit sliceSuccess(m_remainAttain.take());
		}
		else
			emit sliceFailed();
	}
	else
	{
		emit sliceSuccess(m_sliceAttain);
		m_sliceAttain = nullptr;
	}

	clearModelSpaceDrity();
	SlicerManager::instance().setSliceParamDirty(false);
}

bool AnsycWorker::needReSlice()
{
	if (SlicerManager::instance().sliceParamDirty()
		|| modelSpaceDirty())
		return true;

	if (m_remainAttain.isNull())
		return true;

	return false;
}

void AnsycWorker::work(qtuser_core::Progressor* progressor)
{
	if (!needReSlice())
		return;

	qInfo() << "AnsycWorker work";
	if (!m_source || !m_source->needSlice() || !m_ansycSlicer)
	{
		progressor->failed("nothing to slice.");
		return;
	}
	
	//Merge QML modifications
	SlicerManager::instance().mergeSettingCache();
	qInfo() << "Merge QML modifications work";
    us::GlobalSetting* globalSettings = m_globalSettings;
    QList<const us::ExtruderSetting*> extruderSettings = SlicerManager::instance().extruderSettings();
	QList<const us::ModelGroupInput*> modelGroupInputs = m_source->createGroupInputs(globalSettings);
	
	
	if (modelGroupInputs.size() > 0)
	{
		qInfo() << "m_ansycSlicer->doSlice before work";
		QSharedPointer<cxsw::SliceResult> result(m_ansycSlicer->doSlice(modelGroupInputs, globalSettings, extruderSettings, progressor));
		if (result)
		{
			m_sliceAttain = new SliceAttain();
			result->load();
			std::string prefix = result->getGCodePrefix();
			std::string tailfix = result->getGCodeTailfix();
			m_sliceAttain->build(result->m_data_gcodelayer, prefix, tailfix, globalSettings);

		}
	}
		
	for (const us::ModelGroupInput* modelGroupInput : modelGroupInputs)
	{
		delete modelGroupInput;
	}
	modelGroupInputs.clear();
}


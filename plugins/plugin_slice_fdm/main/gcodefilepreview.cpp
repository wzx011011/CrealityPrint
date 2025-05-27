#include "gcodefilepreview.h"
#include "render/slicepreviewflow.h"
#include "main/sliceattain.h"
#include "core/sliceresult.h"
#include "us/slicermanager.h"

GcodeFilePreview::GcodeFilePreview(QObject* parent)
	:PreviewBase(parent)
{
}

GcodeFilePreview::~GcodeFilePreview()
{
}

void GcodeFilePreview::work(qtuser_core::Progressor* progressor)
{
	qInfo() << "work";

	progressor->progress(0.1f);
	us::GlobalSetting* globalSettings = SlicerManager::instance().globalsettings_modify();
	progressor->progress(0.2f);
	cxsw::SliceResult result;
	result.load(std::string(m_fileName.toLocal8Bit()));
	if (!result.m_data_gcodelayer.empty())
	{
		m_attain = new SliceAttain();
		std::string prefix = result.getGCodePrefix();
		//int last = result.m_data_gcodelayer.size()-1;
		//result.setGCodeTail(result.m_data_gcodelayer.at(last));
		std::string tailfix = result.getGCodeTailfix();
		m_attain->build(result.m_data_gcodelayer, prefix, tailfix,globalSettings);
	}
	progressor->progress(1.0);
}

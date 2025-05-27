#include "ansycslicer.h"
AnsycSlicer::AnsycSlicer(QObject* parent)
	:QObject(parent)
{
}

AnsycSlicer::~AnsycSlicer()
{
}

cxsw::SliceResult* AnsycSlicer::doSlice(const QList<const us::ModelGroupInput*>& modelGroups, const us::GlobalSetting* globalSetting, QList<const us::ExtruderSetting*> extruderSettings,
	qtuser_core::Progressor* progressor)
{
	return nullptr;
}

void AnsycSlicer::stop()
{

}

#ifndef _NULLSPACE_DLLANSYCSLICER_1591781523824_H
#define _NULLSPACE_DLLANSYCSLICER_1591781523824_H
#include "main/ansycslicer.h"

class DLLAnsycSlicer: public AnsycSlicer
{
public:
	DLLAnsycSlicer(QObject* parent = nullptr);
	virtual ~DLLAnsycSlicer();

    cxsw::SliceResult* doSlice(const QList<const us::ModelGroupInput*>& modelGroups, const us::GlobalSetting* globalSetting, QList<const us::ExtruderSetting*> extruderSettings,
		qtuser_core::Progressor* progressor) override;
};
#endif // _NULLSPACE_DLLANSYCSLICER_1591781523824_H

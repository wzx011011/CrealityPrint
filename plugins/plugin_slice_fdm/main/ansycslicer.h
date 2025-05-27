#ifndef _NULLSPACE_ANSYCSLICER_1591673316708_H
#define _NULLSPACE_ANSYCSLICER_1591673316708_H
#include <QtCore/QObject>
#include "qtusercore/module/progressor.h"

namespace cxsw
{
	class SliceResult;
}

namespace us
{
	class ModelGroupInput;
	class GlobalSetting;
	class ExtruderSetting;
}

class AnsycSlicer: public QObject
{
public:
	AnsycSlicer(QObject* parent = nullptr);
	virtual ~AnsycSlicer();

    virtual cxsw::SliceResult* doSlice(const QList<const us::ModelGroupInput*>& modelGroups, const us::GlobalSetting* globalSetting, QList<const us::ExtruderSetting*> extruderSettings,
		qtuser_core::Progressor* progressor);

	virtual void stop();
};
#endif // _NULLSPACE_ANSYCSLICER_1591673316708_H

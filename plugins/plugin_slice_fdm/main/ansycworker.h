#ifndef _NULLSPACE_ANSYCWORKER_1590248311419_H
#define _NULLSPACE_ANSYCWORKER_1590248311419_H
#include "qtusercore/module/job.h"
#include <QtCore/QThread>
#include "us/globalsetting.h"

class SliceSource;
class AnsycSlicer;
class SliceAttain;

namespace us
{
	class ModelGroupInput;
	class ExtruderSetting;
}

class AnsycWorker: public qtuser_core::Job
{
	Q_OBJECT
public:
	AnsycWorker(QObject* parent = nullptr);
	virtual ~AnsycWorker();

	void setGlobalSettings(us::GlobalSetting* globalSettings);
	void setRemainAttain(SliceAttain* attain);
protected:
	QString name() override;
	QString description() override;
	void failed() override;   
	void successed(qtuser_core::Progressor* progressor) override;
	void work(qtuser_core::Progressor* progressor) override;

	bool needReSlice();
signals:
	void sliceSuccess(SliceAttain* attain);
	void sliceFailed();

protected:
	AnsycSlicer* m_ansycSlicer;
	SliceSource* m_source;

	SliceAttain* m_sliceAttain;
	QScopedPointer<SliceAttain> m_remainAttain;

	us::GlobalSetting* m_globalSettings;
};
#endif // _NULLSPACE_ANSYCWORKER_1590248311419_H

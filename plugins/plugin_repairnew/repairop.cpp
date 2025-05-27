#include "repairop.h"

#include "qtuser3d/entity/modelnentity.h"

#include <QtCore/QDebug>  

#include "interface/visualsceneinterface.h"
#include "interface/eventinterface.h"
#include "interface/jobsinterface.h"
#include "interface/reuseableinterface.h"
#include "interface/selectorinterface.h"

#include "data/modeln.h"
#include "repairjob.h"
#include <QList>
#include "kernel/kernel.h"
using namespace creative_kernel;
using namespace qtuser_3d;

RepairOp::RepairOp(QObject* parent)
	:SceneOperateMode(parent)
{
}

RepairOp::~RepairOp()
{
}

void RepairOp::repairModel(QList<creative_kernel::ModelN*> selections)
{
	RepairJob* job = new RepairJob();
	for (size_t i = 0; i < selections.size(); i++)
	{
		ModelN* m = selections.at(i);
		job->setModel(m);
	}
	executeJob(JobPtr(job));
	disconnect(getKernel()->jobExecutor(), SIGNAL(jobsEnd()), this, SIGNAL(repairSuccess()));
	connect(getKernel()->jobExecutor(), SIGNAL(jobsEnd()), this, SIGNAL(repairSuccess()));
}
void RepairOp::onAttach()
{
	//addLeftMouseEventHandler(this);
	//addHoverEventHandler(this);
}

void RepairOp::onDettach()
{
	//removeLeftMouseEventHandler(this);
	//removeHoverEventHandler(this);

}

void RepairOp::onLeftMouseButtonClick(QMouseEvent* event)
{
	QVector3D position, normal;
	ModelN* model = checkPickModel(event->pos(), position, normal);
	if(model)
	{
		RepairJob* job = new RepairJob();
		job->setModel(model);
		executeJob(JobPtr(job));
	}
}

void RepairOp::onHoverMove(QHoverEvent* event)
{
	//
}

/*
void RepairOp::judgeModel(QList<creative_kernel::ModelN*> selections)
{
	JudgeModelJob* job = new JudgeModelJob();
	for (size_t i = 0; i < selections.size(); i++)
	{
		ModelN* m = selections.at(i);
		job->setModel(m);
	}
	executeJob(JobPtr(job));
	disconnect(getKernel()->jobExecutor(), SIGNAL(jobsEnd()), this, SIGNAL(judgeSuccess()));
	connect(getKernel()->jobExecutor(), SIGNAL(jobsEnd()), this, SIGNAL(judgeSuccess()));
}
*/
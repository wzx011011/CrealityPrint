#include "repairjob.h"

#include "data/modeln.h"
#include "data/modelgroup.h"
#include "interface/visualsceneinterface.h"
#include "interface/modelinterface.h"
#include "interface/cacheinterface.h"
#include "utils/matrixutil.h"
#include "utils/convexhullcalculator.h"
#include "mmesh/trimesh/trimeshutil.h"
#include "omp.h"  
#include <QTime>
#include"kernel/kernelui.h"
#include "ccglobal/tracer.h"

#include "cmesh/mesh/repair.h"
#include "cmesh/cgal/subrepairmenu.h"

using namespace creative_kernel;
using namespace trimesh;

class InnerTracer : public ccglobal::Tracer
{
public:
	InnerTracer(qtuser_core::Progressor* progressor, QObject* receiver)
		:m_progressor(progressor)
		, m_receiver(receiver)
	{

	}

	virtual ~InnerTracer()
	{

	}

	void progress(float r) override
	{
		if (m_progressor)
			return m_progressor->progress(r);
	}

	bool interrupt() override
	{
		if (m_progressor)
			return m_progressor->interrupt();
		return false;
	}

	void message(const char* msg) override
	{
		qDebug() << QString(msg);
	}

	void failed(const char* msg) override
	{
		qWarning() << QString(msg);
		if (m_progressor)
			return m_progressor->failed(msg);
	}

	void success() override
	{

	}

	qtuser_core::Progressor* m_progressor;
	QObject* m_receiver;
};

//using namespace trimesh;
RepairJob::RepairJob(QObject* parent)
	:qtuser_core::Job(parent)
	, m_model(nullptr)
	,progressor(nullptr)
	,m_receiver(nullptr)
{
	m_mesh = NULL;
}

RepairJob::~RepairJob()
{
}

void RepairJob::setModel(creative_kernel::ModelN* model)
{
	m_model = model;
}

QString RepairJob::name()
{
	return "";
}

QString RepairJob::description()
{
	return "";
}

void RepairJob::setObject(QObject* receiver)
{
	m_receiver = receiver;
}

void RepairJob::work(qtuser_core::Progressor* progressor)
{

	if (!m_model)
	{
		qDebug() << "Please choose one model";
		return;
	}
	qDebug() << "reapir work";
	TriMesh* choosedMesh = m_model->mesh();
	repairMeshObj(choosedMesh,progressor);
	ConvexHullCalculator::calculate(m_mesh, nullptr);
	progressor->progress(1.0f);
}

void RepairJob::failed()
{
	qDebug() << "repair failed";
	m_model = nullptr;

	if (m_mesh)
	{
		delete m_mesh;
		m_mesh = NULL;
	}
	setVisOperationMode(nullptr);
}

void RepairJob::successed(qtuser_core::Progressor* progressor)
{
#if 0

	std::swap(m_meshLoadInfo->m_mesh, m_meshLoadInfo->m_optimizedMesh);
#else

	trimesh::TriMesh* restoreMesh = m_mesh;
	if(restoreMesh !=NULL)
	{
		creative_kernel::ModelGroup* group = qobject_cast<creative_kernel::ModelGroup*>(m_model->parent());
		if (restoreMesh->faces.size() > 0 && restoreMesh->vertices.size() > 0 && group)
		{
			/*QList<ModelN*> newModels;
			QString name = m_model->objectName().left(m_model->objectName().lastIndexOf("."));
			QString suffix = m_model->objectName().right(m_model->objectName().length() - m_model->objectName().lastIndexOf("."));
			{

				creative_kernel::ModelN* m = new creative_kernel::ModelN();

				if (!name.endsWith("-repair"))
				{
					name += "-repair";
				}
				name += suffix;
				m->setObjectName(name);

				trimesh::fxform xf = qMatrix2Xform(m_model->globalMatrix());
				size_t vertexCount = restoreMesh->vertices.size();
				for (size_t i = 0; i < vertexCount; ++i)
				{
					trimesh::vec3 v = restoreMesh->vertices.at(i);
					restoreMesh->vertices.at(i) = xf * v;
				}

				restoreMesh->clear_bbox();
				restoreMesh->need_bbox();
				trimesh::vec3 offset = mmesh::moveTrimesh2Center(restoreMesh);

				TriMeshPtr ptr(restoreMesh);
				creative_kernel::cache(ptr);
				m->setTrimesh(ptr);

				m->SetInitPosition(m_model->GetInitPosition());
				m->setLocalPosition(-QVector3D(offset.x, offset.y, offset.z));
				newModels.push_back(m);
			}


			if (newModels.size() > 0)
			{
				QList<ModelN*> removes;
				removes << m_model;

				modifyGroup(removes, newModels, group, true);
			}*/

			//delete restoreMesh;

//______________________________________________________________________

			QList<ModelN*> models;
			QList<TriMeshPtr> meshes;
			QList<TriMeshPtr> meshes_aux;
			QList<int> aux_pos;
			QList<QString> names;

			QString name = m_model->objectName().left(m_model->objectName().lastIndexOf("."));
			QString suffix = m_model->objectName().right(m_model->objectName().length() - m_model->objectName().lastIndexOf("."));
			if (!name.endsWith("-repair"))
			{
				name += "-repair";
			}
			name += suffix;
			m_repaireddMesh.reset(m_mesh);
				
			models.push_back(m_model);
			names.push_back(name);
			meshes.push_back(m_repaireddMesh);

			replaceModelsMesh(models, meshes, names, true);
			m_model->needDetectError();
			m_repaireddMesh->need_bbox();
			requestVisUpdate(true);
		}
	}

	//if (m_mesh)
	//{
	//	delete m_mesh;
	//	m_mesh = NULL;
	//}
	//TMesh::info("Saving output mesh ...\n");
	//m_repairMesh->save("HYT_TEST.stl");

	//m_model=nullptr;
#endif
	qDebug() << "repair success";

	//setVisOperationMode(nullptr);

}
trimesh::TriMesh* RepairJob::repairMeshObj(trimesh::TriMesh* choosedMesh, qtuser_core::Progressor* progressor)
{
#ifdef USE_CGAL_REPAIR_MESH
	m_mesh = cgalRepairMeshObj(choosedMesh);
#else
	
	//m_mesh = tmeshRepairMeshObj(choosedMesh, progressor);

	try {
		InnerTracer tracer(progressor, m_receiver);
		//repair::TriMeshRepair repair;
		//repair.initMesh(choosedMesh);
		//m_mesh = repair.repair(&tracer);

		//m_mesh = cmesh::repairMenu(choosedMesh, true, &tracer);

        m_mesh = cmesh::subRepairMenu(choosedMesh, &tracer);
	}
	catch (...)
	{
		qDebug() << "Repair::work, mmesh::repairMeshObj  catch(...)";
		progressor->failed("repair failed!");
		return nullptr;
	}


#endif
	return m_mesh;
}

#if 0
JudgeModelJob::JudgeModelJob(QObject* parent)
	:qtuser_core::Job(parent)
	, m_model(nullptr), m_hasBoundaries(false)
	, progressor(nullptr)
{

}
JudgeModelJob::~JudgeModelJob()
{

}
void JudgeModelJob::setModel(creative_kernel::ModelN* model)
{
	m_model = model;
}
bool JudgeModelJob::judgeModelBoundaries(trimesh::TriMesh* choosedMesh, qtuser_core::Progressor* progressor)
{
	m_hasBoundaries = judgeModelHasBoundaries(choosedMesh, progressor);
	return m_hasBoundaries;
}
QString JudgeModelJob::name()
{
	return "";
}
QString JudgeModelJob::description()
{
	return "";
}
void JudgeModelJob::work(qtuser_core::Progressor* progressor)
{
	if (!m_model)
	{
		qDebug() << "Please choose one model";
		return;
	}
	qDebug() << "judge work";
	TriMesh* choosedMesh = m_model->mesh();
	judgeModelBoundaries(choosedMesh, progressor);
	progressor->progress(1.0f);
}
void JudgeModelJob::failed()
{
	qDebug() << "judge failed";
	m_model = nullptr;
	m_hasBoundaries = false;
	setVisOperationMode(nullptr);
}
void JudgeModelJob::successed(qtuser_core::Progressor* progressor)
{
	qDebug() << "judge success";
	m_model = nullptr;
	if (m_hasBoundaries)
	{
		qDebug() << "popup tip dialog.";
		AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "modelRepairMessageDlg");
	}
	m_hasBoundaries = false;
	
	setVisOperationMode(nullptr);
}
#endif
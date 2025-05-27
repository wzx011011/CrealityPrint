#include "calculatenormaljob.h"
#include "external/utils/modelpositioninitializer.h"
#include "external/job/meshloadinfo.h"
#include "utils/normalcalculator.h"
#include "external/utils/convexhullcalculator.h"

#include "interface/modelinterface.h"
#include "interface/cacheinterface.h"

#include "mmesh/trimesh/trimeshutil.h"
namespace creative_kernel
{
	CalculateNormalJob::CalculateNormalJob(QObject* parent)
		:Job(parent)
		, m_meshLoadInfo(nullptr)
	{
	}

	CalculateNormalJob::~CalculateNormalJob()
	{
	}

	QString CalculateNormalJob::name()
	{
		return "Normal Calculate";
	}

	QString CalculateNormalJob::description()
	{
		return "";
	}

	void CalculateNormalJob::failed()                        // invoke from main thread
	{
		qDebug() << "fail";
	}

	void CalculateNormalJob::successed(qtuser_core::Progressor* progressor)                    // invoke from main thread
	{
		qDebug() << "success";

	}

	void CalculateNormalJob::work(qtuser_core::Progressor* progressor)   // invoke from worker thread
	{
		if (!m_meshLoadInfo)
			return;
		if (!m_meshLoadInfo->m_optimizedMesh)
		{
			std::swap(m_meshLoadInfo->m_mesh, m_meshLoadInfo->m_optimizedMesh);
		}

		if (!m_meshLoadInfo->m_optimizedMesh) return;

		qDebug() << "work";

		trimesh::TriMesh* mesh = m_meshLoadInfo->m_optimizedMesh;
		mesh->need_bbox();

		//move mesh to positive
		mmesh::moveTrimesh2Center(mesh);

		progressor->progress(0.1f);
		//if (mesh->normals.size() != mesh->vertices.size())
		//	NormalCalculator::calculate(mesh, progressor);

		progressor->progress(0.4f);
		ConvexHullCalculator::calculate(mesh, progressor);
		progressor->progress(1.0f);
	}
}

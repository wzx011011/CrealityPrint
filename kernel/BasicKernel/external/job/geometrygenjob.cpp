#include "geometrygenjob.h"

#include "external/utils/modelpositioninitializer.h"
#include "interface/modelinterface.h"
#include "external/job/meshloadinfo.h"
#include "interface/cacheinterface.h"
#include "utils/trimesh2qgeometryrenderer.h"
#include "us/slicermanager.h"
namespace creative_kernel
{
	GeometryGenJob::GeometryGenJob(QObject* parent)
		:Job(parent)
		, m_meshLoadInfo(nullptr)
	{
		//machine_is_belt
		us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();
		m_needLayout = !(globalsetting->settings().value("machine_is_belt")->value().toBool());
	}

	GeometryGenJob::~GeometryGenJob()
	{
	}

	QString GeometryGenJob::name()
	{
		return "Generate Geometry";
	}

	QString GeometryGenJob::description()
	{
		return "Generating Geometry.";
	}

	void GeometryGenJob::failed()
	{
		qDebug() << "fail";
	}

	void GeometryGenJob::successed(qtuser_core::Progressor* progressor)
	{
		trimesh::TriMesh* mesh = m_meshLoadInfo ? m_meshLoadInfo->takeOptimizedMesh() : nullptr;
		if (mesh)
		{
			qDebug() << "success";

			TriMeshPtr ptr(mesh);
			creative_kernel::cache(ptr);

			QString shortName = m_meshLoadInfo->m_name;
			QStringList stringList = shortName.split("/");
			if (stringList.size() > 0) shortName = stringList.back();

			creative_kernel::ModelN* model = new creative_kernel::ModelN();

			model->setTrimeshFromGeometryData(ptr, m_meshLoadInfo->m_faceGeometryData, m_meshLoadInfo->m_lineGeometryData, m_meshLoadInfo->m_edgeGeometryData);
			model->setObjectName(shortName);
			//model->needDetectError();

			if (m_needLayout)
			{
				if (!ModelPositionInitializer::nestLayout(model, true))
				{
					ModelPositionInitializer::layout(model, progressor, false);
				}
			}
			else
			{
				//machine_is_belt
				ModelPositionInitializer::layoutBelt(model, progressor,false);
			}
			
			addModel(model, true, progressor);
			creative_kernel::clearCache();


		}
	}

	void GeometryGenJob::work(qtuser_core::Progressor* progressor)
	{
		qDebug() << "work";
		trimesh::TriMesh* mesh = m_meshLoadInfo ? m_meshLoadInfo->m_optimizedMesh : nullptr;
		if (mesh)
		{
			m_meshLoadInfo->m_faceGeometryData = genGeometryFaceData(mesh, progressor);
			//m_meshLoadInfo->m_lineGeometryData = genGeometryLineData(mesh, progressor);
			//m_meshLoadInfo->m_edgeGeometryData = genGeometryEdgeData(mesh, progressor);
		}
	}
}

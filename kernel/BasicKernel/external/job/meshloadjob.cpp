#include "meshloadjob.h"
#include <QDebug>

#include "external/data/meshiocenter.h"
#include "external/job/meshloadinfo.h"

namespace creative_kernel
{
	MeshLoadJob::MeshLoadJob(QObject* parent)
		:Job(parent)
		, m_meshLoadInfo(nullptr)
	{
	}

	MeshLoadJob::~MeshLoadJob()
	{
	}

	QString MeshLoadJob::name()
	{
		return "Load Mesh";
	}

	QString MeshLoadJob::description()
	{
		return "Loading Mesh.";
	}

	void MeshLoadJob::failed()
	{
		qDebug() << "fail";
	}

	void MeshLoadJob::successed(qtuser_core::Progressor* progressor)
	{
		qDebug() << "success";
	}

	void MeshLoadJob::work(qtuser_core::Progressor* progressor)
	{
		if (!m_meshLoadInfo)
			return;

		qDebug() << "work";
		trimesh::TriMesh* mesh = MESHIO.asyncLoad(m_meshLoadInfo->m_name, progressor);
		if(mesh)
			mesh->normals.clear();
		m_meshLoadInfo->m_mesh = mesh;
	}
}

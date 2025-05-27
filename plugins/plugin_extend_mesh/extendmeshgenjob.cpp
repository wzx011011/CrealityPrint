#include "extendmeshgenjob.h"

#include "data/modeln.h"
#include "data/modelgroup.h"
#include "interface/visualsceneinterface.h"
#include "interface/modelinterface.h"
#include "interface/cacheinterface.h"
#include "utils/matrixutil.h"
#include "utils/convexhullcalculator.h"
#include "photo2mesh.h"

#include <QFileInfo>
#include <QImage>

#include "controller.h"
#include <QCoreApplication>
#include <QStandardPaths>


using namespace creative_kernel;

ExtendMeshGenJob::ExtendMeshGenJob(QObject* parent)
	:qtuser_core::Job(parent)
	, m_model(nullptr)
{

}

ExtendMeshGenJob::~ExtendMeshGenJob()
{
}

void ExtendMeshGenJob::setModel(creative_kernel::ModelN* model)
{
	m_model = model;
}

void ExtendMeshGenJob::setName(QString filename)
{
	m_fileName = filename;
}

QString ExtendMeshGenJob::name()
{
	return "";
}

QString ExtendMeshGenJob::description()
{
	return "";
}

void moveMeshes2Origin(trimesh::TriMesh* mesh, bool zZero)
{
	trimesh::box3 b;
	mesh->need_bbox();
	b = mesh->bbox;

	trimesh::vec3 offset = trimesh::vec3() - b.center();
	if (zZero)
		offset.z = 0.0f - b.min.z;
	else
		offset.z = 0.0f;

	trimesh::trans(mesh, offset);
}

void ExtendMeshGenJob::work(qtuser_core::Progressor* progressor)
{
	qDebug() << "extendmesh load work";
	
	QFileInfo fileinfo(m_fileName);
	QString suffix = fileinfo.suffix();
	suffix = suffix.toLower();

	if (suffix == "bmp" || suffix == "jpg" || suffix == "jpeg" || suffix == "png")
	{
		//Photo2Mesh p2m;
		trimesh::TriMesh* mesh = m_p2m.generate(m_fileName, progressor);
		if (mesh)
		{
			m_meshLoadInfo->m_mesh = mesh;
		}
		else
		{
			progressor->failed("image load failed");
			return;
		}
	}
	else if (suffix == "gtl" || suffix == "gbl" || suffix == "gbo" || suffix == "gbp"
		|| suffix == "gbs" || suffix == "gko" || suffix == "gml" || suffix == "gpb"
		|| suffix == "gto" || suffix == "gpt" || suffix == "gtp" || suffix == "gts"
		)//by TCJ
	{
		//generate image(png or svg)			   
		controller c(nullptr, QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)/*QCoreApplication::applicationDirPath()*/);
		c.list_of_gerbers << m_fileName;
		c.run_all();
			
		//posted								   
		for (QString filename : c.list_of_svgfiles)
		{
			trimesh::TriMesh* mesh = m_p2m.generate(filename, progressor);
			m_meshLoadInfo->m_mesh = mesh;
		}
	}
	moveMeshes2Origin(m_meshLoadInfo->m_mesh, false);
	ConvexHullCalculator::calculate(m_meshLoadInfo->m_mesh, nullptr);
}

void ExtendMeshGenJob::failed()
{
	qDebug() << "extendmesh load failed";
}

void ExtendMeshGenJob::successed(qtuser_core::Progressor* progressor)
{
	qDebug() << "extendmesh load success";
	std::swap(m_meshLoadInfo->m_mesh, m_meshLoadInfo->m_optimizedMesh);
}
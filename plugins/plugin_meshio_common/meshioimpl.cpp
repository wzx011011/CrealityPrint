#include "meshioimpl.h"
#include <QFileInfo>
#include "cxbin/load.h"
#include "qtusercore/module/progressortracer.h"
#include "stringutil/util.h"

MeshIOImpl::MeshIOImpl(QObject* parent)
	:QObject(parent)
{
	m_loadSupportExtensions << "stl" << "obj" << "dae" << "3mf" << "3ds" << "wrl" << "cxbin" << "off" << "ply" << "off";
}

MeshIOImpl::~MeshIOImpl()
{
}

QString MeshIOImpl::description()
{
	return "";
}

QStringList MeshIOImpl::saveSupportExtension()
{
	return m_saveSupportExtensions;
}

QStringList MeshIOImpl::loadSupportExtension()
{
	return m_loadSupportExtensions;
}

trimesh::TriMesh* MeshIOImpl::load(const QString& name, qtuser_core::Progressor* progressor)
{
	qtuser_core::ProgressorTracer tracer(progressor);
	//std::string fileName = name.toLocal8Bit().constData();

	std::wstring strWname = name.toStdWString();
	std::string strname = stringutil::wstring2string(strWname);
	return cxbin::loadAll(strname, &tracer);
}

void MeshIOImpl::save(trimesh::TriMesh* mesh, const QString& name, qtuser_core::Progressor* progressor)
{

}

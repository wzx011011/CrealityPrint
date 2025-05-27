#include "meshloadinfo.h"

MeshLoadInfo::MeshLoadInfo(QObject* parent)
	:QObject(parent)
	, m_mesh(nullptr)
	, m_optimizedMesh(nullptr)
	, m_faceGeometryData(nullptr)
	, m_lineGeometryData(nullptr)
	, m_edgeGeometryData(nullptr)
	//, m_faceGeometry(nullptr)
	//, m_lineGeometry(nullptr)
	//, m_edgeGeometry(nullptr)
{
}

MeshLoadInfo::~MeshLoadInfo()
{
	if (m_mesh)
	{
		delete m_mesh;
		m_mesh = nullptr;
	}

	if (m_optimizedMesh)
	{
		delete m_optimizedMesh;
		m_optimizedMesh = nullptr;
	}
	if (m_faceGeometryData)
	{
		delete m_faceGeometryData;
		m_faceGeometryData = nullptr;
	}
	if (m_lineGeometryData)
	{
		delete m_lineGeometryData;
		m_lineGeometryData = nullptr;
	}
	if (m_edgeGeometryData)
	{
		delete m_edgeGeometryData;
		m_edgeGeometryData = nullptr;
	}
	//if (m_faceGeometry)
	//{
	//	delete m_faceGeometry;
	//	m_faceGeometry = nullptr;
	//}
	//if (m_lineGeometry)
	//{
	//	delete m_lineGeometry;
	//	m_lineGeometry = nullptr;
	//}
	//if (m_edgeGeometry)
	//{
	//	delete m_edgeGeometry;
	//	m_edgeGeometry = nullptr;
	//}
}

trimesh::TriMesh* MeshLoadInfo::takeOptimizedMesh()
{
	trimesh::TriMesh* tmp = m_optimizedMesh;
	m_optimizedMesh = nullptr;
	return  tmp;
}

creative_kernel::GeometryData* MeshLoadInfo::takeFaceGeometryData()
{
	creative_kernel::GeometryData* tmp = m_faceGeometryData;
	m_faceGeometryData = nullptr;
	return  tmp;
}

creative_kernel::GeometryData* MeshLoadInfo::takeLineGeometryData()
{
	creative_kernel::GeometryData* tmp = m_lineGeometryData;
	m_lineGeometryData = nullptr;
	return  tmp;
}

creative_kernel::GeometryData* MeshLoadInfo::takeEdgeGeometryData()
{
	creative_kernel::GeometryData* tmp = m_edgeGeometryData;
	m_edgeGeometryData = nullptr;
	return  tmp;
}

//Qt3DRender::QGeometry* MeshLoadInfo::takeFaceGeometry()
//{
//	Qt3DRender::QGeometry* tmp = m_faceGeometry;
//	m_faceGeometry = nullptr;
//	return  tmp;
//}
//
//Qt3DRender::QGeometry* MeshLoadInfo::takeLineGeometry()
//{
//	Qt3DRender::QGeometry* tmp = m_lineGeometry;
//	m_lineGeometry = nullptr;
//	return  tmp;
//}
//
//Qt3DRender::QGeometry* MeshLoadInfo::takeEdgeGeometry()
//{
//	Qt3DRender::QGeometry* tmp = m_edgeGeometry;
//	m_edgeGeometry = nullptr;
//	return  tmp;
//}

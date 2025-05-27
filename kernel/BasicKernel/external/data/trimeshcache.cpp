#include "trimeshcache.h"
#include "external/utils/trimesh2qgeometryrenderer.h"

#include "interface/reuseableinterface.h"

namespace creative_kernel
{
	TrimeshCacheField::TrimeshCacheField()
		:m_faceGeometry(nullptr), m_lineGeometry(nullptr), m_edgeGeometry(nullptr)
	{

	}

	TrimeshCacheField::~TrimeshCacheField()
	{
		unCacheNode(m_faceGeometry);
		unCacheNode(m_lineGeometry);
		unCacheNode(m_edgeGeometry);
	}

	Qt3DRender::QGeometry* TrimeshCacheField::getGeometry(int optype, qtuser_core::Progressor* progressor)
	{
		switch (optype)
		{
		case 0:
			if (!m_faceGeometry)
			{
				m_faceGeometry = mesh ? trimesh2GeometryFace(&*mesh, nullptr, progressor) : nullptr;
				cacheNode(m_faceGeometry);
			}
			return m_faceGeometry;
		case 1:
			if (!m_lineGeometry)
			{
				m_lineGeometry = mesh ? trimesh2GeometryLine(&*mesh, nullptr, progressor) : nullptr;
				cacheNode(m_lineGeometry);
			}
			return m_lineGeometry;
		case 2:
			if (!m_edgeGeometry)
			{
				m_edgeGeometry = mesh ? trimesh2GeometryEdge(&*mesh, nullptr, progressor) : nullptr;
				cacheNode(m_edgeGeometry);
			}
			return m_edgeGeometry;
		default:
			return nullptr;
		}
	}

	Qt3DRender::QGeometry* TrimeshCacheField::getGeometryByData(int optype, GeometryData* geometryData)
	{
		switch (optype)
		{
		case 0:
			if (!m_faceGeometry)
			{
				m_faceGeometry = mesh ? genGeometryFaceData(geometryData, nullptr) : nullptr;
				cacheNode(m_faceGeometry);
			}
			return m_faceGeometry;
		case 1:
			if (!m_lineGeometry)
			{
				m_lineGeometry = mesh ? genGeometryLineData(geometryData, nullptr) : nullptr;
				cacheNode(m_lineGeometry);
			}
			return m_lineGeometry;
		case 2:
			if (!m_edgeGeometry)
			{
				m_edgeGeometry = mesh ? genGeometryEdgeData(geometryData, nullptr) : nullptr;
				cacheNode(m_edgeGeometry);
			}
			return m_edgeGeometry;
		default:
			return nullptr;
		}
	}

	SINGLETON_IMPL(TrimeshCache)
	TrimeshCache::TrimeshCache()
	{
	}
	
	TrimeshCache::~TrimeshCache()
	{
	}

	TrimeshCacheField* TrimeshCache::get(TriMeshPtr mesh)
	{
		if (!mesh) return nullptr;

		TrimeshCacheField* field = nullptr;
		for (TrimeshCacheField* f : m_fileds)
		{
			if (f->mesh == mesh)
			{
				field = f;
				break;
			}
		}
		if (field)
			return field;

		field = cache(mesh);
		return field;
	}

	TrimeshCacheField* TrimeshCache::cache(TriMeshPtr mesh)
	{
		TrimeshCacheField* field = new TrimeshCacheField();
		field->mesh = mesh;

		m_fileds.push_back(field); 

		return field;
	}

	void TrimeshCache::clear(bool noReferrence)
	{
		QList< TrimeshCacheField*> removes;
		for (TrimeshCacheField* f : m_fileds)
		{
			if (!noReferrence || (noReferrence && f->mesh.use_count() == 1))
			{
				delete f;
				removes.push_back(f);
			}
		}

		for (TrimeshCacheField* f : removes)
		{
			m_fileds.removeOne(f);
		}
	}
}
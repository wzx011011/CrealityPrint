#include "trimeshentity.h"
#include "external/utils/matrixutil.h"
#include "external/utils/convexhullcalculator.h"
#include "qtuser3d/math/space3d.h"

#include "interface/cacheinterface.h"
#include "utils/trimesh2qgeometryrenderer.h"
namespace creative_kernel
{
	TrimeshEntity::TrimeshEntity(QObject* parent)
		:Node3D(parent)
	{
	}
	
	TrimeshEntity::~TrimeshEntity()
	{
	}

	void TrimeshEntity::replaceTrimesh(TriMeshPtr mesh)
	{
		if (m_mesh == mesh)
			return;

		m_mesh = mesh;
		meshChanged();
	}

	void TrimeshEntity::setTrimesh(TriMeshPtr mesh, qtuser_core::Progressor *progressor)
	{
		if (m_mesh == mesh)
			return;

		m_mesh = mesh;
		meshChanged(progressor);

		m_localBox = qtuser_3d::Box3D(QVector3D());
		if (m_mesh)
		{
			m_localBox = triBox2Box3D(m_mesh->bbox);
		}

		setCenter(m_localBox.center());
	}

	void TrimeshEntity::setTrimeshFromGeometryData(TriMeshPtr mesh, GeometryData* faceGeoData, GeometryData* lineGeoData, GeometryData* edgeGeoData)
	{
		if(m_mesh == mesh)
			return;

		m_mesh = mesh;
		meshFromGeometryData(faceGeoData, lineGeoData, edgeGeoData);

		m_localBox = qtuser_3d::Box3D(QVector3D());
		if (m_mesh)
		{
			m_localBox = triBox2Box3D(m_mesh->bbox);
		}

		setCenter(m_localBox.center());
	}


	void TrimeshEntity::meshFromGeometryData(GeometryData* faceGeoData, GeometryData* lineGeoData, GeometryData* edgeGeoData)
	{
		//
	}

	trimesh::TriMesh* TrimeshEntity::mesh()
	{
		return m_mesh ? &*m_mesh : nullptr;
	}

	TriMeshPtr TrimeshEntity::meshptr()
	{
		return m_mesh;
	}

	void TrimeshEntity::meshChanged(qtuser_core::Progressor *progressor)
	{

	}

	int TrimeshEntity::primitiveNum()
	{
		return m_mesh ? (int)m_mesh->faces.size() : 0;
	}

	qtuser_3d::Box3D TrimeshEntity::calculateGlobalSpaceBox()
	{
		if (m_mesh)
		{
			QMatrix4x4 matrix = globalMatrix();
			qtuser_3d::Box3D box;
			if (m_mesh->flags.size() > 0)
			{
				//for bug ID1012787
				for (int i = 0; i < m_mesh->flags.size(); i++)
				{
					if (m_mesh->flags[i] >= m_mesh->vertices.size())
					{
						ConvexHullCalculator::calculate(m_mesh.get(), nullptr);
						break;
					}
				}
				
				int size = (int)m_mesh->flags.size();

				QVector<QVector2D> hullPoints2D;
				hullPoints2D.resize(size); 
				for (int i = 0; i < size; ++i)
				{
					trimesh::vec3 v = m_mesh->vertices.at(m_mesh->flags.at(i));
					QVector3D p(v.x, v.y, v.z);
					p = matrix * p;
					box += p;

					hullPoints2D[i] = p.toVector2D();
				}

				ConvexHullCalculator::calculate(hullPoints2D, m_convexPolygon.m_polygons, 0.0f);
			}
			else
			{
				int size = (int)m_mesh->vertices.size();
				for (int i = 0; i < size; ++i)
				{
					trimesh::vec3 v = m_mesh->vertices.at(i);
					QVector3D p(v.x, v.y, v.z);
					box += matrix * p;
				}
			}
			return box;
		}
		return m_localBox;
	}

	bool TrimeshEntity::rayCheck(int primitiveID, const qtuser_3d::Ray& ray, QVector3D& collide, QVector3D* normal)
	{
		if (primitiveID < 0 || primitiveID >= m_mesh->faces.size())
			return false;

		QMatrix4x4 m = globalMatrix();

		trimesh::TriMesh::Face f = m_mesh->faces.at(primitiveID);
		trimesh::vec3 v1 = m_mesh->vertices.at(f[0]);
		trimesh::vec3 v2 = m_mesh->vertices.at(f[1]);
		trimesh::vec3 v3 = m_mesh->vertices.at(f[2]);

		QVector3D qv1 = QVector3D(v1.x, v1.y, v1.z);
		QVector3D qv2 = QVector3D(v2.x, v2.y, v2.z);
		QVector3D qv3 = QVector3D(v3.x, v3.y, v3.z);
		qv1 = m * qv1;
		qv2 = m * qv2;
		qv3 = m * qv3;


		QVector3D v12 = qv2 - qv1;
		QVector3D v13 = qv3 - qv1;

		float len = v12.length();
		len += v13.length();

		v12.normalize();
		v13.normalize();
		QVector3D d = QVector3D::crossProduct(v12, v13);
		if (normal)
		{

			*normal = d;
			normal->normalize();
			//local normal	

			if (len == 0.0f) len = 1.0f;
			*normal = len * *normal;
		}

		return qtuser_3d::lineCollidePlane(qv1, d, ray, collide);
	}
	//global normal ,normal is not normalized , represent the average lenght of edges

	qtuser_3d::ConvexPolygon& TrimeshEntity::convexPolygon()
	{
		return m_convexPolygon;
	}

	trimesh::point TrimeshEntity::getMinYZ()
	{
		trimesh::point pointMinZ(0,0,99999);
		trimesh::fxform xf = qMatrix2Xform(globalMatrix());
		for (trimesh::point apoint:m_mesh->vertices)
		{
			apoint = xf * apoint;
			if (pointMinZ.z >apoint.z)
			{
				pointMinZ = apoint;
			} 
			else if (pointMinZ.z == apoint.z && pointMinZ.y > apoint.y)
			{
				pointMinZ = apoint;
			}
		}
		return pointMinZ;
	}

}
#ifndef CREATIVE_KERNEL_TRIMESHENTITY_1592195157165_H
#define CREATIVE_KERNEL_TRIMESHENTITY_1592195157165_H
#include "basickernelexport.h"
#include "qtuser3d/module/node3d.h"

#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QGeometryRenderer>

#include "qtuser3d/math/ray.h"
#include "qtuser3d/module/convexpolygon.h"
#include "trimeshheader.h"
#include "qtusercore/module/progressor.h"
#include "data/geometrydata.h"

using namespace qtuser_3d;
namespace creative_kernel
{
	class BASIC_KERNEL_API TrimeshEntity : public Node3D
	{
		Q_OBJECT
	public:
		TrimeshEntity(QObject* parent = nullptr);
		virtual ~TrimeshEntity();

		void setTrimesh(TriMeshPtr mesh, qtuser_core::Progressor *progressor=nullptr);
		void setTrimeshFromGeometryData(TriMeshPtr mesh, GeometryData* faceGeoData = nullptr, GeometryData* lineGeoData = nullptr, GeometryData* edgeGeoData = nullptr);
		void replaceTrimesh(TriMeshPtr mesh);

		virtual void meshChanged(qtuser_core::Progressor *progressor=nullptr);
		virtual void meshFromGeometryData(GeometryData *faceGeoData=nullptr, GeometryData *lineGeoData = nullptr, GeometryData *edgeGeoData = nullptr);

		trimesh::TriMesh* mesh();  // don't store this raw pointer as shared pointer
		TriMeshPtr meshptr();

		qtuser_3d::Box3D calculateGlobalSpaceBox() override;

		bool rayCheck(int primitiveID, const qtuser_3d::Ray& ray, QVector3D& collide, QVector3D* normal);
		//global normal ,normal is not normalized , represent the average lenght of edges

		qtuser_3d::ConvexPolygon& convexPolygon();

		trimesh::point getMinYZ();

		int primitiveNum() override;
	protected:
		TriMeshPtr m_mesh;
		qtuser_3d::ConvexPolygon m_convexPolygon;
	};
}
#endif // CREATIVE_KERNEL_TRIMESHENTITY_1592195157165_H
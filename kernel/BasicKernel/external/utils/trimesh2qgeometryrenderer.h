#ifndef TRIMESH_2_GEOMETRY_RENDERER_H
#define TRIMESH_2_GEOMETRY_RENDERER_H
#include "basickernelexport.h"
#include "qtusercore/module/progressor.h"
#include <QtCore/qobject.h>
#include "trimesh2/TriMesh.h"
#include "data/geometrydata.h"

namespace Qt3DRender
{
	class QGeometry;
}

namespace Qt3DCore
{
	class QNode;
}

namespace trimesh
{
	class TriMesh;
}

//BASIC_KERNEL_API class Trimesh2Geometry
//{
//public:
	//Trimesh2Geometry(QObject *parent=nullptr);
	//~Trimesh2Geometry();

BASIC_KERNEL_API creative_kernel::GeometryData* genGeometryFaceData(trimesh::TriMesh* mesh, qtuser_core::Progressor* progressor = nullptr);

BASIC_KERNEL_API creative_kernel::GeometryData* genGeometryLineData(trimesh::TriMesh* mesh, qtuser_core::Progressor* progressor = nullptr);

BASIC_KERNEL_API creative_kernel::GeometryData* genGeometryEdgeData(trimesh::TriMesh* mesh, qtuser_core::Progressor* progressor = nullptr, float angleThreshold = 30.0f);

// mesh only have point or line. no face
BASIC_KERNEL_API creative_kernel::GeometryData* genPLGeometryData(trimesh::TriMesh* mesh, qtuser_core::Progressor* progressor = nullptr);



BASIC_KERNEL_API Qt3DRender::QGeometry* genGeometryFaceData(creative_kernel::GeometryData* geometryData, Qt3DCore::QNode* parent = nullptr);

BASIC_KERNEL_API Qt3DRender::QGeometry* genGeometryLineData(creative_kernel::GeometryData* geometryData, Qt3DCore::QNode* parent = nullptr);

BASIC_KERNEL_API Qt3DRender::QGeometry* genGeometryEdgeData(creative_kernel::GeometryData* geometryData, Qt3DCore::QNode* parent = nullptr);

// mesh only have point or line. no face
BASIC_KERNEL_API Qt3DRender::QGeometry* genPLGeometryData(creative_kernel::GeometryData* geometryData, Qt3DCore::QNode* parent = nullptr);



BASIC_KERNEL_API Qt3DRender::QGeometry* trimesh2GeometryFace(trimesh::TriMesh* mesh, Qt3DCore::QNode* parent = nullptr, qtuser_core::Progressor* progressor = nullptr);

BASIC_KERNEL_API Qt3DRender::QGeometry* trimesh2GeometryLine(trimesh::TriMesh* mesh, Qt3DCore::QNode* parent = nullptr, qtuser_core::Progressor* progressor = nullptr);

BASIC_KERNEL_API Qt3DRender::QGeometry* trimesh2GeometryEdge(trimesh::TriMesh* mesh, Qt3DCore::QNode* parent = nullptr, qtuser_core::Progressor* progressor = nullptr, float angleThreshold = 30.0f);

// mesh only have point or line. no face
BASIC_KERNEL_API Qt3DRender::QGeometry* trimeshPL2Geometry(trimesh::TriMesh* mesh, Qt3DCore::QNode* parent = nullptr, qtuser_core::Progressor* progressor = nullptr);

	//static int twoFaceCollinear(const trimesh::TriMesh::Face& f1, const trimesh::TriMesh::Face& f2, std::vector<int>& faceLine);

//};

#endif // TRIMESH_2_GEOMETRY_RENDERER_H
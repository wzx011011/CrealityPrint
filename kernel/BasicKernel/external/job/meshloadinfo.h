#ifndef _NULLSPACE_MESHLOADINFO_1591622013054_H
#define _NULLSPACE_MESHLOADINFO_1591622013054_H
#include <QtCore/QObject>
#include "trimesh2/TriMesh.h"
#include <Qt3DRender/QGeometry>
#include "data/trimeshheader.h"
#include "data/geometrydata.h"
#include "basickernelexport.h"

class BASIC_KERNEL_API MeshLoadInfo: public QObject
{
public:
	MeshLoadInfo(QObject* parent = nullptr);
	virtual ~MeshLoadInfo();

	trimesh::TriMesh* takeOptimizedMesh();

	creative_kernel::GeometryData* takeFaceGeometryData();
	creative_kernel::GeometryData* takeLineGeometryData();
	creative_kernel::GeometryData* takeEdgeGeometryData();

	//Qt3DRender::QGeometry* takeFaceGeometry();
	//Qt3DRender::QGeometry* takeLineGeometry();
	//Qt3DRender::QGeometry* takeEdgeGeometry();

	QString m_name;
	trimesh::TriMesh* m_mesh;
	trimesh::TriMesh* m_optimizedMesh;

	creative_kernel::GeometryData* m_faceGeometryData;
	creative_kernel::GeometryData* m_lineGeometryData;
	creative_kernel::GeometryData* m_edgeGeometryData;

	//Qt3DRender::QGeometry* m_faceGeometry;
	//Qt3DRender::QGeometry* m_lineGeometry;
	//Qt3DRender::QGeometry* m_edgeGeometry;
};
#endif // _NULLSPACE_MESHLOADINFO_1591622013054_H

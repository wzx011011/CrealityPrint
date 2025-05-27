#include "creategeometryrendererfromfile.h"

#include "trimesh2/TriMesh.h"
#include "external/utils/trimesh2qgeometryrenderer.h"
#include "external/utils/matrixutil.h"
Qt3DRender::QGeometry* createGeometryRendererFromFile(const char* file, qtuser_3d::Box3D& box, Qt3DCore::QNode* parent)
{
	trimesh::TriMesh* mesh = trimesh::TriMesh::read(file);
	Qt3DRender::QGeometry* geometry = nullptr;

	if (mesh)
	{
		mesh->need_bbox();

		box = triBox2Box3D(mesh->bbox);
		geometry = trimesh2GeometryFace(mesh, parent);
		delete mesh;
	}

	return geometry;
}

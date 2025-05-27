#ifndef TRIMESH_SELECT_H
#define TRIMESH_SELECT_H
#include "basickernelexport.h"
#include <vector>
#include <QtGui/QVector3D>

namespace trimesh
{
	class TriMesh;
}

BASIC_KERNEL_API int trimeshSelect(trimesh::TriMesh* mesh, int faceid, std::vector<QVector3D> &vertexData, float offset = 0.1f);

#endif //
#include "matrixutil.h"

QMatrix4x4 xform2QMatrix(const trimesh::fxform& xf)
{
	QMatrix4x4 m;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m(i, j) = xf(i, j);
		}
	}
	return m;
}

trimesh::fxform qMatrix2Xform(const QMatrix4x4& matrix)
{
	trimesh::fxform xf;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			xf(i, j) = matrix(i, j);
		}
	}
	return xf;
}

qtuser_3d::Box3D triBox2Box3D(const trimesh::box3& box)
{
	QVector3D bmin = QVector3D(box.min.x, box.min.y, box.min.z);
	QVector3D bmax = QVector3D(box.max.x, box.max.y, box.max.z);
	qtuser_3d::Box3D box3d(bmin, bmax);
	return box3d;
}


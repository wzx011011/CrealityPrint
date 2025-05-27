#ifndef creative_kernel_MATRIX_UTIL_H
#define creative_kernel_MATRIX_UTIL_H
#include "basickernelexport.h"
#include <QtGui/QMatrix4x4>
#include "trimesh2/XForm.h"
#include "trimesh2/Box.h"
#include "qtuser3d/math/box3d.h"

BASIC_KERNEL_API QMatrix4x4 xform2QMatrix(const trimesh::fxform& xf);
BASIC_KERNEL_API trimesh::fxform qMatrix2Xform(const QMatrix4x4& matrix);
BASIC_KERNEL_API qtuser_3d::Box3D triBox2Box3D(const trimesh::box3& box);


#endif // creative_kernel_MATRIX_UTIL_H
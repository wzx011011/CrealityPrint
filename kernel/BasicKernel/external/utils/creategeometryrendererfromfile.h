#ifndef _NULLSPACE_CREATEGEOMETRYRENDERERFROMFILE_1590638435816_H
#define _NULLSPACE_CREATEGEOMETRYRENDERERFROMFILE_1590638435816_H
#include "basickernelexport.h"
#include <Qt3DRender/QGeometry>
#include "qtuser3d/math/box3d.h"

BASIC_KERNEL_API Qt3DRender::QGeometry* createGeometryRendererFromFile(const char* file, qtuser_3d::Box3D& box, Qt3DCore::QNode* parent);

#endif // _NULLSPACE_CREATEGEOMETRYRENDERERFROMFILE_1590638435816_H

#ifndef _NULLSPACE_COLLIDELINES_1591247228278_H
#define _NULLSPACE_COLLIDELINES_1591247228278_H
#include <QtCore/QObject>
#include "qtuser3d/math/plane.h"
#include "trimesh2/TriMesh.h"

class CollideLines: public QObject
{
public:
	CollideLines(QObject* parent = nullptr);
	virtual ~CollideLines();
};
#endif // _NULLSPACE_COLLIDELINES_1591247228278_H

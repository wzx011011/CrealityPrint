#ifndef _NULLSPACE_SPLITPLANE_1592443125497_H
#define _NULLSPACE_SPLITPLANE_1592443125497_H
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/math/box3d.h"

class SplitPlane: public qtuser_3d::BasicEntity
{
public:
	SplitPlane(Qt3DCore::QNode* parent = nullptr);
	virtual ~SplitPlane();

	void setWidthHeight(float width, float height);
	void updateBox(qtuser_3d::Box3D box);
protected:
	qtuser_3d::BasicEntity* m_plane;
	qtuser_3d::BasicEntity* m_grid;

	Qt3DRender::QParameter* m_planeClip;
	Qt3DRender::QParameter* m_gridClip;
};
#endif // _NULLSPACE_SPLITPLANE_1592443125497_H

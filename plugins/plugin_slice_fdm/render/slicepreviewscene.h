#ifndef _NULLSPACE_SLICEPREVIEWSCENE_1589876502240_H
#define _NULLSPACE_SLICEPREVIEWSCENE_1589876502240_H
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QParameter>

#include "trimesh2/Vec.h"
#include <unordered_map>

namespace qtuser_3d
{
	class BasicEntity;
	class LineEntity;
}

class SlicePreviewNode;
class SlicePreviewScene: public Qt3DCore::QEntity
{
	Q_OBJECT
public:
	SlicePreviewScene(Qt3DCore::QNode* parent = nullptr);
	virtual ~SlicePreviewScene();

	void clear();

	void setOnlyLayer(int layer);
	void unsetOnlyLayer();

	void showIndicator(bool isShow);

	void setSolidModel(Qt3DRender::QGeometry* geometry);

	//fdm
	void setGeometry(Qt3DRender::QGeometry* geometry, Qt3DRender::QGeometryRenderer::PrimitiveType type, int vCountPerPatch = -1);
	void visual();
	void setClipValue(const QVector4D& clipValue);
	float getSpeedFlag(int layer, int step);

	void setIndicatorLayerStep(const std::unordered_map<int, trimesh::vec4>& indicatorLayerStep);

	void setColorType(int colorType);
	void setAnimation(int animation);

	void setShowColor(int type, bool isShow);

protected:
	SlicePreviewNode* m_slicePreviewNode;
	qtuser_3d::BasicEntity* m_fdmIndicator;

	std::unordered_map<int, trimesh::vec4> m_indicatorLayerStep;

	qtuser_3d::BasicEntity* m_solidModel;
};
#endif // _NULLSPACE_SLICEPREVIEWSCENE_1589876502240_H

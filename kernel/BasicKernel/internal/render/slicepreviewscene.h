#ifndef _NULLSPACE_SLICEPREVIEWSCENE_1589876502240_H
#define _NULLSPACE_SLICEPREVIEWSCENE_1589876502240_H
#include "data/kernelenum.h"
#include "crslice/gcode/header.h"
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QParameter>

#include "trimesh2/Vec.h"
#include <unordered_map>

namespace qtuser_3d
{
	class PureColorEntity;
	class AlonePointEntity;
	class NozzleEntity;
	class XEntity;
}

namespace creative_kernel
{
	class SlicePreviewNode;
	class SlicePreviewScene : public Qt3DCore::QEntity
	{
		Q_OBJECT
	public:
		SlicePreviewScene(Qt3DCore::QNode* parent = nullptr);
		virtual ~SlicePreviewScene();

		void initialize();
		void clear();

		void setOnlyLayer(int layer);
		void unsetOnlyLayer();

		void setIndicatorVisible(bool isShow);
		void showGCodeType(int type, bool isShow, bool isCuraProducer);
		void setGCodeVisualType(gcode::GCodeVisualType type);

		SlicePreviewNode* previewNode();

		//fdm
		void setGeometry(Qt3DRender::QGeometry* geometry, Qt3DRender::QGeometryRenderer::PrimitiveType type, int vCountPerPatch = -1);
		void setPreviewNodeModelMatrix(const QMatrix4x4& matrix);

		void visual();
		void setClipValue(const QVector2D& clipValue);
		void setIndicatorPos(const QVector3D& position);

		void setAnimation(int animation);
	
		void setLayerHeight(float height);
		void setLineWidth(float width);

	protected:
		float caculateAdaptivePointSize();
		void updatePointEntitySize();

	protected Q_SLOTS:
		void cameraProjectionMatrixChanged(const QMatrix4x4&);

	protected:
		SlicePreviewNode* m_slicePreviewNode;
		qtuser_3d::NozzleEntity* m_fdmIndicator;

		float m_layerHeight;
		float m_lineWidth;

		gcode::GCodeVisualType m_gcodeVisulaType;
	};
}

#endif // _NULLSPACE_SLICEPREVIEWSCENE_1589876502240_H


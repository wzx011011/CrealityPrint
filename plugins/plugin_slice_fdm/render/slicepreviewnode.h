#ifndef _NULLSPACE_SLICEPREVIEWNODE_1590320779367_H
#define _NULLSPACE_SLICEPREVIEWNODE_1590320779367_H
#include "qtuser3d/entity/basicentity.h"
#include <QtGui/QVector4D>

class SlicePreviewNode : public qtuser_3d::BasicEntity
{
public:
	SlicePreviewNode(Qt3DCore::QNode* parent = nullptr);
	virtual ~SlicePreviewNode();

	void setClipValue(const QVector4D& value);
	void updateGeometry(Qt3DRender::QGeometry* geometry);

	void setColorType(int colorType);
	void setAnimation(int animation);

	void setTypeShow(int typepos, bool needshow);

	void setLayerShowScope(int startlayer, int endlayer);
	void clearLayerShowScope();

	void setLayerStartFlagEnable(bool enable);

protected:
	Qt3DRender::QParameter* m_clipValue;
	Qt3DRender::QParameter* m_typeColors;
	Qt3DRender::QParameter* m_speedColors;
	Qt3DRender::QParameter* m_nozzleColors;
	Qt3DRender::QParameter* m_showType;
	Qt3DRender::QParameter* m_animation;

	Qt3DRender::QParameter* m_typecolorsshow;

	Qt3DRender::QParameter* m_layerShow;
	Qt3DRender::QParameter* m_layerStartFlagShow;

	QVariantList m_showTypeValues;
};
#endif // _NULLSPACE_SLICEPREVIEWNODE_1590320779367_H

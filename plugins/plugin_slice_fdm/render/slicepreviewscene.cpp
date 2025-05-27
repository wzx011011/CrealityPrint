#include "slicepreviewscene.h"
#include "render/slicepreviewnode.h"
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/entity/lineentity.h"
#include "qtuser3d/utils/primitiveshapecache.h"

#include <QMatrix4x4>

#include "qtuser3d/effect/effectmanager.h"
#include <Qt3DRender/QTexture>

#include "interface/reuseableinterface.h"

using namespace qtuser_3d;
SlicePreviewScene::SlicePreviewScene(Qt3DCore::QNode* parent)
	:QEntity(parent)
{
	m_slicePreviewNode = new SlicePreviewNode(this);

	m_fdmIndicator = new BasicEntity(this);
	m_fdmIndicator->setGeometry(PRIMITIVESHAPE("pen"));
	m_fdmIndicator->setEffect(EFFECTCREATE("modelsimple", m_fdmIndicator));

	Qt3DRender::QBlendEquationArguments* blend = m_fdmIndicator->blendArguments();
	if (blend)
	{
		blend->setSourceRgba(Qt3DRender::QBlendEquationArguments::SourceAlpha);
		blend->setDestinationRgba(Qt3DRender::QBlendEquationArguments::OneMinusSourceAlpha);
	}

	m_solidModel = new BasicEntity(this);
	m_solidModel->setEffect(creative_kernel::getCachedModelEffect());
}

SlicePreviewScene::~SlicePreviewScene()
{
	clear();
}

void SlicePreviewScene::clear()
{
	//m_slicePreviewNode->setGeometry(nullptr);
	m_solidModel->setGeometry(nullptr);
}

void SlicePreviewScene::setColorType(int colorType)
{
	m_slicePreviewNode->setColorType(colorType);
}

void SlicePreviewScene::setAnimation(int animation)
{
	m_slicePreviewNode->setAnimation(animation);
}

void SlicePreviewScene::setSolidModel(Qt3DRender::QGeometry* geometry)
{
	m_solidModel->setGeometry(geometry);
}

void SlicePreviewScene::visual()
{
	m_slicePreviewNode->setEnabled(true);
	m_fdmIndicator->setEnabled(true);

	qDebug() << "fdm preview visual";
}

void SlicePreviewScene::setOnlyLayer(int layer)
{
	m_slicePreviewNode->setLayerShowScope(layer, layer);
}

void SlicePreviewScene::unsetOnlyLayer()
{
	m_slicePreviewNode->clearLayerShowScope();
}

void SlicePreviewScene::setClipValue(const QVector4D& clipValue)
{
	if (m_fdmIndicator != nullptr)
	{
		int i = (int)clipValue.y() * 100000 + (int)clipValue.w();
		trimesh::vec4 pos = m_indicatorLayerStep[i];
		QVector3D transPos(pos[0], pos[1], pos[2]);
		QMatrix4x4 tansMatrix;
		tansMatrix.translate(transPos);
		m_fdmIndicator->setPose(tansMatrix);
	}
	m_slicePreviewNode->setClipValue(clipValue);
}

float SlicePreviewScene::getSpeedFlag(int layer, int step)
{
	if (m_fdmIndicator != nullptr)
	{
		int i = (int)layer * 100000 + (int)step;
		trimesh::vec4 pos = m_indicatorLayerStep[i];
		int t = (int)pos[3];
		if (t == 0)
		{
			t = 1;
		}
		float speed = sqrt((float)t);	
		return speed;
	}
	return 1.0;
}

void SlicePreviewScene::setGeometry(Qt3DRender::QGeometry* geometry, Qt3DRender::QGeometryRenderer::PrimitiveType type, int vCountPerPatch)
{
	m_slicePreviewNode->setGeometry(geometry, type, vCountPerPatch);
}

void SlicePreviewScene::setIndicatorLayerStep(const std::unordered_map<int, trimesh::vec4>& indicatorLayerStep)
{
	m_indicatorLayerStep = indicatorLayerStep;
}

void SlicePreviewScene::showIndicator(bool isShow)
{
	m_fdmIndicator->setEnabled(isShow);
}

void SlicePreviewScene::setShowColor(int type, bool isShow)
{
	m_slicePreviewNode->setTypeShow(type, isShow);
}
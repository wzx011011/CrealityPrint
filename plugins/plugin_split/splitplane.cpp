#include "splitplane.h"
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/geometry/gridcreatehelper.h"

using namespace qtuser_3d;
SplitPlane::SplitPlane(Qt3DCore::QNode* parent)
	:BasicEntity(parent)
	, m_grid(nullptr)
{
	m_plane = new BasicEntity(this);
	//m_grid = new BasicEntity(this);

	m_plane->createParameter("borderWidth", 0.4);

	QVector4D color = QVector4D(246.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1.0f);
	//m_grid->createParameter("color", color);
	color.setW(0.1f);
	m_plane->createParameter("color", color);

	//m_grid->setEffect(EFFECT("splitplane"));
	m_plane->setEffect(EFFECTCREATE("splitplane.alpha", m_plane));

	Qt3DRender::QBlendEquationArguments* blend = m_plane->blendArguments();
	if (blend)
	{
		blend->setSourceRgba(Qt3DRender::QBlendEquationArguments::SourceAlpha);
		blend->setDestinationRgba(Qt3DRender::QBlendEquationArguments::OneMinusSourceAlpha);
	}

	QVariantList values = QVariantList();
	values << QVector3D(-50.0f, -50.0f, -50.0f) << QVector3D(50.0f, 50.0f, 50.0f);

	//m_gridClip = m_grid->createParameter("clip[0]", values);
	m_planeClip = m_plane->createParameter("clip[0]", values);
}

SplitPlane::~SplitPlane()
{
}

void SplitPlane::setWidthHeight(float width, float height)
{
    m_plane->setGeometry(GridCreateHelper::createPlane(width, height));
	//m_grid->setGeometry(GridCreateHelper::createPlane(width, height, false), Qt3DRender::QGeometryRenderer::Lines);
}

void SplitPlane::updateBox(Box3D box)
{
	QVector3D offset = QVector3D(20.0f, 20.0f, 20.0f);
	QVariantList values = QVariantList();
	values << (box.min - offset) << (box.max + offset);

	//m_gridClip->setValue(values);
	m_planeClip->setValue(values);
}

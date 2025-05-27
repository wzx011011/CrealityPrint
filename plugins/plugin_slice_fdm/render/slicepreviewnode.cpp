#include "slicepreviewnode.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/effect/ueffect.h"
#include <QVector2D>
#include "interface/renderinterface.h"
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QCullFace>

SlicePreviewNode::SlicePreviewNode(Qt3DCore::QNode* parent)
	:BasicEntity(parent)
{
	//setEffect(EFFECTCREATE("slicepreview", m_material));
	//setEffect(EFFECTCREATE("slicepreviewttt", m_material));
	qtuser_3d::UEffect* effect = (qtuser_3d::UEffect*)EFFECTCREATE("slicepreviewNoTess", m_material);
	setEffect(effect);
	Qt3DRender::QRenderPass* pass = effect->traitRenderPass("slicepreviewNoTess");
	if (pass)
	{
		//add render state
		Qt3DRender::QCullFace* cullFace = new Qt3DRender::QCullFace(pass);
		cullFace->setMode(Qt3DRender::QCullFace::NoCulling);
		pass->addRenderState(cullFace);
	}

	m_clipValue = createParameter("clipValue", QVector4D(0.0f, 200.0f, 0.0f, 300.0f));
	m_showType = createParameter("showType", 1);
	m_animation = createParameter("animation", 0);
	m_layerShow = createParameter("layershow", QVector2D(-1.0f, 9999999.0f));
	m_layerStartFlagShow = createParameter("layerstartflag_show", 1);

	QVariantList values = QVariantList();
	values << QVector4D(0.5f, 0.5f, 0.5f, 1.0f)
		<< QVector4D(0.47f, 0.18f, 0.16f, 1.0f)
		<< QVector4D(0.01f, 0.55f, 0.02f, 1.0f)
		<< QVector4D(1.00f, 0.7f, 0.5f, 1.0f)
		<< QVector4D(0.02f, 0.55f, 0.55f, 1.0f)
		<< QVector4D(0.32f, 0.12f, 0.33f, 1.0f)
		<< QVector4D(0.90f, 0.86f, 0.20f, 1.0f)
		<< QVector4D(0.71f, 0.74f, 0.22f, 1.0f)
		<< QVector4D(1.00f, 0.42f, 0.27f, 1.0f)
		<< QVector4D(0.40f, 0.20f, 0.30f, 1.0f)
		<< QVector4D(0.84f, 0.23f, 0.07f, 1.0f)
		<< QVector4D(0.20f, 0.60f, 0.10f, 1.0f)
		<< QVector4D(0.90f, 0.30f, 0.20f, 1.0f)
		<< QVector4D(0.37f, 0.34f, 0.37f, 1.0f)
		<< QVector4D(1.00f, 1.00f, 1.00f, 1.0f)
		<< QVector4D(0.00f, 0.00f, 1.00f, 1.0f)
		<< QVector4D(1.00f, 0.00f, 0.00f, 1.0f);
	m_typeColors = createParameter("typecolors[0]", values);

	values.clear();
	values<< QVector4D(0.38f, 0.60f, 0.33f, 1.0f)
		<< QVector4D(0.20f, 0.60f, 0.20f, 1.0f)
		<< QVector4D(0.58f, 0.88f, 1.00f, 1.0f)
		<< QVector4D(0.00f, 0.80f, 1.00f, 1.0f)
		<< QVector4D(0.00f, 0.43f, 0.60f, 1.0f)
		<< QVector4D(1.00f, 0.78f, 0.42f, 1.0f)
		<< QVector4D(1.00f, 0.60f, 0.00f, 1.0f)
		<< QVector4D(0.60f, 0.38f, 0.02f, 1.0f)
		<< QVector4D(1.00f, 0.40f, 0.70f, 1.0f)
		<< QVector4D(1.00f, 0.00f, 0.60f, 1.0f);
	m_speedColors = createParameter("speedcolors[0]", values);

	values.clear();
	values << QVector4D(1.0f, 0.50f, 0.50f, 1.0f)
		<< QVector4D(0.50f, 1.00f, 0.50f, 1.0f)
		<< QVector4D(0.50f, 0.50f, 1.00f, 1.0f)
		<< QVector4D(1.00f, 1.00f, 0.50f, 1.0f)
		<< QVector4D(1.00f, 0.50f, 1.00f, 1.0f)
		<< QVector4D(0.50f, 1.00f, 1.00f, 1.0f);

	m_nozzleColors = createParameter("nozzlecolors[0]", values);

	m_showTypeValues.clear();
	for(int i = 0; i < 18; i++)
		m_showTypeValues << 0;
	m_showTypeValues[12] = 0;

	m_typecolorsshow = createParameter("typecolorsshow[0]", m_showTypeValues);
}

SlicePreviewNode::~SlicePreviewNode()
{
}

void SlicePreviewNode::setTypeShow(int typepos, bool needshow)
{
	if (typepos >= 0 && typepos < m_showTypeValues.size())
	{
		m_showTypeValues[typepos] = needshow ? 1 : 0;
	}
	m_typecolorsshow->setValue(m_showTypeValues);
	creative_kernel::renderOneFrame();
}

void SlicePreviewNode::updateGeometry(Qt3DRender::QGeometry* geometry)
{
	setGeometry(geometry);
	qDebug() << "SlicePreviewNode::updateGeometry";
}

void SlicePreviewNode::setClipValue(const QVector4D& value)
{
	m_clipValue->setValue(value);
}

void SlicePreviewNode::setColorType(int colorType)
{
	if (colorType >= 0 && colorType <= 2)
	{
		m_showType->setValue(colorType);
	}
}

void SlicePreviewNode::setAnimation(int animation)
{
	m_animation->setValue(animation);
}

void SlicePreviewNode::setLayerShowScope(int startlayer, int endlayer)
{
	m_layerShow->setValue(QVector2D(startlayer, endlayer));
}

void SlicePreviewNode::clearLayerShowScope()
{
	m_layerShow->setValue(QVector2D(-1.0f, 9999999.0f));
}

void SlicePreviewNode::setLayerStartFlagEnable(bool enable)
{
	m_layerStartFlagShow->setValue(enable ? 1 : 0);
}


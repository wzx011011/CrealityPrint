#include "slicepreviewscene.h"
#include "entity/purecolorentity.h"
#include "qtuser3d/camera/screencamera.h"
#include "entity/alonepointentity.h"
#include "data/kernelmacro.h"
#include "internal/render/slicepreviewnode.h"
#include "interface/reuseableinterface.h"
#include "interface/camerainterface.h"
#include <cmath>
#include "entity/nozzleentity.h"

#include "gcode/gcode/sliceline.h"
#include "cxkernel/data/trimeshutils.h"
#include "data/modeln.h"
#include <QtCore/QCoreApplication>

#ifdef _DEBUG
#include "buildinfo.h"
#endif

using namespace qtuser_3d;
namespace creative_kernel
{
	SlicePreviewScene::SlicePreviewScene(Qt3DCore::QNode* parent)
		:QEntity(parent),
		m_layerHeight(0.1),
		m_lineWidth(0.4)
	{
		m_slicePreviewNode = new SlicePreviewNode(this);
		m_fdmIndicator = new NozzleEntity(this);
		{

#ifdef _DEBUG
			QString path = SOURCE_ROOT + QString("/resources/mesh/");
#elif defined (__APPLE__)
			int index = QCoreApplication::applicationDirPath().lastIndexOf("/");
			QString path = QCoreApplication::applicationDirPath().left(index) + QString("/Resources/resources/mesh/");
#else
			QString path = QCoreApplication::applicationDirPath() + QString("/resources/mesh/");
#endif

			QString filePath = path + "fdm_nozzle.stl";
			TriMeshPtr meshPtr = cxkernel::loadMeshFromName(filePath, nullptr);
			if (meshPtr)
			{
				m_fdmIndicator->setGeometry(creative_kernel::createGeometryFromMesh(meshPtr.get()));
			}
		}
	}

	SlicePreviewScene::~SlicePreviewScene()
	{
		clear();
	}

	void SlicePreviewScene::initialize()
	{
		m_slicePreviewNode->initialize();

		qtuser_3d::ScreenCamera* sc = visCamera();
		Qt3DRender::QCamera* camera = sc->camera();
		connect(camera, SIGNAL(projectionMatrixChanged(const QMatrix4x4 &)), this, SLOT(cameraProjectionMatrixChanged(const QMatrix4x4&)));
	}

	void SlicePreviewScene::clear()
	{

	}

	void SlicePreviewScene::setGCodeVisualType(gcode::GCodeVisualType type)
	{
		m_gcodeVisulaType = type;
		m_slicePreviewNode->setGCodeVisualType((int)type);
	}

	SlicePreviewNode* SlicePreviewScene::previewNode()
	{
		return m_slicePreviewNode;
	}

	void SlicePreviewScene::setAnimation(int animation)
	{
		m_slicePreviewNode->setAnimation(animation);
	}

	void SlicePreviewScene::visual()
	{
		m_fdmIndicator->setEnabled(true);
	}

	void SlicePreviewScene::setOnlyLayer(int layer)
	{
		m_slicePreviewNode->setLayerShowScope(layer, layer);
	}

	void SlicePreviewScene::unsetOnlyLayer()
	{
		m_slicePreviewNode->clearLayerShowScope();
	}

	void SlicePreviewScene::setClipValue(const QVector2D& clipValue)
	{
		m_slicePreviewNode->setClipValue(clipValue);
	}

	void SlicePreviewScene::setIndicatorPos(const QVector3D& position)
	{
		QMatrix4x4 tansMatrix;
		tansMatrix.translate(position);
		m_fdmIndicator->setPose(tansMatrix);
	}

	void SlicePreviewScene::setGeometry(Qt3DRender::QGeometry* geometry, Qt3DRender::QGeometryRenderer::PrimitiveType type, int vCountPerPatch)
	{
		m_slicePreviewNode->setGeometry(geometry, Qt3DRender::QGeometryRenderer::Points);
	}

	void SlicePreviewScene::setPreviewNodeModelMatrix(const QMatrix4x4& matrix)
	{
		m_slicePreviewNode->setModelMatrix(matrix);
	}

	void SlicePreviewScene::setIndicatorVisible(bool isShow)
	{
		m_fdmIndicator->setEnabled(isShow);
	}

	void SlicePreviewScene::showGCodeType(int type, bool isShow, bool isCuraProducer)
	{
		m_slicePreviewNode->setTypeShow(type, isShow);
	}

	// ���������ϢԤ����һ�����ʵ�Բ�뾶
	float SlicePreviewScene::caculateAdaptivePointSize()
	{
		if (m_lineWidth <= 0.0 || m_layerHeight <= 0.0)
		{
			return 2.0;
		}

		qtuser_3d::ScreenCamera* sc = visCamera();

		QVector3D origin = sc->orignCenter();
		QVector3D right = sc->horizontal();
		QVector3D p = origin + right.normalized() * 1.0;

		const QMatrix4x4 vp = sc->projectionMatrix() * sc->viewMatrix();

		QVector4D clipO = vp * QVector4D(origin, 1.0);
		QVector3D ndcO = QVector3D(clipO) / clipO.w();
		QVector2D uvO = QVector2D(ndcO);

		QVector4D clipP = vp * QVector4D(p, 1.0);
		QVector3D ndcP = QVector3D(clipP) / clipP.w();
		QVector2D uvP = QVector2D(ndcP);

		float scaleX = uvP.x() - uvO.x();
		float pixels = sc->size().width() * scaleX * 0.5;

		//��ͬ�۲�Ƕȴ�С��һ������ֱ���򿴵��Ĵ�����߿��Ĵ�С��ˮƽ��������ǲ�߰��С
		{
			float lineWidth = m_lineWidth, layerHeight = m_layerHeight * 2.0;
			float maxSize = lineWidth * 1.25, minSize = layerHeight * 1.25;

			QVector3D viewDir = sc->camera()->viewVector().normalized();
			float k = QVector3D::dotProduct(viewDir, QVector3D(0.0, 0.0, 1.0));
			k = abs(k);
			k = minSize + (maxSize - minSize) * k;
			pixels *= k;
		}

		pixels = fmax(pixels, 2.0);
		pixels = ceil(pixels);

		return pixels;
	}

	void SlicePreviewScene::updatePointEntitySize()
	{
	}

	void SlicePreviewScene::cameraProjectionMatrixChanged(const QMatrix4x4& mat)
	{
		updatePointEntitySize();
	}

	void SlicePreviewScene::setLayerHeight(float height)
	{
		m_layerHeight = height;

		m_slicePreviewNode->setLayerHeight(height);

		updatePointEntitySize();
	}

	void SlicePreviewScene::setLineWidth(float width)
	{
		m_lineWidth = width;
		updatePointEntitySize();
	}
}
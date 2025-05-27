#include "slicepreviewflow.h"
#include "render/slicepreviewscene.h"

#include "qtuser3d/framegraph/surface.h"
#include "qtuser3d/framegraph/colorpicker.h"
#include "qtuser3d/framegraph/texturerendertarget.h"
#include "qtusercore/string/stringtool.h"

#include "main/sliceattain.h"

#include "interface/renderinterface.h"
#include "interface/camerainterface.h"
#include "interface/reuseableinterface.h"
#include "interface/spaceinterface.h"
#include "interface/eventinterface.h"

#include "qtuser3d/camera/screencamera.h"

#include <QTimer>

using namespace creative_kernel;


SlicePreviewFlow::SlicePreviewFlow(QObject* parent)
	:RenderGraph()
	, m_currentLayer(0)
	, m_currentStep(0)
	, m_animationSpeed(10)
	, m_animation(false)
	, m_totalNum(0)
	, m_currentNum(0)
{
	m_scene = new SlicePreviewScene(nullptr);
	m_surface = new qtuser_3d::Surface();
	m_surface->setClearColor(QColor("transparent"));

	m_textureRenderTarget = new qtuser_3d::TextureRenderTarget(m_scene);

	m_colorPicker = new qtuser_3d::ColorPicker(m_surface);
	m_colorPicker->useSelfCameraSelector(true);
	m_colorPicker->setFilterKey("r2t", 0);
	m_colorPicker->setEnabled(true);
	m_colorPicker->setClearColor(QColor(0, 0, 0, 0));
	QSize size = QSize(400, 300);
	m_colorPicker->resize(size);
	m_colorPicker->setDebugName("slicer2t.bmp");
	m_colorPicker->setTextureRenderTarget(m_textureRenderTarget);

	m_animationTimer = new QTimer(this);
	connect(m_animationTimer, SIGNAL(timeout()), this, SLOT(animateStep()));

}

void SlicePreviewFlow::initialize()
{
}

SlicePreviewFlow::~SlicePreviewFlow()
{
	delete m_scene;
}

SlicePreviewScene* SlicePreviewFlow::scene()
{
	return m_scene;
}

void SlicePreviewFlow::requestPreview(requestCallFuncImage func)
{
	qDebug() << "fdm slice preview request preview";
	m_colorPicker->setRequestCallback(func);

	m_colorPicker->setFilterKey("rt", 0);
	QSize size = QSize(600, 600);
	qtuser_3d::ScreenCamera* screenCamera = visCamera();
	m_textureRenderTarget->resize(size);

	Qt3DRender::QCamera* pickerCamera = m_colorPicker->camera();
	pickerCamera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
	Qt3DRender::QCamera* camera = getCachedCameraEntity();

	float np = camera->nearPlane();
	float fp = camera->farPlane();

	pickerCamera->setNearPlane(0.1);
	pickerCamera->setFarPlane(1000);
	//pickerCamera->setFieldOfView(camera->fieldOfView());
	pickerCamera->setAspectRatio(1.0f);

	float depression_angle = 8;  //预览图俯视角
	qtuser_3d::Box3D box;
	if(m_attain)
		box = m_attain->box();
	QVector3D bsize = box.size();
	float newDistance = 1.5f * qMax(qMax(bsize.x(), bsize.y() / 2), bsize.z());
	QVector3D ViewCenter = box.center(); //视点
	float y = ViewCenter.y() - newDistance;
	float z = newDistance * tan(depression_angle * M_PIl / 180) + ViewCenter.z();
	QVector3D cameraPos = QVector3D(ViewCenter.x(), y, z);
	pickerCamera->setPosition(cameraPos);
	pickerCamera->setViewCenter(ViewCenter);

	auto radians = atan2(newDistance, ViewCenter.z());
	auto filedOfView = radians * (180 / M_PIl);

	pickerCamera->setFieldOfView(filedOfView);

	spaceEntity()->setParent(m_scene);
	m_colorPicker->requestCapture();
	renderOneFrame();
	//pickerCamera->setNearPlane(np);
	//pickerCamera->setFarPlane(fp);

	qDebug() << "fdm SlicePreviewFlow::fdmRequestPreview";
}

void SlicePreviewFlow::endRequest()
{
	qDebug() << "fdm slice preview end request";
	spaceEntity()->setParent((Qt3DCore::QNode*)nullptr);
	m_surface->setViewport(QRectF(0.0f, 0.0f, 1.0f, 1.0f));
	renderOneFrame();
}

void SlicePreviewFlow::notifyClipValue()
{
	if (m_attain.isNull())
		return;

	int currentStep = m_currentStep;
	if (currentStep >= m_attain->steps(m_currentLayer))
	{
		currentStep = m_attain->steps(m_currentLayer) - 1;
	}
	QVector4D clipValue(0.0f, (float)m_currentLayer, 0.0f, (float)currentStep);
	m_scene->setClipValue(clipValue);

	renderOneFrame();
}

bool SlicePreviewFlow::isAvailable()
{
	return !m_attain.isNull();
}

void SlicePreviewFlow::slicePreviewSetOp(QString opname, int param1, int param2)
{
	if (m_attain.isNull())
	{
		qInfo() << "dlp no date. return";
		return;
	}
	opname = opname.toLower();
	if (opname == "currentlayer")
	{
		int layer = param1;
		int randomstep = param2;

		m_currentLayer = layer;
		if (randomstep)
		{
			m_currentStep = qrand() % m_attain->steps(m_currentLayer);
		}

		emit stepsChanged();
		{
			notifyClipValue();
		}
	}
	else if (opname == "currentstep")
	{
		m_currentStep = param1;
		notifyClipValue();
	}
	else if (opname == "animationspeed")
	{
		m_animationSpeed = param1;
	}
	else if (opname == "isanimate")
	{
		m_scene->setAnimation(param1);
	}
	else if (opname == "onlylayer")
	{
		m_scene->setOnlyLayer(param1);
		renderOneFrame();
	}
	else
	{
		qCritical() << "fdm no support operation " << opname;
	}
}

void SlicePreviewFlow::slicePreviewCommonOp(QString opname)
{
	opname = opname.toLower();
	if (opname == "animate")
	{
		if (m_animation) return;

		m_animation = true;
		m_scene->setAnimation(1);
		m_animationTimer->start(m_animationSpeed);
		m_currentStep = 0;
	}
	else if (opname == "unsetonlylayer")
	{
		m_scene->unsetOnlyLayer();
		renderOneFrame();
	}
	else
	{
		qCritical() << "fdm no support operation " << opname;
	}
}

float SlicePreviewFlow::slicePreviewGetOp(QString opname)
{
	opname = opname.toLower();
	if (opname == "speedflags")
	{
		return m_scene->getSpeedFlag(m_currentLayer, m_currentStep);
	}
	else
	{
		qCritical() << "fdm no support operation " << opname;
		return 0;
	}
	return 0;
}

int SlicePreviewFlow::layers()
{
	return m_attain.data() ? m_attain->layers() : 0;
}

int SlicePreviewFlow::steps()
{
	if (!m_attain.data())
		return 0;

	if (m_currentLayer < 0 || m_currentLayer >= m_attain->layers())
		return 0;

	return m_attain->steps(m_currentLayer);
}

void SlicePreviewFlow::clear()
{
	m_attain.reset();
}

void SlicePreviewFlow::setSliceAttain(SliceAttain* attain)
{
	qDebug() << "fdm slice preview set slice data";
	m_attain.reset(attain);

	int stepCount = 0;

	if (m_attain.data())
	{
		Qt3DRender::QGeometry* geometry = nullptr;
		std::unordered_map<int, trimesh::vec4> indicator;
		m_attain->take(&geometry, indicator);
		//m_scene->setGeometry(geometry, Qt3DRender::QGeometryRenderer::Patches, 2);
		m_scene->setGeometry(geometry, Qt3DRender::QGeometryRenderer::Triangles);
		m_scene->setIndicatorLayerStep(indicator);

		int layers = m_attain.data()->layers();
		for (int i = 0; i < layers; i++)
		{
			stepCount += m_attain.data()->steps(i);
		}
	}

	// 模型越大，从加载到显示所需的时间会越多，甚至会大于 1 秒，
	// 所以需要先开启持续渲染，直到模型显示出来后，再换回按需渲染，
	// 以避免加载时间超出按需阈值，所导致的模型不显示
	setContinousRender();
	// 持续渲染模式时长
	int continousMSecs = stepCount / 160;
	QTimer::singleShot(continousMSecs, [](){ setCommandRender(); });

	emit layersChanged();
}

SliceAttain* SlicePreviewFlow::attain()
{
	return m_attain.data();
}

SliceAttain* SlicePreviewFlow::takeAttain()
{
	return m_attain.take();
}

QStringList SlicePreviewFlow::getCurrentLayerGCodeList()
{
	QStringList strLayerLineList;
	if (!m_attain)
		return strLayerLineList;
	
	QString strLayerCode = QString::fromStdString(m_attain->layerGcode(m_currentLayer));
	strLayerLineList = strLayerCode.split("\n");
	int nStepCount = 0;
	int nLineCount = 0;
	m_vecStepRow.clear();
    foreach (QString strLine, strLayerLineList)
	{
		if (strLine.contains("G") && strLine.contains("X") && strLine.contains("Y") && strLine.contains("E"))
		{
			m_vecStepRow.push_back(nLineCount);

			nStepCount++;
		}
		nLineCount++;
	}
	return strLayerLineList;
}


int SlicePreviewFlow::findViewIndexFromStep(int nStep)
{
	if (m_vecStepRow.size() > nStep)
	{
		return m_vecStepRow[nStep];
	}
	return -1;
}

int SlicePreviewFlow::findStepFromViewIndex(int nViewIndex)
{
	int index = m_vecStepRow.indexOf(nViewIndex);
	return  index;
}
void SlicePreviewFlow::animateStep()
{
	if (m_attain.isNull())
		return;

	++m_currentStep;
	if (m_currentStep < m_attain->steps(m_currentLayer))
	{
		m_animationTimer->start(m_animationSpeed);
		QVector4D clipValue((float)m_currentLayer, (float)m_currentLayer, 0.0f, (float)m_currentStep);
		m_scene->setClipValue(clipValue);
	}
	else
	{
		m_animation = false;
		m_scene->setAnimation(0);
		QVector4D clipValue(0.0f, (float)m_currentLayer, 0.0f, (float)m_currentStep);
		m_scene->setClipValue(clipValue);
	}

	renderOneFrame();
}

Qt3DCore::QEntity* SlicePreviewFlow::sceneGraph()
{
	return m_scene;
}

Qt3DRender::QFrameGraphNode* SlicePreviewFlow::frameGraph()
{
	return m_surface;
}

void SlicePreviewFlow::begineRender()
{
	qInfo() << "fdm slice preview begineRender";
	m_surface->setCamera(getCachedCameraEntity());

	if (!m_attain.data())
	{
		spaceEntity()->setParent(m_scene);
	}
	else
	{
		m_scene->visual();
	}

#ifdef _DEBUG
	addKeyEventHandler(this);
#endif
}

void SlicePreviewFlow::endRender()
{
	qInfo() << "fdm slice preview endRender";
	m_surface->setCamera(nullptr);
	m_scene->clear();
	spaceEntity()->setParent((Qt3DCore::QNode*)nullptr);
	setModelEffectClipMaxZ();

#ifdef _DEBUG
	removeKeyEventHandler(this);
#endif
}

void SlicePreviewFlow::updateRenderSize(QSize size)
{
	if (size.width() == 0 || size.height() == 0)
		return;

	m_surface->updateSurfaceSize(size);
	m_colorPicker->resize(size);
	m_textureRenderTarget->resize(size);
}

void SlicePreviewFlow::onKeyPress(QKeyEvent* event)
{
	if (event->key() == Qt::Key_P && m_attain.data())
	{
		auto f = [this](QImage& image) {
			image.save("fdm.bmp");
			endRequest();
		};

		requestPreview(f);
	}
}

void SlicePreviewFlow::onKeyRelease(QKeyEvent* event)
{

}

void SlicePreviewFlow::setShowColor(int type, bool isShow)
{
	m_scene->setShowColor(type, isShow);
	renderOneFrame();
}

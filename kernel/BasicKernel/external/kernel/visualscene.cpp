#include "visualscene.h"
#include <QDebug>

#include "external/interface/undointerface.h"
#include "external/interface/eventinterface.h"
#include "external/interface/spaceinterface.h"
#include "external/interface/reuseableinterface.h"

#include "qtuser3d/framegraph/colorpicker.h"
#include "qtuser3d/framegraph/surface.h"
#include "qtuser3d/framegraph/texturerendertarget.h"

#include "interface/appsettinginterface.h"

#include "internal/utils/visscenekeyhandler.h"
#include "internal/utils/visscenehoverhandler.h"
#include "internal/utils/vissceneleftmousehandler.h"
namespace creative_kernel
{
	VisualScene::VisualScene(Qt3DCore::QNode* parent)
		:RenderGraph(parent)
		, m_handlerEnabled(true)
	{
		m_rootEntity = new Qt3DCore::QEntity();

		m_surface = new qtuser_3d::Surface();
        m_surface->setClearColor(getSettingColor("modelui_basecolor"));
		//m_surface->setClearColor(QColor(0xbb, 0xbb, 0xbb));

		m_colorPicker = new qtuser_3d::ColorPicker();
		m_colorPicker->setParent(m_surface->getCameraViewportFrameGraphNode());
		
		m_textureRenderTarget = new qtuser_3d::TextureRenderTarget(m_rootEntity);
		m_colorPicker->setTextureRenderTarget(m_textureRenderTarget);

		connect(m_colorPicker, SIGNAL(signalUpdate()), this, SIGNAL(signalUpdate()));

		m_keyHandler = new VisSceneKeyHandler(this);
		m_hoverHandler = new VisSceneHoverHandler(this);
		m_leftMouseHandler = new VisSceneLeftMouseHandler(this);
	}
	
	VisualScene::~VisualScene()
	{
		if (m_rootEntity != nullptr)
		{
			delete m_rootEntity;
			m_rootEntity = nullptr;
		}
	}

	qtuser_3d::FacePicker* VisualScene::facePicker()
	{
		return m_colorPicker;
	}

	void VisualScene::updateRender(bool updatePick)
	{
		if (updatePick)
			m_colorPicker->requestCapture();
		
		emit signalUpdate();
	}

	void VisualScene::show(Qt3DCore::QEntity* entity)
	{
		if (entity || entity->parent() == m_rootEntity)
		{
			entity->setParent(m_rootEntity);
		}
	}

	void VisualScene::hide(Qt3DCore::QEntity* entity)
	{
		if (entity && (entity->parent() == m_rootEntity))
		{
			entity->setParent((Qt3DCore::QNode*)nullptr);
		}
	}

	void VisualScene::enableHandler(bool enable)
	{
		if (enable == m_handlerEnabled)
			return;

		m_handlerEnabled = enable;
		if (m_handlerEnabled)
		{
			addKeyEventHandler(m_keyHandler);
			addHoverEventHandler(m_hoverHandler);
			addLeftMouseEventHandler(m_leftMouseHandler);
		}
		else
		{
			removeKeyEventHandler(m_keyHandler);
			removeHoverEventHandler(m_hoverHandler);
			removeLeftMouseEventHandler(m_leftMouseHandler);
		}
	}

	Qt3DCore::QEntity* VisualScene::sceneGraph()
	{
		return m_rootEntity;
	}

	Qt3DRender::QFrameGraphNode* VisualScene::frameGraph()
	{
		return m_surface;
	}

	void VisualScene::begineRender()
	{
		qInfo() << "scene begineRender";
		Qt3DRender::QCamera* qcamera = getCachedCameraEntity();
		m_surface->setCamera(qcamera);

		show(spaceEntity());

		addKeyEventHandler(m_keyHandler);
		addHoverEventHandler(m_hoverHandler);
		addLeftMouseEventHandler(m_leftMouseHandler);
		m_handlerEnabled = true;

		bindModelSpaceUndoStack();
	}

	void VisualScene::endRender()
	{
		qInfo() << "scene endRender";
		hide(spaceEntity());

		removeKeyEventHandler(m_keyHandler);
		removeHoverEventHandler(m_hoverHandler);
		removeLeftMouseEventHandler(m_leftMouseHandler);

		bindUndoStack(nullptr);
	}

	void VisualScene::updateRenderSize(QSize size)
	{
		if (size.width() == 0 || size.height() == 0)
			return;
		
		m_surface->updateSurfaceSize(size);
		m_colorPicker->resize(size);
		m_textureRenderTarget->resize(size);
		updateRender(true);
	}
}

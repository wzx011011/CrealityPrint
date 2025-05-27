#include "rendercenter.h"
#include "qtuserqml/gl/glquickitem.h"
#include "qtuser3d/event/eventsubdivide.h"

using namespace qtuser_3d;
namespace creative_kernel
{
	SINGLETON_IMPL(RenderCenter)
	RenderCenter::RenderCenter()
		:QObject()
		,m_glQuickItem(nullptr)
		, m_eventSubdivide(nullptr)
		, m_compute(nullptr)
	{
	}
	
	RenderCenter::~RenderCenter()
	{
	}

	void RenderCenter::setGLQuickItem(GLQuickItem* item)
	{
		m_glQuickItem = item;
		if (m_glQuickItem)
		{
			m_eventSubdivide = m_glQuickItem->eventSubdivide();
	//		m_compute = new OpenGLCompute(this, m_glQuickItem->rawOGL());
		}
	}

	GLQuickItem* RenderCenter::glQuickItem()
	{
		return m_glQuickItem;
	}

	qtuser_qml::RawOGL* RenderCenter::rawOGL()
	{
		return m_glQuickItem->rawOGL();
	}

	qtuser_3d::EventSubdivide* RenderCenter::eventSubdivide()
	{
		return m_eventSubdivide;
	}

	void RenderCenter::uninitialize()
	{
		qDebug() << "RenderCenter uninitialize";
		if (m_glQuickItem) m_glQuickItem->unRegisterAll();
 		if (m_eventSubdivide) m_eventSubdivide->closeHandlers();
	}

	void RenderCenter::registerResidentNode(Qt3DCore::QNode* node)
	{
		if (m_glQuickItem) m_glQuickItem->registerResidentNode(node);
	}

	void RenderCenter::unRegisterResidentNode(Qt3DCore::QNode* node)
	{
		if (m_glQuickItem) m_glQuickItem->unRegisterResidentNode(node);
	}

	void RenderCenter::renderRenderGraph(qtuser_3d::RenderGraph* graph)
	{
		if (m_glQuickItem) m_glQuickItem->renderRenderGraph(graph);
	}

	bool RenderCenter::isRenderRenderGraph(qtuser_3d::RenderGraph* graph)
	{
		if (m_glQuickItem) return m_glQuickItem->isRenderRenderGraph(graph);
		return false;
	}

	void RenderCenter::registerRenderGraph(qtuser_3d::RenderGraph* graph)
	{
		if (m_glQuickItem) m_glQuickItem->registerRenderGraph(graph);
	}

	void RenderCenter::unRegisterRenderGraph(qtuser_3d::RenderGraph* graph)
	{
		if (m_glQuickItem) m_glQuickItem->unRegisterRenderGraph(graph);
	}

	void RenderCenter::renderOneFrame()
	{
		if (m_glQuickItem) m_glQuickItem->requestUpdate();
	}

	void RenderCenter::compute(computeFunc func)
	{
        if(m_compute == nullptr)
        {
            m_compute = new OpenGLCompute(this, m_glQuickItem->rawOGL());
        }
		if (m_compute)
			m_compute->compute(func);
	}
}

#include "renderinterface.h"

#include "qtuserqml/gl/glquickitem.h"

#include "external/kernel/rendercenter.h"
#include "external/kernel/kernel.h"
#include "external/kernel/visualscene.h"
namespace creative_kernel
{
	void registerResidentNode(Qt3DCore::QNode* node)
	{
		getRenderCenter()->registerResidentNode(node);
	}

	void unRegisterResidentNode(Qt3DCore::QNode* node)
	{
		getRenderCenter()->unRegisterResidentNode(node);
	}

	void renderRenderGraph(qtuser_3d::RenderGraph* graph)
	{
		getRenderCenter()->renderRenderGraph(graph);
	}

	bool isRenderRenderGraph(qtuser_3d::RenderGraph* graph)
	{
		return getRenderCenter()->isRenderRenderGraph(graph);
	}

	void registerRenderGraph(qtuser_3d::RenderGraph* graph)
	{
		getRenderCenter()->registerRenderGraph(graph);
	}

	void renderOneFrame()
	{
		getRenderCenter()->renderOneFrame();
	}

	bool isRenderDefaultRenderGraph()
	{
		return isRenderRenderGraph(getKernel()->visScene());
	}

	void renderDefaultRenderGraph()
	{
		renderRenderGraph(getKernel()->visScene());
	}

	void setContinousRender()
	{
		getRenderCenter()->glQuickItem()->setAlways(true);
	}

	void setCommandRender()
	{
		getRenderCenter()->glQuickItem()->setAlways(false);
	}

	void oglCompute(computeFunc func)
	{
		getRenderCenter()->compute(func);
	}
}
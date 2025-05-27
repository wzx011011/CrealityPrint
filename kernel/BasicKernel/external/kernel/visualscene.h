#ifndef CREATIVE_KERNEL_VISUALSCENE_1592188654160_H
#define CREATIVE_KERNEL_VISUALSCENE_1592188654160_H
#include "basickernelexport.h"
#include "qtuser3d/framegraph/rendergraph.h"

namespace qtuser_3d
{
	class Surface;
	class FacePicker;
	class ColorPicker;
	class TextureRenderTarget;
}

namespace creative_kernel
{
	class VisSceneKeyHandler;
	class VisSceneHoverHandler;
	class VisSceneLeftMouseHandler;
	class BASIC_KERNEL_API VisualScene : public qtuser_3d::RenderGraph
	{
		Q_OBJECT
	public:
		VisualScene(Qt3DCore::QNode* parent = nullptr);
		virtual ~VisualScene();

		void show(Qt3DCore::QEntity* entity);
		void hide(Qt3DCore::QEntity* entity);
		void enableHandler(bool enable);

		qtuser_3d::FacePicker* facePicker();
	public slots:
		void updateRender(bool updatePick = false);
	public:
		Qt3DCore::QEntity* sceneGraph() override;
		Qt3DRender::QFrameGraphNode* frameGraph() override;

		void begineRender() override;
		void endRender() override;
		void updateRenderSize(QSize size) override;
	private:
		qtuser_3d::Surface* m_surface;
		qtuser_3d::ColorPicker* m_colorPicker;
		qtuser_3d::TextureRenderTarget* m_textureRenderTarget;

		Qt3DCore::QEntity* m_rootEntity;

		VisSceneKeyHandler* m_keyHandler;
		VisSceneHoverHandler* m_hoverHandler;
		VisSceneLeftMouseHandler* m_leftMouseHandler;

		bool m_handlerEnabled;
	};
}
#endif // CREATIVE_KERNEL_VISUALSCENE_1592188654160_H
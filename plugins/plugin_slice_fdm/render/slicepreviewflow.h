#ifndef _NULLSPACE_SLICEPREVIEWFLOW_1589874729758_H
#define _NULLSPACE_SLICEPREVIEWFLOW_1589874729758_H
#include "qtuser3d/framegraph/rendergraph.h"
#include "qtuser3d/event/eventhandlers.h"
#include <QtCore/QTimer>
#include <QtCore/QSharedPointer>

class SliceAttain;
class SlicePreviewScene;
namespace qtuser_3d
{
	class Surface;
	class ColorPicker;
	class TextureRenderTarget;
}

typedef std::function<void(QImage & image)> requestCallFuncImage;

class SlicePreviewFlow: public qtuser_3d::RenderGraph
	,public qtuser_3d::KeyEventHandler
{
	Q_OBJECT
	Q_PROPERTY(int layers READ layers NOTIFY layersChanged)
	Q_PROPERTY(int steps READ steps NOTIFY stepsChanged)
public:
	SlicePreviewFlow(QObject* parent = nullptr);
	virtual ~SlicePreviewFlow();

	void initialize();
	SlicePreviewScene* scene();

	void requestPreview(requestCallFuncImage func);
	void endRequest();
	void notifyClipValue();

	Q_INVOKABLE void slicePreviewSetOp(QString opname, int param1, int param2 = 0);
	Q_INVOKABLE void slicePreviewCommonOp(QString opname);
	Q_INVOKABLE float slicePreviewGetOp(QString opname);

	bool isAvailable();

	int layers();
	int steps();
	void clear();

	void setSliceAttain(SliceAttain* attain);
	SliceAttain* attain();
	SliceAttain* takeAttain();

	void resetSteps() { m_currentStep = 0; }

	// lisugui 2020-12-24 gcode preview
	Q_INVOKABLE QStringList getCurrentLayerGCodeList();
	Q_INVOKABLE int findViewIndexFromStep(int nStep);
	Q_INVOKABLE int findStepFromViewIndex(int nViewIndex);

	Q_INVOKABLE void setShowColor(int type, bool isShow);

public slots:
	void animateStep();
signals:
	void layersChanged();
	void stepsChanged();
protected:
	Qt3DCore::QEntity* sceneGraph() override;
	Qt3DRender::QFrameGraphNode* frameGraph() override;
	void updateRenderSize(QSize size) override;

	void begineRender() override;
	void endRender() override;

	void onKeyPress(QKeyEvent* event) override;
	void onKeyRelease(QKeyEvent* event) override;
private:
	QScopedPointer<SliceAttain> m_attain;
	SlicePreviewScene* m_scene;
	int m_currentLayer;
	int m_currentStep;

	int m_animationSpeed;
	bool m_animation;

	QTimer* m_animationTimer;
	qtuser_3d::Surface* m_surface;
	qtuser_3d::ColorPicker* m_colorPicker;
	qtuser_3d::TextureRenderTarget* m_textureRenderTarget;

	int m_totalNum;
	int m_currentNum;

	//2020-12-23 lisugui
	QVector<int> m_vecStepRow;

};
#endif // _NULLSPACE_SLICEPREVIEWFLOW_1589874729758_H

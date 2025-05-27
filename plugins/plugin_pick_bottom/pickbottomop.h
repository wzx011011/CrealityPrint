#ifndef _NULLSPACE_PICKBOTTOMOP_1589851340567_H
#define _NULLSPACE_PICKBOTTOMOP_1589851340567_H
#include "qtuser3d/scene/sceneoperatemode.h"
#include "qtuser3d/module/pickableselecttracer.h"

namespace creative_kernel
{
	class ModelN;
}

namespace qtuser_3d
{
	class PureColorEntity;
	class ModelNEntity;
	class SelectEntity;
	class FacePickable;
}

class PickBottomOp: public qtuser_3d::SceneOperateMode
	, public qtuser_3d::SelectorTracer
{
	Q_OBJECT
public:
	PickBottomOp(QObject* parent = nullptr);
	virtual ~PickBottomOp();
	bool getShowPop();

	bool getMessage();
	void setMessage(bool isRemove);
	
protected:
	void onAttach() override;
	void onDettach() override;
	void onLeftMouseButtonClick(QMouseEvent* event) override;
	void onHoverMove(QHoverEvent* event) override;

	void selectChanged(qtuser_3d::Pickable* pickable) override;
	void onSelectionsChanged() override;
	void reGenerateFaces();
	void removeFaces();
protected:
	qtuser_3d::PureColorEntity* m_arrowEntity;

	qtuser_3d::SelectEntity* m_selectFace;
	std::vector<qtuser_3d::FacePickable*> m_pickFaces;
	creative_kernel::ModelN* m_selectModel;
	bool m_bShowPop = false;//by TCJ
	bool m_curstate = true;
	bool m_prestate = true;
signals:
	void mouseLeftClicked();
};
#endif // _NULLSPACE_PICKBOTTOMOP_1589851340567_H

#ifndef _NULLSPACE_TRANSLATEOP_1589770383921_H
#define _NULLSPACE_TRANSLATEOP_1589770383921_H
#include "basickernelexport.h"
#include "qtuser3d/scene/sceneoperatemode.h"
#include "qtuser3d/module/pickableselecttracer.h"

#include <QVector3D>
#include "qtuser3d/entity/translatehelperentity.h"
#include "qtuser3d/math/box3d.h"
#include "data/modeln.h"
#include "kernel/abstractkernel.h"

#define PosMax 1000

namespace creative_kernel
{
	class Kernel;
}

class BASIC_KERNEL_API TranslateOp : public qtuser_3d::SceneOperateMode
	, public qtuser_3d::SelectorTracer
{
	enum class TMode
	{
		null,
		x,
		y,
		z,
		xy,
		yz,
		xz,
		sp

	};

	Q_OBJECT
public:
	TranslateOp(creative_kernel::AbstractKernelUI* pKernel, QObject* parent = nullptr);
	virtual ~TranslateOp();

	void setMessage(bool isRemove);
	bool getMessage();

	void reset();
    void center();
	QVector3D position();
	void setPosition(QVector3D position);
	void setBottom();
	bool getShowPop();
signals:
	void positionChanged();
	void mouseLeftClicked();
protected:
	void onAttach() override;
	void onDettach() override;
	void onLeftMouseButtonPress(QMouseEvent* event) override;
	void onLeftMouseButtonRelease(QMouseEvent* event) override;
	void onLeftMouseButtonMove(QMouseEvent* event) override;
	void onLeftMouseButtonClick(QMouseEvent* event) override;
	void onKeyPress(QKeyEvent* event) override;
	void onKeyRelease(QKeyEvent* event) override;

	void onSelectionsChanged() override;
	void selectChanged(qtuser_3d::Pickable* pickable) override;

	void onMachineSelectChange() override;
protected:
	void setSelectedModel(creative_kernel::ModelN* model);
	void setSelectedModel(QList<creative_kernel::ModelN*> models);

	void buildFromSelections();
	QVector3D process(const QPoint& point);
	void getProperPlane(QVector3D& planeCenter, QVector3D& planeDir, qtuser_3d::Ray& ray);
	QVector3D clip(const QVector3D& delta);

	void updateHelperEntity();

    void movePositionToinit(QList<creative_kernel::ModelN*>& selections);
private:
	qtuser_3d::TranslateHelperEntity* m_helperEntity;

	QVector3D m_spacePoint;

	TMode m_mode;

	QVector3D m_saveLocalPosition;
	QList<QVector3D> m_saveLocalPositions;

	creative_kernel::AbstractKernelUI* m_pKernelUI;

	creative_kernel::ModelN* m_selectedModel;
	QList<creative_kernel::ModelN*> m_selectedModels;
    bool m_bShowPop = false;
	bool moveEnable;
};
#endif // _NULLSPACE_TRANSLATEOP_1589770383921_H

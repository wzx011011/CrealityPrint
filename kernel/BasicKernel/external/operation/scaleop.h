#ifndef _NULLSPACE_SCALEOP_1589770383921_H
#define _NULLSPACE_SCALEOP_1589770383921_H
#include "basickernelexport.h"
#include "qtuser3d/scene/sceneoperatemode.h"
#include "qtuser3d/module/pickableselecttracer.h"

#include <QVector3D>
#include "qtuser3d/entity/translatehelperentity.h"
#include "data/modeln.h"
#include "data/undochange.h"

#include "kernel/abstractkernel.h"
#include "../utils/operationutil.h"

namespace creative_kernel
{
	class Kernel;
}

class BASIC_KERNEL_API ScaleOp : public qtuser_3d::SceneOperateMode
	, public qtuser_3d::SelectorTracer
{
	//enum class TMode
	//{
	//	null,
	//	x,
	//	y,
	//	z,
	//};

	Q_OBJECT
public:
	ScaleOp(creative_kernel::AbstractKernelUI* pKernel, QObject* parent = nullptr);
	virtual ~ScaleOp();

	void setMessage(bool isRemove);
	bool getMessage();

	void reset();
	QVector3D scale();
    QVector3D box();
	QVector3D globalbox();
	void setScale(QVector3D rotate);

	bool uniformCheck();
	void setUniformCheck(bool check);

	bool getShowPop();

signals:
	void scaleChanged();
	void mouseLeftClicked();
protected:
	void onAttach() override;
	void onDettach() override;
	void onLeftMouseButtonPress(QMouseEvent* event) override;
	void onLeftMouseButtonRelease(QMouseEvent* event) override;
	void onLeftMouseButtonMove(QMouseEvent* event) override;
	void onLeftMouseButtonClick(QMouseEvent* event) override;
	void onSelectionsChanged() override;
	void selectChanged(qtuser_3d::Pickable* pickable) override;

	void onMachineSelectChange() override;
protected:
	void setSelectedModel(creative_kernel::ModelN* model);

	void buildFromSelections();
	QVector3D process(const QPoint& point);
	void getProperPlane(QVector3D& planeCenter, QVector3D& planeDir, qtuser_3d::Ray& ray);
	QVector3D getScale(const QVector3D& p);

	void updateHelperEntity();

	void perform(const QPoint& point, bool reversible);
private:
	qtuser_3d::TranslateHelperEntity* m_helperEntity;

	QVector3D m_spacePoint;
	TMode m_mode;

	QVector3D m_saveScale;
	QVector3D m_savePosition;

	creative_kernel::AbstractKernelUI* m_pKernelUI;

	creative_kernel::ModelN* m_selectedModel;
    bool m_bShowPop=false;

	bool m_uniformCheck=true;

	QList<creative_kernel::NUnionChangedStruct> m_changes;
};
#endif // _NULLSPACE_SCALEOP_1589770383921_H

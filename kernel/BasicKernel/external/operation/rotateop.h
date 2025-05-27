#ifndef _NULLSPACE_ROTATEOP_1589770383921_H
#define _NULLSPACE_ROTATEOP_1589770383921_H
#include "basickernelexport.h"
#include "qtuser3d/scene/sceneoperatemode.h"
#include "qtuser3d/module/pickableselecttracer.h"

#include <QVector3D>
#include "qtuser3d/entity/rotatehelperentity.h"
#include "data/modeln.h"
#include "data/undochange.h"
#include "kernel/abstractkernel.h"

class BASIC_KERNEL_API RotateOp : public qtuser_3d::SceneOperateMode
	, public qtuser_3d::SelectorTracer
{
	enum class TMode
	{
		null,
		x,
		y,
		z,
	};

	Q_OBJECT
public:
	RotateOp(creative_kernel::AbstractKernelUI* pKernel, QObject* parent = nullptr);
	virtual ~RotateOp();

	void setMessage(bool isRemove);
	bool getMessage();

	void reset();
	QVector3D rotate();
	void setRotate(QVector3D rotate);
    void startRotate();
	bool getShowPop();
	

signals:
	void rotateChanged();
	void supportMessage();
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
	void setSelectedModel(QList<creative_kernel::ModelN*> models);

	void buildFromSelections();
	QVector3D process(const QPoint& point, creative_kernel::ModelN* model);
	void process(const QPoint& point, QVector3D& axis, float& angle);
	void getProperPlane(QVector3D& planeCenter, QVector3D& planeDir, qtuser_3d::Ray& ray, creative_kernel::ModelN* model);
    void rotateByAxis(QVector3D& axis,float & angle);

	void updateHelperEntity();

	void perform(const QPoint& point, bool reversible, bool needcheck);
private:
	qtuser_3d::RotateHelperEntity* m_helperEntity;

	QVector3D m_spacePoint;
	QList<QVector3D> m_spacePoints;
	TMode m_mode;

	creative_kernel::AbstractKernelUI* m_pKernelUI;

	float m_saveAngle;

	bool m_isRoate;

	QList<creative_kernel::ModelN*> m_selectedModels;
    QVector3D m_displayRotate;
    bool m_bShowPop=false;

	QList<creative_kernel::NUnionChangedStruct> m_changes;
};
#endif // _NULLSPACE_ROTATEOP_1589770383921_H

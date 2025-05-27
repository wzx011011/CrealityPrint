#ifndef _NULLSPACE_SPLITOP_1591235104278_H
#define _NULLSPACE_SPLITOP_1591235104278_H
#include "qtuser3d/scene/sceneoperatemode.h"

#include "qtuser3d/module/pickableselecttracer.h"
#include "qtuser3d/module/manipulatecallback.h"
#include "qtuser3d/math/plane.h"
#include "data/modeln.h"

namespace qtuser_3d
{
	class AlonePointEntity;
	class RotateHelperEntity;
	class PureColorEntity;
}

namespace creative_kernel
{
	class Kernel;
}

class SplitPlane;
class SplitOp: public qtuser_3d::SceneOperateMode
	, public qtuser_3d::SelectorTracer
	, public qtuser_3d::RotateCallback
{
	Q_OBJECT
public:
	SplitOp(QObject* parent = nullptr);
	virtual ~SplitOp();

	void split();
	void splitParts();

	const qtuser_3d::Plane& plane();
	void setPlanePosition(const QVector3D& position);
	void setPlaneNormal(const QVector3D& normal);
	void setPlaneDir(const QVector3D& rotate);

	QVector3D& planeRotateAngle();
	bool getShowPop();
	bool getMessage();
	void setMessage(bool isRemove);

protected:
	void onAttach() override;
	void onDettach() override;
	void onLeftMouseButtonClick(QMouseEvent* event) override;
	void onKeyPress(QKeyEvent* event) override;
	void onHoverMove(QHoverEvent* event) override;

	void setRotateAngle(QVector3D axis, float angle) override;

	void onStartRotate() override;
	void onRotate(QQuaternion q) override;
	void onEndRotate(QQuaternion q) override;

	//void updateHelperEntity();
	void selectChanged(qtuser_3d::Pickable* pickable) override;

	void onSelectionsChanged() override;
	void setSelectedModel(creative_kernel::ModelN* model);

	void updatePlane(bool request);

	
protected:
	qtuser_3d::PureColorEntity* m_debugEntity;
	SplitPlane* m_splitPlane;

	qtuser_3d::Plane m_plane;
	qtuser_3d::RotateHelperEntity* m_helperEntity;

	QVector3D m_saveNormal;
	QVector3D m_saveAngle;

	QVector3D m_rotateAngle;    // rotate euler

	qtuser_3d::Box3D m_box3D;
	bool m_bShowPop = false;
signals:
	void posionChanged();
	void rotateAngleChanged();
	void mouseLeftClicked();
};
#endif // _NULLSPACE_SPLITOP_1591235104278_H

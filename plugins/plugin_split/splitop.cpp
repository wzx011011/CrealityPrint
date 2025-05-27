#include "splitop.h"

#include "splitplane.h"
#include "splitjob.h"
#include "splitpartsjob.h"

#include "qtuser3d/entity/alonepointentity.h"
#include "qtuser3d/entity/rotatehelperentity.h"
#include "qtuser3d/entity/purecolorentity.h"
#include "qtuser3d/utils/primitiveshapecache.h"
#include "qtuser3d/math/primitiveshapeutil.h"

#include "interface/jobsinterface.h"
#include "interface/visualsceneinterface.h"
#include "interface/selectorinterface.h"
#include "interface/camerainterface.h"
#include "interface/eventinterface.h"
#include "interface/spaceinterface.h"
#include "data/modeln.h"

#include "kernel/abstractkernel.h"
#include "data/fdmsupportgroup.h"
#include "interface/spaceinterface.h"

using namespace creative_kernel;
using namespace qtuser_3d;
SplitOp::SplitOp(QObject* parent)
	:SceneOperateMode(parent)
	, m_debugEntity(nullptr)
	, m_rotateAngle(0, 0, 0)
{
	m_debugEntity = new PureColorEntity();
	m_debugEntity->setGeometry(PRIMITIVESHAPE("cylinder"));

	m_splitPlane = new SplitPlane();
	m_splitPlane->setWidthHeight(1000.0f, 1000.0f);

	m_helperEntity = new RotateHelperEntity();

	m_helperEntity->setRotateCallback(this);
}

SplitOp::~SplitOp()
{

}

void SplitOp::onAttach()
{
    QList<ModelN*> selectModels = selectionms();
	tracePickable(m_helperEntity->xPickable());
	tracePickable(m_helperEntity->yPickable());
	tracePickable(m_helperEntity->zPickable());
    if(selectModels.length()>0)
    {
		ModelN* model = selectModels.at(0);
        creative_kernel::selectOne(model);
    }else
	{
        //creative_kernel::selectOne(selectModels.at(0));
    }
	visShow(m_helperEntity);
	visShow(m_splitPlane);

	addLeftMouseEventHandler(m_helperEntity);
	addHoverEventHandler(m_helperEntity);
	addLeftMouseEventHandler(this);
	addHoverEventHandler(this);
	addKeyEventHandler(this);

	//traceSpace(this);
	addSelectTracer(this);
	onSelectionsChanged();

	m_helperEntity->setPickSource(visPickSource());
	m_helperEntity->setScreenCamera(visCamera());
	m_helperEntity->setRotateCallback(this);

	QList<ModelN*> selections = selectionms();
	if (!selections.isEmpty())
	{
		QVector3D position, normal;
		ModelN* ModelNEnd = selections.at(0);
		m_box3D = ModelNEnd->globalSpaceBox();
		m_helperEntity->onBoxChanged(m_box3D);
		m_splitPlane->updateBox(m_box3D);
		m_plane.center = m_box3D.center();
	}
	else
	{
		QList<ModelN*> ModelNs = modelns();
		if (!ModelNs.isEmpty())
		{
			QVector3D position, normal;
			ModelN* ModelNEnd = ModelNs.at(ModelNs.size() - 1);
			m_box3D = ModelNEnd->globalSpaceBox();
			m_helperEntity->onBoxChanged(m_box3D);
			m_splitPlane->updateBox(m_box3D);
			m_plane.center = m_box3D.center();
		}
	}
	updatePlane(true);
	m_bShowPop = true;
}

void SplitOp::onDettach()
{
	visHide(m_helperEntity);
	visHide(m_debugEntity);
	visHide(m_splitPlane);

	removeLeftMouseEventHandler(m_helperEntity);
	removeHoverEventHandler(m_helperEntity);
	removeLeftMouseEventHandler(this);
	removeHoverEventHandler(this);
	removeKeyEventHandler(this);

	//unTraceSpace(this);
	removeSelectorTracer(this);

	unTracePickable(m_helperEntity->xPickable());
	unTracePickable(m_helperEntity->yPickable());
	unTracePickable(m_helperEntity->zPickable());

	m_helperEntity->setPickSource(nullptr);
	m_helperEntity->setScreenCamera(nullptr);
	m_helperEntity->setRotateCallback(nullptr);

	m_bShowPop = false;
}

void SplitOp::setRotateAngle(QVector3D axis, float angle)
{
	for (int i = 0; i < 3; i++)
	{
		if (qAbs(axis[i]) < 0.01)
		{
			axis[i] = 0;
		}
		else
		{
			axis[i] = axis[i] > 0 ? 1 : -1;
		}
	}
	QVector3D v = axis * angle;
	m_rotateAngle = m_saveAngle + v;
	emit rotateAngleChanged();
}

void SplitOp::onStartRotate()
{
	m_saveNormal = m_plane.dir;
	m_saveAngle = m_rotateAngle;
}

void SplitOp::onRotate(QQuaternion q)
{
	QVector3D n = q * m_saveNormal;
	setPlaneNormal(n);
}

void SplitOp::onEndRotate(QQuaternion q)
{
	QVector3D n = q * m_saveNormal;
	setPlaneNormal(n);
}

void SplitOp::setPlaneDir(const QVector3D& rotate)
{
	QList<ModelN*> selections = selectionms();
	m_rotateAngle = rotate;
	m_saveAngle = rotate;

	qDebug() << "setPlaneDir";

	if (selections.size())
	{
		QVector3D dir(0.0f, 0.0f, 1.0f);

		QVector3D axis = QVector3D(1.0f, 0.0f, 0.0f);
		float angle = rotate.x();
		QQuaternion q = QQuaternion::fromAxisAndAngle(axis, angle);
		QVector3D nx = q * dir;
		setPlaneNormal(nx);

        axis = QVector3D(0.0f, 1.0f, 0.0f);
        angle = rotate.y();
		q = QQuaternion::fromAxisAndAngle(axis, angle);
		QVector3D ny = q * m_plane.dir;
		setPlaneNormal(ny);
       
        axis = QVector3D(0.0f, 0.0f, 1.0f);
        angle = rotate.z();
		q = QQuaternion::fromAxisAndAngle(axis, angle);
		QVector3D nz = q * m_plane.dir;
		setPlaneNormal(nz);
   }
}

QVector3D& SplitOp::planeRotateAngle()
{
	return m_rotateAngle;
}

//void SplitOp::updateHelperEntity()
//{
//	QList<ModelN*> selections = selectionms();
//	if (selections.size())
//	{
//		qtuser_3d::Box3D box = selections.at(0)->globalSpaceBox();
//		m_helperEntity->onBoxChanged(box);
//		m_splitPlane->updateBox(box);
//	}
//}

void SplitOp::onSelectionsChanged()
{
	QList<creative_kernel::ModelN*> selections = selectionms();
	setSelectedModel(selections.size() > 0 ? selections.at(0) : nullptr);

	bool flag = (selections.size() == 0) ? false : true;
	AbstractKernelUI::getSelf()->switchShowPop(flag);

	emit mouseLeftClicked();
}

void SplitOp::selectChanged(qtuser_3d::Pickable* pickable)
{
	//updateHelperEntity();
}

void SplitOp::setSelectedModel(creative_kernel::ModelN* model)
{
	if (model)
	{
		visShow(m_helperEntity);
		visShow(m_debugEntity);
		visShow(m_splitPlane);
	}
	else
	{
		visHide(m_helperEntity);
		visHide(m_debugEntity);
		visHide(m_splitPlane);
	}
	requestVisUpdate(true);
}

void SplitOp::onLeftMouseButtonClick(QMouseEvent* event)
{
	QList<creative_kernel::ModelN*> selections = selectionms();
	if (selections.size() < 1)
		return;

	QVector3D position, normal;
	ModelN* model = checkPickModel(event->pos(), position, normal);
	if (model)
	{
		setPlanePosition(position);
		m_splitPlane->updateBox(model->globalSpaceBox());
	}
}

void SplitOp::onKeyPress(QKeyEvent* event)
{
#ifdef _DEBUG
	if (event->key() == Qt::Key_S)
	{
		split();
	}
	else if (event->key() == Qt::Key_P)
	{
		splitParts();
	}
#endif
}

void SplitOp::onHoverMove(QHoverEvent* event)
{
	visHide(m_debugEntity);
	QVector3D scale(1.0f, 1.0f, 1.0f);

	QList<creative_kernel::ModelN*> selections = selectionms();
	if (selections.size() < 1)
		return;

	QVector3D position, normal;
	ModelN* model = checkPickModel(event->pos(), position, normal);
	if (model)
	{
		normal.normalize();
		QMatrix4x4 matrix = layArrowMatrix(position, normal, scale);
		m_debugEntity->setPose(matrix);
		visShow(m_debugEntity);
	}

	requestVisUpdate();
}

void SplitOp::split()
{
	QList<ModelN*> selections = selectionms();

	if (selections.size() > 0 && jobExecutorAvaillable())
	{
		SplitJob* job = new SplitJob();

		job->setModel(selections.at(0));
		job->setPlane(m_plane);
		executeJob(JobPtr(job));
	}
}

void SplitOp::splitParts()
{
	QList<ModelN*> selections = selectionms();

	if (selections.size() == 1 && jobExecutorAvaillable())
	{
		SplitPartsJob* job = new SplitPartsJob();
		
		job->setModel(selections.at(0));
		executeJob(JobPtr(job));
	}
}

const Plane& SplitOp::plane()
{
	return m_plane;
}

void SplitOp::setPlanePosition(const QVector3D& position)
{
	m_plane.center = position;
	Box3D box = m_box3D;
	box += m_plane.center;

	qDebug() << "setPlanePosition";

	m_splitPlane->updateBox(box);
	updatePlane(true);
	
	box.translate(QVector3D(0.0, 0.0, m_plane.center.z() - box.center().z()));
	m_helperEntity->onBoxChanged(box);

	emit posionChanged();
}

void SplitOp::setPlaneNormal(const QVector3D& normal)
{
	m_plane.dir = normal;
	if (normal.length() == 0.0f)
		m_plane.dir = QVector3D(0.0f, 0.0f, 1.0f);
	m_plane.dir.normalize();

	updatePlane(true);
	
}

void SplitOp::updatePlane(bool request)
{
	QMatrix4x4 m;
	QQuaternion q = QQuaternion::fromDirection(m_plane.dir, QVector3D(0.0f, 0.0f, 1.0f));
	m.translate(m_plane.center);
	m.rotate(q);
	m_splitPlane->setPose(m);

	requestVisUpdate(request);
}

bool SplitOp::getShowPop()
{
	return m_bShowPop;
}

bool SplitOp::getMessage()
{
	//if (m_selectedModels.size())
	QList<ModelN*> selections = selectionms();
	for (size_t i = 0; i < selections.size(); i++)
	{
		ModelN* model = selections.at(i);
		if (model->hasFDMSupport())
		{
			FDMSupportGroup* p_support = selections.at(i)->fdmSupport();
			if (p_support->fdmSupportNum())
			{
				return true;
			}
		}

		if (model->hasAttach())
		{
			return true;
		}
	}
	return false;
}

void SplitOp::setMessage(bool isRemove)
{
	if (isRemove)
	{
		QList<ModelN*> selections = selectionms();
		for (size_t i = 0; i < selections.size(); i++)
		{
			ModelN* model = selections.at(i);
			if (model->hasFDMSupport())
			{
				FDMSupportGroup* p_support = selections.at(i)->fdmSupport();
				p_support->clearSupports();
			}

			// Çå¿Õ¸½ÊôÄ£ÐÍ
			if (model->hasAttach())
			{
				selections.at(i)->clearAttachModel();
			}
		}
		requestVisUpdate(true);
	}
}
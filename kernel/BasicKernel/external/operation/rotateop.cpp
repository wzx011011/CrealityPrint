#include "rotateop.h"

#include "qtuser3d/math/space3d.h"
#include "qtuser3d/math/angles.h"

#include "interface/visualsceneinterface.h"
#include "interface/selectorinterface.h"
#include "interface/camerainterface.h"
#include "interface/modelinterface.h"
#include "interface/eventinterface.h"
#include <QtCore/qmath.h>
#include <QQmlProperty>
#include "data/fdmsupportgroup.h"
#include "interface/spaceinterface.h"

using namespace creative_kernel;
RotateOp::RotateOp(creative_kernel::AbstractKernelUI* pKernel, QObject* parent)
	:SceneOperateMode(parent)
	, m_mode(TMode::null)
	, m_pKernelUI(pKernel)
{
	m_helperEntity = new qtuser_3d::RotateHelperEntity();
	m_isRoate = false;
}

RotateOp::~RotateOp()
{
}

void RotateOp::setMessage(bool isRemove)
{
	if (isRemove)
	{
		for (size_t i = 0; i < m_selectedModels.size(); i++)
		{
			ModelN* model = m_selectedModels.at(i);
			if (model->hasFDMSupport())
			{
				FDMSupportGroup* p_support = m_selectedModels.at(i)->fdmSupport();
				p_support->clearSupports();
			}
			
			// ��ո���ģ��
			if (model->hasAttach())
			{
				m_selectedModels.at(i)->clearAttachModel();
			}
		}
		requestVisUpdate(true);
	}
}

bool RotateOp::getMessage()
{
	//if (m_selectedModels.size())
	for (size_t i = 0; i < m_selectedModels.size(); i++)
	{
		ModelN* model = m_selectedModels.at(i);
		if (model->hasFDMSupport())
		{
			FDMSupportGroup* p_support = m_selectedModels.at(i)->fdmSupport();
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

void RotateOp::onAttach()
{
    QList<ModelN*> selectModels = selectionms();
	tracePickable(m_helperEntity->xPickable(), false);
	tracePickable(m_helperEntity->yPickable(), false);
	tracePickable(m_helperEntity->zPickable(), false);

	for (int i = 0; i < selectModels.size(); i++)
	{
		creative_kernel::appendSelect(selectModels.at(i));
	}

    //if(selectModels.length()>0)
    //{
    //    creative_kernel::selectOne(selectModels.at(0));
    //}

	addSelectTracer(this);
	onSelectionsChanged();
	addLeftMouseEventHandler(this);

	if (m_pKernelUI != nullptr)
	{
		m_pKernelUI->switchPopupMode();
	}
	m_bShowPop = true;
    //set left tool bar pop to autoclose status
//   QMetaObject::invokeMethod(getKernelUI()->leftToolbar(), "switchPopupMode",Q_ARG(QVariant, QVariant::fromValue(true)));
}

void RotateOp::onDettach()
{
	unTracePickable(m_helperEntity->xPickable(), false);
	unTracePickable(m_helperEntity->yPickable(), false);
	unTracePickable(m_helperEntity->zPickable(), false);

	visHide(m_helperEntity);
	//setSelectedModel(nullptr);
	m_selectedModels.clear();

	removeSelectorTracer(this);
	requestVisUpdate(true);

	removeLeftMouseEventHandler(this);

	if (m_pKernelUI != nullptr)
	{
		m_pKernelUI->switchPopupMode();
	}
	m_bShowPop = false;
    //restore left tool bar status
 //   QMetaObject::invokeMethod(getKernelUI()->leftToolbar(), "switchPopupMode",Q_ARG(QVariant, QVariant::fromValue(false)));
}

void RotateOp::onLeftMouseButtonPress(QMouseEvent* event)
{
	m_mode = TMode::null;
	m_spacePoints.clear();
	for (size_t i = 0; i < m_selectedModels.size(); i++)
	{
		Pickable* pickable = checkPickable(event->pos(), nullptr);
		if (pickable == m_helperEntity->xPickable()) m_mode = TMode::x;
		if (pickable == m_helperEntity->yPickable()) m_mode = TMode::y;
		if (pickable == m_helperEntity->zPickable()) m_mode = TMode::z;

		m_spacePoints.push_back(process(event->pos(), m_selectedModels[i]));

		m_saveAngle = 0;
		m_isRoate = true;

		m_selectedModels[i]->setNeedCheckScope(0);
	}

	fillChangeStructs(m_changes, true);
}

void RotateOp::perform(const QPoint& point, bool reversible, bool needcheck)
{
	QVector3D axis;
	float angle = 0;
	if (m_selectedModels.size() > 0)
	{
		process(point, axis, angle);
		axis.normalize();
	}
	for (size_t i = 0; i < m_selectedModels.size(); i++)
	{
		QQuaternion oldq = m_selectedModels[i]->localQuaternion();
		QQuaternion q = m_selectedModels[i]->rotateByStandardAngle(axis, angle);

		qtuser_3d::Box3D box = m_selectedModels[i]->globalSpaceBox();
		QVector3D zoffset = QVector3D(0.0f, 0.0f, -box.min.z());

		QVector3D oldPosition = m_selectedModels[i]->localPosition();
		QVector3D newPosition = m_selectedModels[i]->localPosition();

		mixTRModel(m_selectedModels[i], oldPosition, newPosition, oldq, q, reversible, needcheck);

	}

	if (m_selectedModels.size() > 0)
	{
		m_displayRotate = m_selectedModels.back()->localRotateAngle();
	}
	
	requestVisUpdate(true);
}

void RotateOp::onLeftMouseButtonRelease(QMouseEvent* event)
{
	m_isRoate = false;
	if (m_selectedModels.size() && (m_mode != TMode::null))
	{
		perform(event->pos(), false, true);

		for (size_t i = 0; i < m_selectedModels.size(); i++)
		{
			qtuser_3d::Box3D box = m_selectedModels[i]->globalSpaceBox();
			QVector3D zoffset = QVector3D(0.0f, 0.0f, -box.min.z());
			QVector3D oldPosition = m_selectedModels[i]->localPosition();
			QVector3D newPosition = m_selectedModels[i]->localPosition() + zoffset;

			moveModel(m_selectedModels[i], oldPosition, newPosition, false);

			m_selectedModels[i]->setNeedCheckScope(1);
		}
		emit rotateChanged();
		if (m_pKernelUI != nullptr)
		{
			m_pKernelUI->switchPopupMode();
		}
 //       QMetaObject::invokeMethod(getKernelUI()->leftToolbar(), "switchPopupMode",Q_ARG(QVariant, QVariant::fromValue(true)));
        m_bShowPop = false;
	}
	QList<creative_kernel::ModelN*> alls = creative_kernel::modelns();
	for (size_t i = 0; i < alls.size(); i++)
	{
		alls[i]->setNeedCheckScope(1);
	}

	fillChangeStructs(m_changes, false);
	mixUnions(m_changes, true);
	m_changes.clear();
}

void RotateOp::rotateByAxis(QVector3D& axis,float & angle)
{
	for (size_t i = 0; i < m_selectedModels.size(); i++)
	{
		QQuaternion oldQ = m_selectedModels[i]->localQuaternion();
		QQuaternion q = m_selectedModels[i]->rotateByStandardAngle(axis, angle);

		qtuser_3d::Box3D box = m_selectedModels[i]->globalSpaceBox();
		QVector3D zoffset = QVector3D(0.0f, 0.0f, -box.min.z());
		QVector3D localPosition = m_selectedModels[i]->localPosition();
		QVector3D newPosition = localPosition + zoffset;
		//m_selectedModels[i]->setLocalPosition(newPosition);
		mixTRModel(m_selectedModels[i], localPosition, newPosition, oldQ, q, true);
		emit rotateChanged();
	}
}

void RotateOp::onLeftMouseButtonMove(QMouseEvent* event)
{
	if (m_selectedModels.size() && (m_mode != TMode::null))
	{
		perform(event->pos(), true, false);
        emit rotateChanged();
        if(!m_bShowPop)
        {
            //QQmlProperty::write(getKernelUI()->leftToolbar(), "showPop", QVariant::fromValue(true));
            //QMetaObject::invokeMethod(getKernelUI()->leftToolbar(), "switchPopupMode",Q_ARG(QVariant, QVariant::fromValue(false)));
            m_bShowPop=true;
        }
	}
}

void RotateOp::onLeftMouseButtonClick(QMouseEvent* event)
{
	
}

void RotateOp::setSelectedModel(QList<creative_kernel::ModelN*> models)
{
	//trace selected node
	m_selectedModels = models;
    if(m_selectedModels.size())
        m_displayRotate = m_selectedModels.back()->localRotateAngle();
	qDebug() << "setSelectedModel";
	buildFromSelections();
}

void RotateOp::onSelectionsChanged()
{
	QList<creative_kernel::ModelN*> selections = selectionms();
	//setSelectedModel(selections.size() > 0 ? selections.at(0) : nullptr);
	setSelectedModel(selections);
    //lisugui 2021-1-30
//    AbstractKernelUI::getSelf()->refreshPickPanel();

	bool flag = (selections.size() == 0) ? false : true;
	AbstractKernelUI::getSelf()->switchShowPop(flag);

	emit mouseLeftClicked();
}

void RotateOp::selectChanged(qtuser_3d::Pickable* pickable)
{
	for (size_t i = 0; i < m_selectedModels.size(); i++)
	{
		if (pickable == m_selectedModels[i])
			updateHelperEntity();
	}
	if (m_selectedModels.size() > 0)
	{
		m_displayRotate = m_selectedModels.at(0)->localRotateAngle();
		emit rotateChanged();
	}
}

void RotateOp::buildFromSelections()
{
	if (m_selectedModels.size())
	{
		updateHelperEntity();
		visShow(m_helperEntity);
	}
	else
	{
		visHide(m_helperEntity);
	}

	requestVisUpdate(true);
	emit rotateChanged();
}

void RotateOp::reset()
{
	qDebug() << "rotate reset";
	for (size_t i = 0; i < m_selectedModels.size(); i++)
	{
		QQuaternion oldQ = m_selectedModels[i]->localQuaternion();
		QQuaternion q = QQuaternion();
		//m_selectedModels[i]->setLocalQuaternion(q);
		m_selectedModels[i]->resetRotate();

		qtuser_3d::Box3D box = m_selectedModels[i]->globalSpaceBox();
		QVector3D zoffset = QVector3D(0.0f, 0.0f, -box.min.z());

		QVector3D oldPosition = m_selectedModels[i]->localPosition();
		QVector3D newPosition = oldPosition + zoffset;

		mixTRModel(m_selectedModels[i], oldPosition, newPosition, oldQ, q, true);

		//moveModel(m_selectedModels[i], oldPosition, newPosition, true);
	}
    m_displayRotate = QVector3D();
	emit rotateChanged();
}

QVector3D RotateOp::rotate()
{
	if (m_selectedModels.size())
	{

        return m_displayRotate;
	}

	return QVector3D(0.0f, 0.0f, 0.0f);
}

void RotateOp::setRotate(QVector3D rotate)
{
	qDebug() << "rotate setRotate" << rotate;

	if (m_selectedModels.size() == 0)
	{
		return;
	}

	creative_kernel::ModelN* m = m_selectedModels.back();
	QVector3D currentAngles = m->localRotateAngle();
    m_displayRotate = rotate;
	rotate = rotate - currentAngles;
	
	{
		if (qAbs(rotate.x()) > 0.000001)
		{
			QVector3D axis = QVector3D(1.0f, 0.0f, 0.0f);
			float angle = rotate.x();			
			rotateByAxis(axis, angle);
		}
		if (qAbs(rotate.y()) > 0.000001)
		{
			QVector3D axis = QVector3D(0.0f, 1.0f, 0.0f);
			float angle = rotate.y();
			rotateByAxis(axis, angle);
		}
		if (qAbs(rotate.z()) > 0.000001)
		{
			QVector3D axis = QVector3D(0.0f, 0.0f, 1.0f);
			float angle = rotate.z();
			rotateByAxis(axis, angle);
		}

		updateHelperEntity();
		requestVisUpdate(true);
	}
}

void RotateOp::startRotate()
{
}

void RotateOp::process(const QPoint& point, QVector3D& axis, float& angle)
{
	//QVector3D p = process(point);

	axis = QVector3D(0.0f, 0.0f, 1.0f);
	QVector3D planeCenter = QVector3D(0.0f, 0.0f, 0.0f);
	
	//if (m_selectedModels.size())
	for (size_t i = 0; i < m_selectedModels.size(); i++)
	{
		ModelN* model = m_selectedModels.at(i);
		if (model->hasFDMSupport() || model->hasAttach())
		{
			emit supportMessage();
			break;
		}
	}
	
//	for (size_t i = 0; i < m_selectedModels.size(); i++)
	{
		int i = m_selectedModels.size() - 1;
		QVector3D p = process(point, m_selectedModels[i]);
		qtuser_3d::Box3D box = m_selectedModels[i]->globalSpaceBox();
		planeCenter = box.center();

		QVector3D delta;
		QVector3D oc0 = m_spacePoints[i] - planeCenter;
		QVector3D oc1 = p - planeCenter;
		if (oc0.length() != 0.0f && oc1.length() != 0.0f)
		{
			oc0.normalize();
			oc1.normalize();

			if (oc0 == oc1)
			{
				angle = 0.0f;
			}
			else if (oc0 == -oc1)
			{
				angle = 180.0f;
			}
			else
			{
				axis = QVector3D::crossProduct(oc0, oc1);
				axis.normalize();
				for (int j = 0; j < 3; j++)
				{
					if (qAbs(axis[j]) < 0.01)
					{
						axis[j] = 0;
					}
					else
					{
						axis[j] = axis[j] > 0 ? 1 : -1;
					}
				}
				angle = qAcos(QVector3D::dotProduct(oc0, oc1)) * 180.0f / 3.1415926f;
				float baseAngle = m_saveAngle;
				m_saveAngle = angle;
				angle -= baseAngle;
			}
		}
	}
	//angle = 0.0f;
}

QVector3D RotateOp::process(const QPoint& point, creative_kernel::ModelN* model)
{
	qtuser_3d::Ray ray = visRay(point);

	QVector3D planeCenter;
	QVector3D planeDir;
	getProperPlane(planeCenter, planeDir, ray, model);

	QVector3D c;
	qtuser_3d::lineCollidePlane(planeCenter, planeDir, ray, c);
	return c;
}

void RotateOp::getProperPlane(QVector3D& planeCenter, QVector3D& planeDir, qtuser_3d::Ray& ray, creative_kernel::ModelN* model)
{
	planeCenter = QVector3D(0.0f, 0.0f, 0.0f);

	if (model)
	{
		qtuser_3d::Box3D box = model->globalSpaceBox();
		planeCenter = box.center();
	}

	if (m_mode == TMode::x)  // x
	{
		planeDir = QVector3D(1.0f, 0.0f, 0.0f);
	}
	else if (m_mode == TMode::y)
	{
		planeDir = QVector3D(0.0f, 1.0f, 0.0f);
	}
	else if (m_mode == TMode::z)
	{
		planeDir = QVector3D(0.0f, 0.0f, 1.0f);
	}
}

void RotateOp::updateHelperEntity()
{
	if (m_selectedModels.size() && !m_isRoate)
	{
		qtuser_3d::Box3D box = m_selectedModels[m_selectedModels.size() - 1]->globalSpaceBox();
		m_helperEntity->onBoxChanged(box);
	}
}

void RotateOp::onMachineSelectChange()
{
	updateHelperEntity();
}

bool RotateOp::getShowPop()
{
	return m_bShowPop;
}

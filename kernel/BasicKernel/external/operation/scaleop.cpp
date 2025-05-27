#include "scaleop.h"

#include "qtuser3d/math/space3d.h"
#include "qtuser3d/math/angles.h"

#include "interface/visualsceneinterface.h"
#include "interface/selectorinterface.h"
#include "interface/camerainterface.h"
#include "interface/modelinterface.h"
#include "interface/eventinterface.h"
#include <QQmlProperty>

#include "data/fdmsupportgroup.h"
#include "interface/spaceinterface.h"
using namespace creative_kernel;
ScaleOp::ScaleOp(creative_kernel::AbstractKernelUI* pKernel, QObject* parent)
	:SceneOperateMode(parent)
	, m_mode(TMode::null)
	, m_selectedModel(nullptr)
	, m_pKernelUI(pKernel)
{
	m_helperEntity = new qtuser_3d::TranslateHelperEntity(nullptr, 7, 1, QVector3D(1, 1, 1));
}

ScaleOp::~ScaleOp()
{
}

void ScaleOp::setMessage(bool isRemove)
{
	if (isRemove)
	{
		QList<creative_kernel::ModelN*> selections = selectionms();
		foreach(creative_kernel::ModelN * model, selections)
		{
			if (model->hasFDMSupport())
			{
				FDMSupportGroup* p_support = model->fdmSupport();
				p_support->clearSupports();
			}

			// ��ո���ģ��
			if (model->hasAttach())
			{
				model->clearAttachModel();
			}
		}
		requestVisUpdate(true);
	}
}

bool ScaleOp::getMessage()
{
	//if (!m_bShowPop)
	//{
	//	return false;
	//}
	QList<creative_kernel::ModelN*> selections = selectionms();
	foreach(creative_kernel::ModelN * model, selections)
	{
		if (model->hasFDMSupport())
		{
			FDMSupportGroup* p_support = model->fdmSupport();
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

void ScaleOp::onAttach()
{
    QList<ModelN*> selectModels = selectionms();
	tracePickable(m_helperEntity->xPickable(), false);
	tracePickable(m_helperEntity->yPickable(), false);
	tracePickable(m_helperEntity->zPickable(), false);
    if(selectModels.length()>0)
    {
        creative_kernel::selectOne(selectModels.at(0));
    }else{

        //creative_kernel::selectOne(selectModels.at(0));
    }
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

void ScaleOp::onDettach()
{
	unTracePickable(m_helperEntity->xPickable(), false);
	unTracePickable(m_helperEntity->yPickable(), false);
	unTracePickable(m_helperEntity->zPickable(), false);
	
	visHide(m_helperEntity);
	setSelectedModel(nullptr);

	removeSelectorTracer(this);
	requestVisUpdate(true);

	removeLeftMouseEventHandler(this);
	if (m_pKernelUI != nullptr)
	{
		m_pKernelUI->switchPopupMode();
	}
	m_bShowPop = false;
    //restore left tool bar status
//    QMetaObject::invokeMethod(getKernelUI()->leftToolbar(), "switchPopupMode",Q_ARG(QVariant, QVariant::fromValue(false)));
}

void ScaleOp::onLeftMouseButtonPress(QMouseEvent* event)
{
	m_mode = TMode::null;

	if (m_selectedModel)
	{
		Pickable* pickable = checkPickable(event->pos(), nullptr);
		if (pickable == m_helperEntity->xPickable()) m_mode = TMode::x;
		if (pickable == m_helperEntity->yPickable()) m_mode = TMode::y;
		if (pickable == m_helperEntity->zPickable()) m_mode = TMode::z;

		m_saveScale = m_selectedModel->localScale();
		m_savePosition = m_selectedModel->localPosition();
		m_spacePoint = operationProcessCoord(event->pos(), m_selectedModel, 0, m_mode);


		m_selectedModel->setNeedCheckScope(0);
	}

	if(m_mode == TMode::x 
		|| m_mode == TMode::y
		|| m_mode == TMode::z)
		fillChangeStructs(m_changes, true);
}

void ScaleOp::perform(const QPoint& point, bool reversible)
{
	QVector3D p = operationProcessCoord(point, m_selectedModel, 0, m_mode);
	QVector3D delta = getScale(p);
	QVector3D newScale = m_saveScale * delta;

	//lisugui 2021-05-27 
	QList<creative_kernel::ModelN*> selections = selectionms();
	for(creative_kernel::ModelN * model : selections)
	{
		model->setLocalScale(newScale);

		qtuser_3d::Box3D box = model->globalSpaceBox();
		QVector3D zoffset = QVector3D(0.0f, 0.0f, 0.0f);  // QVector3D(0.0f, 0.0f, -box.min.z());
		QVector3D localPosition = model->localPosition();
		QVector3D newPosition = localPosition + zoffset;

		mixTSModel(model, m_savePosition, newPosition, m_saveScale, newScale, reversible);
	}
}

void ScaleOp::onLeftMouseButtonRelease(QMouseEvent* event)
{
	if (m_selectedModel && (m_mode != TMode::null))
	{
		perform(event->pos(), false);

		//lisugui 2021-05-27 
		QList<creative_kernel::ModelN*> selections = selectionms();
		foreach(creative_kernel::ModelN * model, selections)
		{
			qtuser_3d::Box3D box = model->globalSpaceBox();
			QVector3D zoffset = QVector3D(0.0f, 0.0f, -box.min.z());
			QVector3D oldPosition = model->localPosition();
			QVector3D newPosition = model->localPosition() + zoffset;
			moveModel(model, oldPosition, newPosition, false);

			m_selectedModel->setNeedCheckScope(1);
		}

		emit scaleChanged();
		if (m_pKernelUI != nullptr)
		{
			m_pKernelUI->switchPopupMode();
		}

        m_bShowPop = false;
	}

	QList<creative_kernel::ModelN*> alls = creative_kernel::modelns();
	for (size_t i = 0; i < alls.size(); i++)
	{
		alls[i]->setNeedCheckScope(1);
	}

	if (m_mode == TMode::x
		|| m_mode == TMode::y
		|| m_mode == TMode::z)
	{
		fillChangeStructs(m_changes, false);
		mixUnions(m_changes, true);
		m_changes.clear();
	}
}

void ScaleOp::onLeftMouseButtonMove(QMouseEvent* event)
{
	if (m_selectedModel && (m_mode != TMode::null))
	{
		perform(event->pos(), false);
        emit scaleChanged();
        if(!m_bShowPop)
        {
			if (m_pKernelUI != nullptr)
			{
				m_pKernelUI->switchPopupMode();
			}
            //QQmlProperty::write(getKernelUI()->leftToolbar(), "showPop", QVariant::fromValue(true));
            //QMetaObject::invokeMethod(getKernelUI()->leftToolbar(), "switchPopupMode",Q_ARG(QVariant, QVariant::fromValue(false)));
            m_bShowPop=true;
        }
	}
}

void ScaleOp::onLeftMouseButtonClick(QMouseEvent* event)
{
	
}

void ScaleOp::onSelectionsChanged()
{
	QList<creative_kernel::ModelN*> selections = selectionms();
	setSelectedModel(selections.size() > 0 ? selections.at(0) : nullptr);
    //lisugui 2021-1-30
//    AbstractKernelUI::getSelf()->refreshPickPanel();

	bool flag = (selections.size() == 0) ? false : true;
	AbstractKernelUI::getSelf()->switchShowPop(flag);

	emit mouseLeftClicked();
}

void ScaleOp::selectChanged(qtuser_3d::Pickable* pickable)
{

	if (pickable == m_selectedModel)
	{
		updateHelperEntity();
		emit scaleChanged();
	}
}

void ScaleOp::setSelectedModel(creative_kernel::ModelN* model)
{
	//trace selected node
	m_selectedModel = model;

	buildFromSelections();
}

void ScaleOp::buildFromSelections()
{
	if (m_selectedModel)
	{
		updateHelperEntity();
		visShow(m_helperEntity);
	}
	else
	{
		visHide(m_helperEntity);
	}

	requestVisUpdate(true);
	emit scaleChanged();
}

void ScaleOp::reset()
{
	QList<creative_kernel::ModelN*> selections = selectionms();
	foreach(creative_kernel::ModelN * model, selections)
	{
		QVector3D oldLocalScale = model->localScale();
		QVector3D newLocalScale = QVector3D(1.0f, 1.0f, 1.0f);

		model->setLocalScale(newLocalScale);

		qtuser_3d::Box3D box = model->globalSpaceBox();
		QVector3D zoffset = QVector3D(0.0f, 0.0f, -box.min.z());
		QVector3D oldLocalPosition = model->localPosition();
		QVector3D newLocalPosition = oldLocalPosition + zoffset;

		mixTSModel(model, oldLocalPosition, newLocalPosition, oldLocalScale, newLocalScale, true);

		//emit scaleChanged();
	}
}

QVector3D ScaleOp::scale()
{
	if (m_selectedModel)
	{
        return m_selectedModel->localScale();
	}

    return QVector3D(0.0f, 0.0f, 0.0f);
}

QVector3D ScaleOp::box()
{
    if (m_selectedModel)
    {
         qtuser_3d::Box3D box3d = m_selectedModel->localBox();
         return box3d.size();
    }

    return QVector3D(0.0f, 0.0f, 0.0f);
}

QVector3D ScaleOp::globalbox()
{
	if (m_selectedModel)
	{
		qtuser_3d::Box3D box3d = m_selectedModel->globalSpaceBox();
		return box3d.size();
	}

	return QVector3D(0.0f, 0.0f, 0.0f);
}

void ScaleOp::setScale(QVector3D scale)
{
    //if (scale.x() <= 0.01f)
    //{
    //    scale.setX(0.01f);
    //}
    //else if (scale.y() <= 0.01f)
    //{
    //    scale.setY(0.01f);
    //}
    //else if (scale.z() <= 0.01f)
    //{
    //    scale.setZ(0.01f);
    //}

	//lisugui 2021-05-27 
	QList<creative_kernel::ModelN*> selections = selectionms();
	foreach(creative_kernel::ModelN * model ,selections)
	{
		QVector3D oldLocalScale = model->localScale();
		QVector3D newLocalScale = scale;

		model->setLocalScale(newLocalScale);

		qtuser_3d::Box3D box = model->boxWithSup();
		QVector3D zoffset = QVector3D(0.0f, 0.0f, -box.min.z());
		QVector3D oldLocalPosition = model->localPosition();
		QVector3D newLocalPosition = oldLocalPosition + zoffset;

		mixTSModel(model, oldLocalPosition, newLocalPosition, oldLocalScale, newLocalScale, true);

		//emit scaleChanged();
	}
	creative_kernel::checkModelRange();
	creative_kernel::checkBedRange();
}

bool ScaleOp::uniformCheck()
{
	return m_uniformCheck;
}
void ScaleOp::setUniformCheck(bool check)
{
	m_uniformCheck = check;
}

QVector3D ScaleOp::process(const QPoint& point)
{
	qtuser_3d::Ray ray = visRay(point);

	QVector3D planeCenter;
	QVector3D planeDir;
	getProperPlane(planeCenter, planeDir, ray);

	QVector3D c;
	qtuser_3d::lineCollidePlane(planeCenter, planeDir, ray, c);
	return c;
}

void ScaleOp::getProperPlane(QVector3D& planeCenter, QVector3D& planeDir, qtuser_3d::Ray& ray)
{
	planeCenter = QVector3D(0.0f, 0.0f, 0.0f);
	qtuser_3d::Box3D box = m_selectedModel->globalSpaceBox();
	planeCenter = box.center();

	QList<QVector3D> dirs;
	if (m_mode == TMode::x)  // x
	{
		dirs << QVector3D(0.0f, 1.0f, 0.0f);
		dirs << QVector3D(0.0f, 0.0f, 1.0f);
	}
	else if (m_mode == TMode::y)
	{
		dirs << QVector3D(1.0f, 0.0f, 0.0f);
		dirs << QVector3D(0.0f, 0.0f, 1.0f);
	}
	else if (m_mode == TMode::z)
	{
		dirs << QVector3D(1.0f, 0.0f, 0.0f);
		dirs << QVector3D(0.0f, 1.0f, 0.0f);
	}

	float d = -1.0f;
	for (QVector3D& n : dirs)
	{
		float dd = QVector3D::dotProduct(n, ray.dir);
		if (qAbs(dd) > d)
		{
			planeDir = n;
		}
	}
}

QVector3D ScaleOp::getScale(const QVector3D& c)
{
	QVector3D scaleDelta = QVector3D(1.0f, 1.0f, 1.0f);
	bool uniformCheck = m_uniformCheck;

	auto f = [&scaleDelta, uniformCheck](float d1, float d2, int pos) {
		if (abs(d1) > 0.0f)
		{
			float s = abs(d2) / abs(d1);
			if (uniformCheck)
			{
				scaleDelta = QVector3D(s, s, s);
			}
			else
			{
				scaleDelta[pos] = s;
			}
		}
	};

	if (m_mode == TMode::x)  // x
	{
		f(m_spacePoint.x(), c.x(), 0);
	}
	else if (m_mode == TMode::y)
	{
		f(m_spacePoint.y(), c.y(), 1);
	}
	else if (m_mode == TMode::z)
	{
		f(m_spacePoint.z(), c.z(), 2);
	}

	return scaleDelta;
}

void ScaleOp::updateHelperEntity()
{
	if (m_selectedModel)
	{
		qtuser_3d::Box3D box = m_selectedModel->globalSpaceBox();

		m_helperEntity->setDirColor(QVector4D(1.0f, 0.0f, 0.0f, 0.8f), 0);
		m_helperEntity->setDirColor(QVector4D(0.0f, 1.0f, 0.0f, 0.8f), 1);
		m_helperEntity->setDirColor(QVector4D(0.0f, 0.0f, 1.0f, 0.8f), 2);
		m_helperEntity->updateBox(box);
	}
}

void ScaleOp::onMachineSelectChange()
{
	updateHelperEntity();
}

bool ScaleOp::getShowPop()
{
	return m_bShowPop;
}


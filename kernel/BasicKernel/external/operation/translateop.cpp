#include "translateop.h"
#include <QQmlProperty>
#include "qtuser3d/math/space3d.h"
#include <QMetaObject>
#include "interface/visualsceneinterface.h"
#include "interface/selectorinterface.h"
#include "interface/camerainterface.h"
#include "interface/modelinterface.h"
#include "interface/spaceinterface.h"
#include "interface/eventinterface.h"
#include "us/slicermanager.h"

#include "data/fdmsupportgroup.h"
using namespace creative_kernel;
TranslateOp::TranslateOp(creative_kernel::AbstractKernelUI* pKernel, QObject* parent)
	:SceneOperateMode(parent)
	, m_mode(TMode::null)
	, m_selectedModel(nullptr)
	, m_pKernelUI(pKernel)
{
	m_helperEntity = new qtuser_3d::TranslateHelperEntity();
	moveEnable = false;
}

TranslateOp::~TranslateOp()
{
}

void TranslateOp::setMessage(bool isRemove)
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

			// 清空附属模型
			if (model->hasAttach())
			{
				m_selectedModels.at(i)->clearAttachModel();
			}
		}
		requestVisUpdate(true);
	}
}

bool TranslateOp::getMessage()
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

void TranslateOp::onAttach()
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
	addKeyEventHandler(this);

	if (m_pKernelUI != nullptr)
	{
		m_pKernelUI->switchPopupMode();
	}
	m_bShowPop = true;
     //set left tool bar pop to autoclose status
//    QMetaObject::invokeMethod(getKernelUI()->leftToolbar(), "switchPopupMode",Q_ARG(QVariant, QVariant::fromValue(true)));

}

void TranslateOp::onDettach()
{
	unTracePickable(m_helperEntity->xPickable(), false);
	unTracePickable(m_helperEntity->yPickable(), false);
	unTracePickable(m_helperEntity->zPickable(), false);
	
	visHide(m_helperEntity);
	//setSelectedModel(nullptr);
	m_selectedModels.clear();
	removeSelectorTracer(this);

	removeLeftMouseEventHandler(this);
	removeKeyEventHandler(this);

	requestVisUpdate(true);

	if (m_pKernelUI != nullptr)
	{
		m_pKernelUI->switchPopupMode();
	}
	m_bShowPop = false;
    //restore left tool bar status
    //QMetaObject::invokeMethod(getKernelUI()->leftToolbar(), "switchPopupMode",Q_ARG(QVariant, QVariant::fromValue(false)));
}

void TranslateOp::onLeftMouseButtonPress(QMouseEvent* event)
{
	moveEnable = false;
	m_mode = TMode::null;
	m_saveLocalPositions.clear();
	for (size_t i = 0; i < m_selectedModels.size(); i++)
	{
		Pickable* pickable = checkPickable(event->pos(), nullptr);

		

		if (pickable == m_helperEntity->xPickable()) m_mode = TMode::x;
		else if (pickable == m_helperEntity->yPickable()) m_mode = TMode::y;
		else if (pickable == m_helperEntity->zPickable()) m_mode = TMode::z;
		else if (pickable != nullptr) m_mode = TMode::sp;
		ModelN* model = qobject_cast<ModelN*>(pickable);
		//if (model == m_selectedModels[0])
		//{
		//	m_mode = TMode::sp;
		//}
		if (pickable == m_selectedModels[i] || m_mode == TMode::x || m_mode == TMode::y || m_mode == TMode::z)
		{
			moveEnable = true;
		}


		m_spacePoint = process(event->pos());
		m_saveLocalPositions.push_back(m_selectedModels[i]->localPosition());
	}

	//if (m_selectedModel)
	//{
	//	Pickable* pickable = checkPickable(event->pos(), nullptr);
	//	if (pickable == m_helperEntity->xPickable()) m_mode = TMode::x;
	//	if (pickable == m_helperEntity->yPickable()) m_mode = TMode::y;
	//	if (pickable == m_helperEntity->zPickable()) m_mode = TMode::z;
	//	
	//	ModelN* model = qobject_cast<ModelN*>(pickable);
	//	if (model == m_selectedModel)
	//	{
	//		m_mode = TMode::sp;
	//	}

	//	m_spacePoint = process(event->pos());
	//	m_saveLocalPosition = m_selectedModel->localPosition();
	//}
}

void TranslateOp::onLeftMouseButtonRelease(QMouseEvent* event)
{
	if (!moveEnable)
	{
		return;
	}

	for (size_t i = 0; i < m_selectedModels.size(); i++)
	{
		if (m_mode != TMode::null && !SlicerManager::instance().getDoingSlice())
		{	//
			QVector3D p = process(event->pos());
			QVector3D delta = clip(p - m_spacePoint);
			QVector3D newLocalPosition = m_saveLocalPositions[i] + delta;
			moveModel(m_selectedModels[i], m_saveLocalPositions[i], newLocalPosition, true);
			requestVisUpdate(true);
			emit positionChanged();
			if (m_pKernelUI != nullptr)
			{
				m_pKernelUI->switchPopupMode();
			}
//            QMetaObject::invokeMethod(getKernelUI()->leftToolbar(), "switchPopupMode",Q_ARG(QVariant, QVariant::fromValue(true)));
            //QQmlProperty::write(getKernelUI()->leftToolbar(), "showPop", QVariant::fromValue(false));
            m_bShowPop = false;
		}
		//m_saveLocalPositions[i] = m_selectedModels[i]->localPosition();
	}
	
	/*if (m_mode == TMode::null)
	{
		AbstractKernelUI::getSelf()->switchShowPop(false);	
	}*/

	//if (m_selectedModel && (m_mode != TMode::null))
	//{	//
	//	QVector3D p = process(event->pos());
	//	QVector3D delta = clip(p - m_spacePoint);
	//	QVector3D newLocalPosition = m_saveLocalPosition + delta;

	//	moveModel(m_selectedModel, m_saveLocalPosition, newLocalPosition, true);

	//	requestVisUpdate(true);
	//	emit positionChanged();
	//}
}

void TranslateOp::onLeftMouseButtonMove(QMouseEvent* event)
{
	if (!moveEnable)
	{
		return;
	}

	for (size_t i = 0; i < m_selectedModels.size(); i++)
	{
		if (m_mode != TMode::null && !SlicerManager::instance().getDoingSlice())
		{
			QVector3D p = process(event->pos());
			QVector3D delta = clip(p - m_spacePoint);

			moveModel(m_selectedModels[i], m_saveLocalPositions[i], m_saveLocalPositions[i] + delta);

			requestVisUpdate(true);

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
    emit positionChanged();
	//if (m_selectedModel && (m_mode != TMode::null))
	//{
	//	QVector3D p = process(event->pos());
	//	QVector3D delta = clip(p - m_spacePoint);
	//	
	//	moveModel(m_selectedModel, m_saveLocalPosition, m_saveLocalPosition + delta);

	//	requestVisUpdate(true);
	//	emit positionChanged();
	//}
}

void TranslateOp::onLeftMouseButtonClick(QMouseEvent* event)
{
	/*QList<creative_kernel::ModelN*> selections = selectionms();
	if (selections.size() == 0)
	{
		AbstractKernelUI::getSelf()->switchShowPop(false);		
	}
	else
	{
		AbstractKernelUI::getSelf()->switchShowPop(true);
	}*/
	
}

void TranslateOp::onKeyPress(QKeyEvent* event)
{
	QList<creative_kernel::ModelN*> selections = selectionms();
	if (selections.size() == 0)
		return;

	QVector3D delta;
	bool needUpdate = false;

	//���û�����-->��ʱ
	if (event->key() == Qt::Key_Right)
	{
		delta = QVector3D(1.0f, 0.0f, 0.0f);
		needUpdate = true;
	}
	//���û�����<--��ʱ
	else if (event->key() == Qt::Key_Left)
	{
		delta = QVector3D(-1.0f, 0.0f, 0.0f);
		needUpdate = true;
	}
	//���û�����up��ʱ
	else if (event->key() == Qt::Key_Up)
	{
		delta = QVector3D(0.0f, 1.0f, 0.0f);
		needUpdate = true;
	}
	//���û�����Down��ʱ
	else if (event->key() == Qt::Key_Down)
	{
		delta = QVector3D(0.0f, -1.0f, 0.0f);
		needUpdate = true;
	}

	if (SlicerManager::instance().getDoingSlice())
	{
		needUpdate = false;
	}


	if (needUpdate)
	{
		for (size_t i = 0; i < selections.size(); i++)
		{
			QVector3D localPosition = selections.at(i)->localPosition();
			moveModel(selections.at(i), localPosition, localPosition + delta);
		}
		requestVisUpdate(true);
		emit positionChanged();
	}
}

void TranslateOp::onKeyRelease(QKeyEvent* event)
{
}

void TranslateOp::onSelectionsChanged()
{
	QList<creative_kernel::ModelN*> selections = selectionms();
	bool flag = (selections.size() == 0) ? false : true;
	AbstractKernelUI::getSelf()->switchShowPop(flag);
	setSelectedModel(selections);
	emit mouseLeftClicked();
}

void TranslateOp::selectChanged(qtuser_3d::Pickable* pickable)
{
	for (size_t i = 0; i < m_selectedModels.size(); i++)
	{
		if (pickable == m_selectedModels[i])
			updateHelperEntity();
	}
	if (m_selectedModels.size() > 0)
	{
		emit positionChanged();
	}
	//if (pickable == m_selectedModel)
	//	updateHelperEntity();
}

void TranslateOp::setSelectedModel(creative_kernel::ModelN* model)
{
	//trace selected node
	m_selectedModel = model;

#if 0
	TranslateMode* mode = qobject_cast<TranslateMode*>(parent());
	if (!m_selectedModel)
	{
		mode->setSource("qrc:/kernel/qml/NoSelect.qml");
	}
	else {
		mode->setSource("qrc:/kernel/qml/MovePanel.qml");
	}

	qtuser_qml::ToolCommandCenter* left = getUILCenter();
	left->changeCommand(mode);
#endif
	buildFromSelections();
}

void TranslateOp::setSelectedModel(QList<creative_kernel::ModelN*> models)
{
	m_selectedModels = models;
	m_saveLocalPositions.clear();
	for (size_t i = 0; i < m_selectedModels.size(); i++)
	{
		m_saveLocalPositions.push_back(m_selectedModels[i]->localPosition());

	}
	buildFromSelections();
}

void TranslateOp::buildFromSelections()
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

	//if (m_selectedModel)
	//{
	//	updateHelperEntity();
	//	visShow(m_helperEntity);
	//}
	//else
	//{
	//	visHide(m_helperEntity);
	//}

	requestVisUpdate(true);
	if (m_selectedModels.size())
	{
		emit positionChanged();
	}
		
}
void TranslateOp::center()
{
	qtuser_3d::Box3D modelsbox;
	for (size_t i = 0; i < m_selectedModels.size(); i++)
	{
		if (m_selectedModels[i])
		{
			qtuser_3d::Box3D modelbox = m_selectedModels[i]->globalSpaceBox();
			modelsbox += modelbox;
		}
	}
	QVector3D movePos;
	float newZ = 0;
	if (modelsbox.valid)
	{
		qtuser_3d::Box3D box = creative_kernel::baseBoundingBox();
		QVector3D size = box.size();
		QVector3D center = box.center();
		newZ = center.z() - size.z() / 2.0f;

		movePos = box.center() - modelsbox.center();
		movePos.setZ(0);
	}

	for (size_t i = 0; i < m_selectedModels.size(); i++)
	{
		if (m_selectedModels[i])
		{
			QVector3D oldLocalPosition = m_selectedModels[i]->localPosition();
			QVector3D newPositon = oldLocalPosition + movePos;
			//newPositon.setZ(newZ);

			moveModel(m_selectedModels[i], oldLocalPosition, newPositon/*m_selectedModels[i]->mapGlobal2Local(newPositon)*/, true);

			updateHelperEntity();
			requestVisUpdate(true);
		}
	}

	emit positionChanged();

  //  if(m_selectedModel)
  //  {
  //      QVector3D oldLocalPosition = m_selectedModel->localPosition();
  //      qtuser_3d::Box3D box = creative_kernel::baseBoundingBox();
  //      QVector3D size = box.size();
  //      QVector3D center = box.center();
		//center.setZ(center.z() - size.z() / 2.0f);

  //      moveModel(m_selectedModel, oldLocalPosition, m_selectedModel->mapGlobal2Local(center), true);

  //      updateHelperEntity();
		//requestVisUpdate(true);

  //      emit positionChanged();
  //  }
}

void TranslateOp::movePositionToinit(QList<creative_kernel::ModelN*>& selections)
{
    for (size_t i = 0; i < selections.size(); i++)
    {
        QVector3D oldLocalPosition = selections.at(i)->localPosition();
        QVector3D initPosition = selections.at(i)->GetInitPosition();
        initPosition.setZ(0.0f);
        QVector3D position = selections.at(i)->mapGlobal2Local(initPosition);
        position.setX(initPosition.x());
        position.setY(initPosition.y());

        moveModel(selections.at(i), oldLocalPosition, position, true);
    }
}

void TranslateOp::reset()
{
	QList<ModelN*> selections = selectionms();
	if(selections.size())
	{
		//for (size_t i = 0; i < selections.size(); i++)
		//{
		//	QVector3D oldLocalPosition = selections.at(i)->localPosition();
		//	QVector3D initPosition = selections.at(i)->GetInitPosition();
  //          QVector3D position = selections.at(i)->mapGlobal2Local(initPosition);
  //          position.setX(initPosition.x());
  //          position.setY(initPosition.y());

		//	moveModel(selections.at(i), oldLocalPosition, position, true);
		//}

        movePositionToinit(selections);
	}
	else
	{
		ModelSpace* space = getModelSpace();
		QList<ModelN*> models = space->modelns();
		//for (size_t i = 0; i < models.size(); i++)
		//{
		//	QVector3D oldLocalPosition = models.at(i)->localPosition();
		//	QVector3D initPosition = models.at(i)->GetInitPosition();
		//	initPosition.setZ(0.0f);

		//	moveModel(models.at(i), oldLocalPosition, models.at(i)->mapGlobal2Local(initPosition), true);
		//}

        movePositionToinit(models);
	}
	updateHelperEntity();
	requestVisUpdate(true);
	emit positionChanged();
}

QVector3D TranslateOp::position()
{
	//qtuser_3d::Box3D box;
	//for (size_t i = 0; i < m_selectedModels.size(); i++)
	//{
	//	box += m_selectedModels[i]->globalSpaceBox();
	//}
	//if (m_selectedModels.size())
	//{
	//	QVector3D size = box.size();
	//	QVector3D center = box.center();
	//	center.setZ(center.z() - size.z() / 2.0f);
	//	return center;
	//}
	if (m_selectedModels.size() > 0)
	{
        //return m_selectedModels[m_selectedModels.size() - 1]->localPosition();

		//10-22 lisugui ��ԭע��,ԭ����������ķ���������ֵ�������֮�󣬳�ʼZ ����Ϊ0
		qtuser_3d::Box3D box = m_selectedModels[m_selectedModels.size() - 1]->globalSpaceBox();
		QVector3D size = box.size();
		QVector3D center = box.center();
		center.setZ(center.z() - size.z() / 2.0f);
		//end
		return center;
	}
    return QVector3D(0.0f, 0.0f, 0.0f);
}

void TranslateOp::setPosition(QVector3D position)
{
	if (1 == m_selectedModels.size())
	{
		moveModel(m_selectedModels[0], m_selectedModels[0]->localPosition()/*QVector3D()*/, m_selectedModels[0]->mapGlobal2Local(position), true);
		updateHelperEntity();
		requestVisUpdate(true);
		emit positionChanged();
	}
	else
	{
        qDebug()<<"position =" <<position;


		for (size_t i = 0; i < m_selectedModels.size(); i++)
		{
            QVector3D moveOffset = position - m_selectedModels[m_selectedModels.size() - 1]->localPosition();
            qDebug()<<"moveOffset =" <<moveOffset;
            moveModel(m_selectedModels[i], m_selectedModels[i]->localPosition(),m_selectedModels[i]->localPosition() + moveOffset, true);
		}
		if (m_selectedModels.size())
		{
			updateHelperEntity();
			requestVisUpdate(true);
			emit positionChanged();
		}
	}
}

void TranslateOp::setBottom()
{
    for (size_t i = 0; i < m_selectedModels.size(); i++)
    {
        qtuser_3d::Box3D box = m_selectedModels[i]->globalSpaceBox();

		float fOffset = -box.min.z();
		if (m_selectedModels[i]->hasFDMSupport())
		{
			//DLPģʽ�� �õײ�������֧��ʱ������֧�ų�ʼλ�õ�ƫ����
			fOffset += m_selectedModels[i]->getSupportLiftHeight();
		}

        QVector3D zoffset = QVector3D(0.0f, 0.0f, fOffset);
        QVector3D localPosition = m_selectedModels[i]->localPosition();

        moveModel(m_selectedModels[i], localPosition, localPosition + zoffset, true);
    }
    if(m_selectedModels.size())
    {
        updateHelperEntity();
        requestVisUpdate(true);
        emit positionChanged();
    }
//	if (m_selectedModels.size())
//	{
//		qtuser_3d::Box3D box = m_selectedModels[m_selectedModels.size() - 1]->globalSpaceBox();
//        QVector3D zoffset = QVector3D(0.0f, 0.0f, -box.min.z());
//        QVector3D localPosition = m_selectedModels[m_selectedModels.size() - 1]->localPosition();

//        moveModel(m_selectedModels[m_selectedModels.size() - 1], localPosition, localPosition + zoffset, true);

//        updateHelperEntity();
//		requestVisUpdate(true);
//        emit positionChanged();
//	}

}

QVector3D TranslateOp::process(const QPoint& point)
{
	qtuser_3d::Ray ray = visRay(point);

	QVector3D planeCenter;
	QVector3D planeDir;
	getProperPlane(planeCenter, planeDir, ray);

	QVector3D c;
	qtuser_3d::lineCollidePlane(planeCenter, planeDir, ray, c);
    if(c.x() > PosMax) { c.setX(PosMax); }
    if(c.y() > PosMax) { c.setY(PosMax);}
    if(c.z() > PosMax) { c.setZ(PosMax);}
	return c;
}

void TranslateOp::getProperPlane(QVector3D& planeCenter, QVector3D& planeDir, qtuser_3d::Ray& ray)
{
	planeCenter = QVector3D(0.0f, 0.0f, 0.0f);
	qtuser_3d::Box3D box = m_selectedModels[m_selectedModels.size()-1]->globalSpaceBox();
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
	else if (m_mode == TMode::sp)
	{
		dirs << QVector3D(0.0f, 0.0f, 1.0f);
		dirs << QVector3D(0.0f, 0.0f, 1.0f);
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

QVector3D TranslateOp::clip(const QVector3D& delta)
{
	QVector3D clipDelta = delta;
	if (m_mode == TMode::x)  // x
	{
		clipDelta.setY(0.0f);
		clipDelta.setZ(0.0f);
	}
	else if (m_mode == TMode::y)
	{
		clipDelta.setX(0.0f);
		clipDelta.setZ(0.0f);
	}
	else if (m_mode == TMode::z)
	{
		clipDelta.setY(0.0f);
		clipDelta.setX(0.0f);
	}

	return clipDelta;
}

void TranslateOp::updateHelperEntity()
{
	if (m_selectedModels.size())
	{
		qtuser_3d::Box3D box = m_selectedModels[m_selectedModels.size()-1]->globalSpaceBox();
		m_helperEntity->updateBox(box);
	}

	//if (m_selectedModel)
	//{
	//	qtuser_3d::Box3D box = m_selectedModel->globalSpaceBox();
	//	m_helperEntity->updateBox(box);
	//}
}

void TranslateOp::onMachineSelectChange()
{
	updateHelperEntity();
}

bool TranslateOp::getShowPop()
{
	return m_bShowPop;
}


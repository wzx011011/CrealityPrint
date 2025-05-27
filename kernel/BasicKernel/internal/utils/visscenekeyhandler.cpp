#include "visscenekeyhandler.h"

#include "data/modeln.h"

#include "interface/selectorinterface.h"
#include "interface/modelinterface.h"
#include "interface/selectorinterface.h"
#include "interface/visualsceneinterface.h"
#include "interface/spaceinterface.h"
#include "kernel/modelnselector.h"
#include "kernel/abstractkernel.h"
#include "inout/cxopenandsavefilemanager.h"
using namespace qtuser_3d;
namespace creative_kernel
{
	VisSceneKeyHandler::VisSceneKeyHandler(QObject* parent)
		:QObject(parent)
	{
	}
	
	VisSceneKeyHandler::~VisSceneKeyHandler()
	{
	}

	void VisSceneKeyHandler::onKeyPress(QKeyEvent* event)
	{
		bool update = false;
		if (event->key() == Qt::Key_Delete)//用户在键盘上按下Delete键时
		{
			qDebug() << "delete";
			removeSelectionModels();
			checkModelRange();
			checkBedRange();
			update = true;
            //AbstractKernelUI::getSelf()->switchShowPop(false);
			//AbstractKernelUI::getSelf()->switchPickMode();//by TCJ

			//set LastOpenFileName after delete model
			QList<ModelN*> models = modelns();
			if (models.size() > 0)
			{
				creative_kernel::ModelN* model = models[0];

				CXFILE.setLastOpenFileName(model->objectName());
			}
		}

		if (event->modifiers() == Qt::ControlModifier && (event->key() == Qt::Key_Delete))//Ctrl+Delete 删除所有
		{
			qDebug() << "ctrl + delete";
			removeAllModel(true);
			checkModelRange();
			checkBedRange();
			update = true;
            //AbstractKernelUI::getSelf()->switchShowPop(false);
			//AbstractKernelUI::getSelf()->switchPickMode();
		}

		if (event->modifiers() == Qt::ControlModifier && (event->key() == Qt::Key_A))//当用户按下Ctrl+A键时
		{
			qDebug() << "ctrl + A";
			selectAll();
			update = true;
		}

		if (event->modifiers() == Qt::ControlModifier && (event->key() == Qt::Key_V))
		{
			QList<ModelN*> selections = selectionms();
			for (size_t i = 0; i < selections.size(); i++)
			{
				ModelN* m = selections.at(i);
				QString objectName = m->objectName();
				objectName.chop(4);
				creative_kernel::ModelN* model = new creative_kernel::ModelN();

                //for repair info 
                model->setErrorEdges(m->getErrorEdges());
                model->setErrorNormals(m->getErrorNormals());
                model->setErrorHoles(m->getErrorHoles());
                model->setErrorIntersects(m->getErrorIntersects());

				model->setTrimesh(m->meshptr());
				QString name = QString("%1#%2").arg(objectName).arg(i) + ".stl";
				model->setObjectName(name);
				model->setLocalScale(m->localScale());
				qtuser_3d::Box3D box = model->globalSpaceBox();
				QVector3D zoffset = QVector3D(0.0f, 0.0f, -box.min.z());
				QVector3D localPosition = model->localPosition();
				QVector3D newPosition = localPosition + zoffset;
				mixTSModel(model, model->localPosition(), newPosition, model->localScale(), m->localScale());
				addModel(model, true);
			}

			checkModelRange();
			checkBedRange();
			update = true;
		}

		if (event->key() == Qt::Key_Control)//用户在键盘上按下ctrl键时
		{
			getModelSelect()->setSelectStatus(true);
		}


		if(update) requestVisUpdate(true);
	}

	void VisSceneKeyHandler::onKeyRelease(QKeyEvent* event)
	{
		if (event->key()== Qt::Key_Control)//用户在键盘上按下ctrl键时
		{
			qDebug() << "release ctrl";
			getModelSelect()->setSelectStatus(false);
		}
	}
}

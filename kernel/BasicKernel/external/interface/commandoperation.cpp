#include "commandoperation.h"

#include "data/modeln.h"
#include "data/fdmsupportgroup.h"
#include "trimesh2/TriMesh.h"

#include "external/utils/matrixutil.h"
#include "external/utils/modelpositioninitializer.h"

#include "mmesh/trimesh/trimeshutil.h"

#include "external/operation/rotateop.h"
#include "interface/spaceinterface.h"

#include "spaceinterface.h"
#include "modelinterface.h"
#include "selectorinterface.h"
#include "visualsceneinterface.h"

#include "qtuser3d/math/angles.h"
#include "us/slicermanager.h"

namespace creative_kernel
{

    int cmdSaveAs(const QString& fileName)
    {
        std::vector<trimesh::TriMesh*> meshs;
        QList<ModelN*> models = selectionms();
        size_t size = models.size();

        if (size > 0)
        {
            for (size_t i = 0; i < models.size(); i++)
            {
                bool isfanzhuan = models.at(i)->isFanZhuan();
                QMatrix4x4 matrix = models.at(i)->globalMatrix();
                trimesh::TriMesh* meshTemp = models.at(i)->mesh();
                trimesh::TriMesh* newMesh = new trimesh::TriMesh();
                *newMesh = *meshTemp;

                trimesh::fxform xf = qMatrix2Xform(matrix);
                int _size = meshTemp->vertices.size();
                for (int n = 0; n < _size; ++n)
                {
                    trimesh::vec3 v = meshTemp->vertices.at(n);
                    newMesh->vertices.at(n) = xf * v;
                }
                if (isfanzhuan)
                {
                    _size = newMesh->faces.size();
                    for (int n = 0; n < _size; ++n)
                    {
                        newMesh->faces.at(n)[1] = meshTemp->faces.at(n)[2];
                        newMesh->faces.at(n)[2] = meshTemp->faces.at(n)[1];
                    }
                }
                meshs.push_back(newMesh);
            }

            trimesh::TriMesh* newmodel = new trimesh::TriMesh();
            mmesh::mergeTriMesh(newmodel, meshs);

            char buff[128];
            QByteArray qbyteTemp = fileName.toLocal8Bit();
            strcpy(buff, qbyteTemp.data());

            newmodel->write(buff);
            newmodel->clear();
        }

        qDeleteAll(meshs);
        return size;
    }

    void saveStl(const QString& fileName, ModelN* model)
    {
        std::vector<trimesh::TriMesh*> meshs;
        //QList<ModelN*> models = selectionms();
        //size_t size = models.size();

        //if (size > 0)
        {
            //for (size_t i = 0; i < models.size(); i++)
            {
                bool isfanzhuan = model->isFanZhuan();
                QMatrix4x4 matrix = model->globalMatrix();
                trimesh::TriMesh* meshTemp = model->mesh();
                trimesh::TriMesh* newMesh = new trimesh::TriMesh();
                *newMesh = *meshTemp;

                trimesh::fxform xf = qMatrix2Xform(matrix);
                int size = meshTemp->vertices.size();
                for (int n = 0; n < size; ++n)
                {
                    trimesh::vec3 v = meshTemp->vertices.at(n);
                    newMesh->vertices.at(n) = xf * v;
                }
                if (isfanzhuan)
                {
                    size = newMesh->faces.size();
                    for (int n = 0; n < size; ++n)
                    {
                        newMesh->faces.at(n)[1] = meshTemp->faces.at(n)[2];
                        newMesh->faces.at(n)[2] = meshTemp->faces.at(n)[1];
                    }
                }
                meshs.push_back(newMesh);
            }

            trimesh::TriMesh* newmodel = new trimesh::TriMesh();
            mmesh::mergeTriMesh(newmodel, meshs);

            char buff[128];
            QByteArray qbyteTemp = fileName.toLocal8Bit();
            strcpy(buff, qbyteTemp.data());

            newmodel->write(buff);
            newmodel->clear();
        }

        qDeleteAll(meshs);
    }

    void cmdSaveSelectStl(const QString filePath)
    {
        QList<ModelN*> models = selectionms();
        size_t size = models.size();
        if (size > 0)
        {
            for (size_t i = 0; i < models.size(); i++)
            {
                QString name = models[i]->objectName();
                QString fullName = name.section(".", 0, 0) + ".stl";
                saveStl(filePath + "/" + fullName, models[i]);
            }
        }
    }


    int cmdShowModelFace()
    {
        QList<creative_kernel::ModelN*> alls = creative_kernel::modelns();
        for (int i = 0; i < alls.size(); i++)
        {
            creative_kernel::ModelN* model = alls.at(i);
            creative_kernel::setModelShowType(model, 1);
            //creative_kernel::hideModelLine(model);
            //creative_kernel::showModelFace(model);
        }
        requestVisUpdate(true);
        qDebug() << "showModelFace";
        return alls.size();
    }

    int cmdShowModelLine()
    {
        QList<creative_kernel::ModelN*> alls = creative_kernel::modelns();
        for (int i = 0; i < alls.size(); i++)
        {
            creative_kernel::ModelN* model = alls.at(i);
            creative_kernel::setModelShowType(model, 2);
            //creative_kernel::showModelLine(model);
            //creative_kernel::hideModelFace(model);
        }
        requestVisUpdate(true);
        qDebug() << "showModelLine";
        return alls.size();
    }

    int cmdShowModelFaceLine()
    {
        QList<creative_kernel::ModelN*> alls = creative_kernel::modelns();
        for (int i = 0; i < alls.size(); i++)
        {
            creative_kernel::ModelN* model = alls.at(i);
            creative_kernel::setModelShowType(model, 3);
            //creative_kernel::showModelLine(model);
            //creative_kernel::showModelFace(model);
        }
        requestVisUpdate(true);
        qDebug() << "showModelFaceLine";
        return alls.size();
    }

    int cmdReset()
    {
        ModelSpace* space = getModelSpace();
        QList<ModelN*> models = space->modelns();
        for (size_t i = 0; i < models.size(); i++)
        {
            QVector3D oldLocalPosition = models.at(i)->localPosition();
            QVector3D initPosition = models.at(i)->GetInitPosition();
            initPosition.setZ(0.0f);
            //setVisOperationMode();
            mirrorReset(models.at(i), true);
            setModelRotation(models.at(i), QQuaternion(), true);
            setModelScale(models.at(i), QVector3D(1.0f, 1.0f, 1.0f), true);
            moveModel(models.at(i), oldLocalPosition, models.at(i)->mapGlobal2Local(initPosition), true);
            updateModel(models.at(i));
                    
            models.at(i)->fdmSupport()->clearSupports();
            requestVisUpdate(true);
        }
        checkModelRange();
        checkBedRange();
        return models.size();
    }

    int cmdResetAll()
    {
        ModelSpace* space = getModelSpace();
        QList<ModelN*> models = space->modelns();
        for (size_t i = 0; i < models.size(); i++)
        {
            QVector3D oldLocalPosition = models.at(i)->localPosition();
            QVector3D initPosition = models.at(i)->GetInitPosition();
            initPosition.setZ(0.0f);
            //setVisOperationMode();
            mirrorReset(models.at(i), true);
            setModelRotation(models.at(i), QQuaternion(), true);
            setModelScale(models.at(i), QVector3D(1.0f, 1.0f, 1.0f), true);
            moveModel(models.at(i), oldLocalPosition, models.at(i)->mapGlobal2Local(initPosition), true);
            updateModel(models.at(i));
        }
        checkModelRange();
        checkBedRange();
        return models.size();
    }

    int cmdClone(int numb)
    {
		us::GlobalSetting* gsets = SlicerManager::instance().globalsettings();
		bool isBelt = gsets->settings().value("machine_is_belt")->value().toBool();

        QList<ModelN*> selections = selectionms();
        if (numb <= 0 || numb > 50 || selections.size() == 0)
        {
            qDebug() << "clone invalid num.";
            return -1;
        }
        int nameIndex = 0;
        QString sname;
        for (size_t i = 0; i < selections.size(); i++)
        {
            ModelN* m = selections.at(i);
            QString objectName = m->objectName();
            objectName.chop(4);
            for (int j = 0; j < numb; ++j)
            {
                creative_kernel::ModelN* model = new creative_kernel::ModelN();

                model->setTrimesh(m->meshptr());
                nameIndex = j;
                QString name = QString("%1-%2").arg(objectName).arg(nameIndex) + ".stl";                
                //---                
                QList<ModelN*> models = modelns();
                for (int k = 0; k < models.size(); ++k)
                {
                    sname = models[k]->objectName();
                    if (name == sname)
                    {
                        nameIndex++;
                        name = QString("%1-%2").arg(objectName).arg(nameIndex) + ".stl";
                        k = -1;
                    }                   
                }
                //---
                model->setObjectName(name);   
                if (!isBelt)
                {
					model->setLocalPosition(m->localPosition(), true);
					model->setLocalScale(m->localScale(), true);
					model->setLocalQuaternion(m->localQuaternion(), true);
                }
                if (m->hasFDMSupport())
                {
                    FDMSupportGroup* fdmSup = model->fdmSupport();
                    fdmSup->buildFDMSup(m->fdmSupport()->FDMSupports());
                    model->setFDMSup(fdmSup);
                }
				if (isBelt)
				{
					ModelPositionInitializer::layoutBelt(model, nullptr);
				}
				else
				{
					ModelPositionInitializer::layout(model, nullptr);
				}
                //model->needDetectError();
                addModel(model, true);
                model->updateMatrix();
            }
        }

        requestVisUpdate(true);
        checkModelRange();
        checkBedRange();
        return selections.size();
    }

    int cmdSetNozzle(int nozzle)
    {
        QList<ModelN*> selections = selectionms();
        for (size_t i = 0; i < selections.size(); i++)
        {
            ModelN* m = selections.at(i);
            m->setNozzle(nozzle);
        }
        return selections.size();
    }

    int cmdOneSideAsBottomFace(QPoint point)
    {
        QVector3D position, normal;
        ModelN* model = checkPickModel(point, position, normal);
        if (model)
        {
            normal.normalize();
            QVector3D z = QVector3D(0.0f, 0.0f, -1.0f);
            QQuaternion q = quaternionFromVector3D2(normal, z);
            rotateModelOnPlane(model, q);

            requestVisUpdate(true);

            return 1;
        }
        return 0;
    }

    int cmdPlaceOnPrinterBed()
    {
        QList<ModelN*> models = selectionms();
        for (size_t i = 0; i < models.size(); i++)
        {
            qtuser_3d::Box3D box = models.at(i)->globalSpaceBox();
            QVector3D zoffset = QVector3D(0.0f, 0.0f, -box.min.z());
            QVector3D localPosition = models.at(i)->localPosition();
            moveModel(models.at(i), localPosition, localPosition + zoffset, true);
            updateModel(models.at(i));
        }
        return models.size();
    }


	int cmdMergeModel()
	{
		ModelSpace* space = getModelSpace();
		QList<ModelN*> models = space->modelns();

		if (models.size() > 1)
		{
			QVector3D center = models.at(0)->globalSpaceBox().center();
			for (size_t i = 1; i < models.size(); i++)
			{
				ModelN* model = models.at(i);
				QVector3D iSize = model->globalSpaceBox().size();
				QVector3D iCenter = model->mapGlobal2Local(center);
                iCenter.setZ(iCenter.z() - center.z());
				//iCenter.setZ(iCenter.z() - iSize.z() / 2.0f);
				moveModel(model, model->localPosition(), iCenter, true);
			}
		}
        return modelns().size();
	}

}

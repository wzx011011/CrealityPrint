#include "selectorinterface.h"
#include "external/kernel/kernel.h"
#include "data/modeln.h"
#include "qtuser3d/module/pickable.h"
#include "kernel/modelnselector.h"

#include "interface/visualsceneinterface.h"
#include "interface/camerainterface.h"
#include "external/utils/trimeshselect.h"

using namespace qtuser_3d;
namespace creative_kernel
{
	void addSelectTracer(SelectorTracer* tracer)
	{
		getKernel()->modelSelector()->addTracer(tracer);
	}

	void removeSelectorTracer(SelectorTracer* tracer)
	{
		getKernel()->modelSelector()->removeTracer(tracer);
	}

	void disableReverseSelect(bool disable)
	{
		getKernel()->modelSelector()->disableReverseSelect(disable);
	}

	void enableSupportMode(bool able)
	{
		getKernel()->modelSelector()->enableSupportMode(able);
	}

	QList<ModelN*> selectionms()
	{
		return getKernel()->modelSelector()->selectionms();
	}

	ModelN* hoverCandidate()
	{
		return getKernel()->modelSelector()->hoverCandidate();
	}


	void tracePickable(Pickable* entity, bool request)
	{
		getKernel()->modelSelector()->addPickable(entity, request);
	}

	void unTracePickable(Pickable* entity, bool request)
	{
		getKernel()->modelSelector()->removePickable(entity, request);
	}

	Pickable* checkPickable(const QPoint& point, int* primitiveID)
	{
		return getKernel()->modelSelector()->check(point, primitiveID);
	}

	ModelN* checkPickModel(const QPoint& point, QVector3D& position, QVector3D& normal, int* faceID)
	{
		int primitiveID = -1;
		Pickable* pickable = checkPickable(point, &primitiveID);
		ModelN* model = qobject_cast<ModelN*>(pickable);
		if (model)
		{
			model->rayCheck(primitiveID, visRay(point), position, &normal);
			if (model->isFanZhuan())
			{
				normal = -normal;
			}
		}
		if (faceID)
		{
			*faceID = primitiveID;
		}
		return model;
	}

	ModelN* rclickModelN(const QPoint& point)
	{
		int primitiveID = -1;
		Pickable* pickable = checkPickable(point, &primitiveID);

		if (pickable != nullptr)
		{
			QString qname = pickable->metaObject()->className();
			if (qname.indexOf("ModelN") == -1)
			{
				return nullptr;
			}
		}
		return (ModelN*)pickable;
	}

	int getModelSelectFace(ModelN* model, int faceId, std::vector<QVector3D>& vertexData, std::vector<QVector3D>& normalData, bool global)
	{
		trimesh::TriMesh* mesh = model->mesh();
		
		if (!mesh || (faceId < 0) || (faceId >= mesh->faces.size()))
			return 0;

		trimeshSelect(mesh, faceId, vertexData);
		if (global)
		{
			QMatrix4x4 m = model->globalMatrix();
			for (int i = 0; i < 3; ++i)
			{
				QVector3D v = vertexData.at(i);
				vertexData.at(i) = m * v;
			}
		}

		QVector3D v01 = vertexData.at(1) - vertexData.at(0);
		QVector3D v02 = vertexData.at(1) - vertexData.at(0);
		QVector3D n = QVector3D::crossProduct(v01, v02);
		n.normalize();
		normalData.resize(3, n);
		return 3;
	}

	void selectedPickableInfoChanged(qtuser_3d::Pickable* pickable)
	{
		getKernel()->modelSelector()->changed(pickable);
	}

	bool hoverVis(QPoint point)
	{
		return getKernel()->modelSelector()->hover(point);
	}

	void selectVis(QPoint point)
	{
		getKernel()->modelSelector()->select(point);
	}

	void selectAll()
	{
		getKernel()->modelSelector()->selectAll();
		requestVisUpdate();
	}
	
		
	ModelNSelector* getModelSelect()
	{
		return getKernel()->modelSelector();
	}

    void selectModel(QObject* model,bool state)
    {
        Pickable* pickable = qobject_cast<Pickable*>(model);
        getKernel()->modelSelector()->select(pickable);
    }

	void appendSelect(qtuser_3d::Pickable* pickable)
	{
		getKernel()->modelSelector()->appendSelect(pickable);
	}

	void selectOne(qtuser_3d::Pickable* pickable)
	{
		getKernel()->modelSelector()->select(pickable);
	}

	void disSelect(qtuser_3d::Pickable* pickable)
	{
		QList<qtuser_3d::Pickable*> onList;
		QList<qtuser_3d::Pickable*> offList;
		offList << pickable;
		getKernel()->modelSelector()->select(onList, offList);
	}

	void selectMore(QList<qtuser_3d::Pickable*> pickables)
	{
		QList<qtuser_3d::Pickable*> offLists = getKernel()->modelSelector()->selectionmPickables();
		getKernel()->modelSelector()->select(pickables, offLists);
	}

	void updateFaceBases()
	{
		getKernel()->modelSelector()->updateFaceBases();
	}
}

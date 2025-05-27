#include "modelspace.h"
#include "modelspaceobserver.h"

#include "data/modeln.h"
#include "qtuser3d/math/box3d.h"
//#include "interface/modelinterface.h"

#include "qtuser3d/entity/modelnentity.h"
#include "qtuser3d/entity/printerentity.h"

#include "external/interface/reuseableinterface.h"

using namespace qtuser_3d;
namespace creative_kernel
{
	ModelSpace::ModelSpace(QObject* parent)
		:QObject(parent)
		, m_root(nullptr)
		, m_currentModelGroup(nullptr)
		, m_spaceDirty(true)
	{
		m_baseBoundingBox += QVector3D(0.0f, 0.0f, 0.0f);
		m_baseBoundingBox += QVector3D(300.0f, 300.0f, 300.0f);

		m_root = new Qt3DCore::QEntity();
	}

	ModelSpace::~ModelSpace()
	{
		delete m_root;
	}

	void ModelSpace::uninitialize()
	{
		m_root->setParent((Qt3DCore::QNode*)nullptr);
		QList<Qt3DCore::QEntity*> entities = m_root->findChildren<Qt3DCore::QEntity*>(QString(), Qt::FindDirectChildrenOnly);
		for(Qt3DCore::QEntity* entity : entities)
			entity->setParent((Qt3DCore::QNode*)nullptr);
	}

	void ModelSpace::addSpaceTracer(SpaceTracer* tracer)
	{
		if (tracer)
		{
			tracer->onBoxChanged(m_baseBoundingBox);
			m_spaceTracers.push_back(tracer);
		}
	}

	void ModelSpace::removeSpaceTracer(SpaceTracer* tracer)
	{
		if(tracer)
			m_spaceTracers.removeAt(m_spaceTracers.indexOf(tracer));
	}

	qtuser_3d::Box3D ModelSpace::baseBoundingBox()
	{
		return m_baseBoundingBox;
	}

	qtuser_3d::Box3D ModelSpace::sceneBoundingBox()
	{
		qtuser_3d::Box3D box;
		box += QVector3D(0.0f, 0.0f, 0.0f);

		QList<ModelN*> models = modelns();
		for (ModelN* m : models)
		{
			box += m->globalSpaceBox();
		}
		return box;
	}

	void ModelSpace::setBaseBoundingBox(const qtuser_3d::Box3D& box)
	{
		if (m_baseBoundingBox == box) return;

		m_baseBoundingBox = box;
		for (SpaceTracer* tracer : m_spaceTracers)
			tracer->onBoxChanged(m_baseBoundingBox);
	}
	void ModelSpace::onModelSceneChanged()
	{
		setModelEffectClipMaxZ(10000.0);
		qtuser_3d::Box3D box = sceneBoundingBox();
		for (SpaceTracer* tracer : m_spaceTracers)
			tracer->onSceneChanged(box);
	}
	void ModelSpace::checkCollide()
	{
	}

	int ModelSpace::checkModelRange()
	{
		//判断模型是否在超出平台大小
		QVector3D bbOffset(0.1f, 0.1f, 0.1f);
		Box3D basebox = baseBoundingBox();
		basebox.min = basebox.min - bbOffset;
		basebox.max = basebox.max + bbOffset;
		QList<ModelN*> models = modelns();
		bool  bleft = false;
		bool  bfront = false;
		bool  bdown = false;
		bool  bright = false;
		bool  bback = false;
		bool  bup = false;
		int result = 0;
		for (size_t i = 0; i < models.size(); i++)
		{
			ModelN* model = models.at(i);
			qtuser_3d::Box3D LocalBox = model->globalSpaceBox();
			if (LocalBox.min.x() < basebox.min.x() ||
				LocalBox.min.y() < basebox.min.y() ||
				LocalBox.min.z() + 0.01 < basebox.min.z() ||
				LocalBox.max.x() > basebox.max.x() ||
				LocalBox.max.y() > basebox.max.y() ||
				LocalBox.max.z() > basebox.max.z())
			{
				model->setErrorState(true);
			}
			else
			{
				model->setErrorState(false);
			}

			//显示面
			if (LocalBox.min.x() < basebox.min.x())
			{
				bleft = true;
				result |= 1;
			}
			if (LocalBox.min.y() < basebox.min.y())
			{
				bfront = true;
				result |= 2;
			}
			if (LocalBox.min.z() + 0.01 < basebox.min.z())
			{
				bdown = true;
				result |= 4;
			}
			if (LocalBox.max.x() > basebox.max.x())
			{
				bright = true;
				result |= 8;
			}
			if (LocalBox.max.y() > basebox.max.y())
			{
				bback = true;
				result |= 16;
			}
			if (LocalBox.max.z() > basebox.max.z())
			{
				bup = true;
				result |= 32;
			}
		}
		qtuser_3d::PrinterEntity* entity = getCachedPrinterEntity();
		entity->onModelChanged(basebox, bleft, bright, bfront, bback, bup, bdown);
		return result;
	}

	int ModelSpace::checkBedRange()
	{
		QList<ModelN*> models = modelns();
		QList<Box3D> boxes;
		for (ModelN* amodel :models)
		{
			boxes.append(amodel->globalSpaceBox());
		}

		qtuser_3d::PrinterEntity* entity = getCachedPrinterEntity();
		entity->onCheckBed(boxes);
		return true;
	}

	void ModelSpace::addModelGroup(ModelGroup* modelGroup)
	{
		modelGroup->setParent(this);
		modelGroup->setParent2Global(QMatrix4x4());
		modelGroup->setChildrenVisibility(true);
		m_currentModelGroup = modelGroup;
	}

	void ModelSpace::removeModelGroup(ModelGroup* modelGroup)
	{
		modelGroup->setParent(nullptr);
		modelGroup->setChildrenVisibility(false);
		if (m_currentModelGroup == modelGroup)
			m_currentModelGroup = nullptr;
	}
	int  ModelSpace::modelNumber() {
		QList<ModelN*> models;
		QList<ModelGroup*> groups = modelGroups();
		for (ModelGroup* group : groups)
		{
			models << group->models();
		}
		return models.size();
	}
	QList<ModelN*> ModelSpace::modelns()
	{
		QList<ModelN*> models;
		QList<ModelGroup*> groups = modelGroups();
		for(ModelGroup* group : groups)
		{
			models << group->models();
		}
		return models;
	}

	Qt3DCore::QEntity* ModelSpace::rootEntity()
	{
		return m_root;
	}

	ModelGroup* ModelSpace::currentGroup()
	{
		return m_currentModelGroup;
	}

	QList<ModelGroup*> ModelSpace::modelGroups()
	{
		QList<ModelGroup*> groups = findChildren<ModelGroup*>(QString(), Qt::FindDirectChildrenOnly);
		return groups;
	}

	void ModelSpace::setSpaceDirty(bool _spaceDirty)
	{
		m_spaceDirty = _spaceDirty;
	}

	bool ModelSpace::spaceDirty()
	{
		return m_spaceDirty;
	}
}

#include "modelgroup.h"

#include "external/data/modeln.h"
#include "qtuser3d/entity/modelnentity.h"
#include "external/interface/visualsceneinterface.h"
#include "external/interface/selectorinterface.h"
#include "us/modellistsetting.h"
namespace creative_kernel
{
	ModelGroup::ModelGroup(QObject* parent)
		:Node3D(parent)
	{
		m_parentMatrixDirty = false;
		m_localMatrixDirty = false;
		m_setting = new us::ModelListSetting(this);
	}
	
	ModelGroup::~ModelGroup()
	{
	}

	void ModelGroup::addModel(ModelN* model)
	{
		if (!model) return;

		model->setParent(this);
		model->updateMatrix();
		model->setVisibility(true);
	}

	void ModelGroup::removeModel(ModelN* model)
	{
		if (!model) return;
		
		model->setParent(nullptr);
		model->setVisibility(false);
	}

	void ModelGroup::setChildrenVisibility(bool visibility)
	{
		QList<ModelN*> modelns = models();
		for (ModelN* modeln : modelns)
		{
			modeln->setVisibility(visibility);
		}
	}

	QList<ModelN*> ModelGroup::models()
	{
		QList<ModelN*> ml = findChildren<ModelN*>(QString(), Qt::FindDirectChildrenOnly);
		return ml;
	}

	us::ModelListSetting* ModelGroup::setting()
	{
		return m_setting;
	}
}
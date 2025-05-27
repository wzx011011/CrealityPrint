#include "modeladd2groupcommand.h"
#include "interface/modelinterface.h"

namespace creative_kernel
{
	ModelAdd2GroupCommand::ModelAdd2GroupCommand(Qt3DCore::QNode* parent)
		:QNode(parent)
		, m_group(nullptr)
		, m_reverse(false)
	{
	}
	
	ModelAdd2GroupCommand::~ModelAdd2GroupCommand()
	{
	}

	void ModelAdd2GroupCommand::undo()
	{
		if (m_reverse)
		{
			deal(false);
		}
		else
			deal(true);
	}

	void ModelAdd2GroupCommand::redo()
	{
		if (m_reverse)
		{
			deal(true);
		}
		else
			deal(false);
	}

	void ModelAdd2GroupCommand::setReverse(bool reverse)
	{
		m_reverse = reverse;
	}

	void ModelAdd2GroupCommand::setModels(const QList<ModelN*>& models)
	{
		m_modelList = models;
	}

	void ModelAdd2GroupCommand::setRemoveModels(const QList<ModelN*>& models)
	{
		m_modelRemoveList = models;
	}

	void ModelAdd2GroupCommand::setGroup(ModelGroup* group)
	{
		m_group = group;
	}

	void ModelAdd2GroupCommand::deal(bool remove)
	{
		for (ModelN* model : m_modelList)
		{
			if (remove)
			{
				_removeModelFromGroup(model, m_group);
				model->setParent(this);
			}
			else
			{
				_addModel2Group(model, m_group);
			}
		}

		for (ModelN* model : m_modelRemoveList)
		{
			if (remove)
			{
				_addModel2Group(model, m_group);
			}
			else
			{
				_removeModelFromGroup(model, m_group);
				model->setParent(this);
			}
		}

		_requestUpdate();
	}
}
#include "groupaddcommand.h"
#include "interface/modelinterface.h"

namespace creative_kernel
{
	GroupAddCommand::GroupAddCommand(Qt3DCore::QNode* parent)
		:QNode(parent)
		, m_reverse(false)
	{
	}
	
	GroupAddCommand::~GroupAddCommand()
	{
	}


	void GroupAddCommand::undo()
	{
		if (m_reverse)
		{
			deal(false);
		}
		else
			deal(true);
	}

	void GroupAddCommand::redo()
	{
		if (m_reverse)
		{
			deal(true);
		}
		else
			deal(false);
	}

	void GroupAddCommand::setReverse(bool reverse)
	{
		m_reverse = reverse;
	}

	void GroupAddCommand::setGroups(QList<ModelGroup*> groups)
	{
		m_groups = groups;
	}

	void GroupAddCommand::deal(bool remove)
	{
		for (ModelGroup* group : m_groups)
		{
			if (remove)
			{
				_removeGroup(group);
				group->setParent(this);
			}
			else
			{
				_addGroup(group);
			}
		}
	}
}
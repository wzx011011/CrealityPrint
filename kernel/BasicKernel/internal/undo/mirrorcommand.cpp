#include "mirrorcommand.h"
#include "external/interface/modelinterface.h"

namespace creative_kernel
{
	MirrorCommand::MirrorCommand(QObject* parent)
		:QObject(parent)
	{
	}
	
	MirrorCommand::~MirrorCommand()
	{
	}

	void MirrorCommand::setMirrors(const QList<NMirrorStruct>& mirrorStructs)
	{
		m_mirrors = mirrorStructs;
	}

	void MirrorCommand::undo()
	{
		call(true);
	}

	void MirrorCommand::redo()
	{
		call(false);
	}

	void MirrorCommand::call(bool reverse)
	{
		for (const NMirrorStruct& mirrorStruct : m_mirrors)
		{
			ModelN* model = mirrorStruct.model;

			if (mirrorStruct.type == 0)
			{
				if (reverse)
				{
					_mirrorSet(model, mirrorStruct.start);
				}
				else
				{
					_mirrorSet(model, mirrorStruct.end);
				}
			}
			else if (mirrorStruct.type == 1)
			{
				_mirrorX(model);
			}
			else if (mirrorStruct.type == 2)
			{
				_mirrorY(model);
			}
			else if (mirrorStruct.type == 3)
			{
				_mirrorZ(model);
			}
		}

		_requestUpdate();
	}
}
#include "nmixchangecommand.h"
#include "data/modeln.h"

#include "interface/modelinterface.h"
#include "interface/spaceinterface.h"

namespace creative_kernel
{
	NMixChangeCommand::NMixChangeCommand(QObject* parent)
		:QObject(parent)
	{
	}
	
	NMixChangeCommand::~NMixChangeCommand()
	{
	}

	void NMixChangeCommand::undo()
	{
		call(false);
	}

	void NMixChangeCommand::redo()
	{
		call(true);
	}

	void NMixChangeCommand::setChanges(const QList<NUnionChangedStruct>& changes)
	{
		m_changes = changes;
	}

	void NMixChangeCommand::call(bool redo)
	{
		_mixUnions(m_changes, redo);
	}

	MeshChangeCommand::MeshChangeCommand(QObject* parent)
		:QObject(parent)
	{
	}

	MeshChangeCommand::~MeshChangeCommand()
	{
	}

	void MeshChangeCommand::undo()
	{
		call(false);
	}

	void MeshChangeCommand::redo()
	{
		call(true);
	}

	void MeshChangeCommand::setChanges(const QList<MeshChange>& changes)
	{
		m_changes = changes;
	}

	void MeshChangeCommand::call(bool redo)
	{
		QList<ModelN*> models;
		QList<TriMeshPtr> meshs;
		QList<QString> names;

		QList<TriMeshPtr> meshs_aux;
		QList<int> aux_pos;
		bool have_aux = false;

		for (MeshChange& changeStruct : m_changes)
		{
			ModelN* model = changeStruct.model;
			if (changeStruct.start != changeStruct.end)
			{
				TriMeshPtr mesh = redo ? changeStruct.end : changeStruct.start;
				QString name = redo ? changeStruct.endName : changeStruct.startName;
				models.push_back(model);
				meshs.push_back(mesh);
				names.push_back(name);

				TriMeshPtr mesh_aux = redo ? changeStruct.aux_end : changeStruct.aux_start;
				meshs_aux.push_back(mesh_aux);
				if (mesh_aux)
				{
					have_aux = true;
				}
				aux_pos.push_back(changeStruct.aux_pos);
			}
		}
		if (have_aux)
		{
			replaceModelsMesh(models, meshs, names, false, &meshs_aux, &aux_pos);

		}
		else
		{
			replaceModelsMesh(models, meshs, names);
		}
	}
}
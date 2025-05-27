#include "modelspaceundo.h"
#include "internal/undo/modeladd2groupcommand.h"
#include "internal/undo/groupaddcommand.h"
#include "internal/undo/nmixchangecommand.h"
#include "internal/undo/mirrorcommand.h"
#include "external/interface/selectorinterface.h"
#include "internal/undo/nmixchangecommand.h"

namespace creative_kernel
{
	ModelSpaceUndo::ModelSpaceUndo(QObject* parent)
		:QUndoStack(parent)
	{
        setUndoLimit(5);
	}
	
	ModelSpaceUndo::~ModelSpaceUndo()
	{
	}

	void ModelSpaceUndo::replaceModels(const QList<ModelN*>& models, const QList<TriMeshPtr>& meshs,
		const QList<QString>& names, const QList<TriMeshPtr>* aux_meshes, QList<int>* aux_pos)
	{
		int count = models.size();
		if (count == 0)
			return;

		QList<MeshChange> changes;
		for (int i = 0; i < count; ++i)
		{
			ModelN* model = models.at(i);
			MeshChange change;
			change.model = model;
			change.start = model->meshptr();
			change.end = meshs.at(i);
			change.startName = model->objectName();
			change.endName = names.at(i);

			if (aux_meshes && aux_pos && aux_meshes->size() == aux_pos->size())
			{
				if (aux_pos->at(i) >= 0 && aux_pos->at(i) < model->numAuxiliaryMesh())
				{
					change.aux_start = model->getAuxiliaryMeshPtr(i);
					change.aux_end = aux_meshes->at(i);
					change.aux_pos = aux_pos->at(i);

				}
			}

			changes.push_back(change);
		}
		MeshChangeCommand* command = new MeshChangeCommand();
		command->setChanges(changes);

		push(command);
	}

	void ModelSpaceUndo::addModels2Group(ModelGroup* group, QList<ModelN*>& models, QList<ModelN*>& removeModels)
	{
		ModelAdd2GroupCommand* command = new ModelAdd2GroupCommand();
		command->setModels(models);
		command->setRemoveModels(removeModels);
		command->setGroup(group);

		push(command);
	}

	void ModelSpaceUndo::addGroups(QList<ModelGroup*>& groups)
	{
		GroupAddCommand* command = new GroupAddCommand();
		command->setGroups(groups);

		push(command);
	}

	void ModelSpaceUndo::mix(const QList<NUnionChangedStruct>& mixChange)
	{
		NMixChangeCommand* command = new NMixChangeCommand();
		command->setChanges(mixChange);

		push(command);
	}

	void ModelSpaceUndo::mirror(const QList<NMirrorStruct>& mirrors)
	{
		MirrorCommand* command = new MirrorCommand();
		command->setMirrors(mirrors);

		push(command);
	}
}
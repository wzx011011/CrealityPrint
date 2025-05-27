#include "modelinterface.h"
#include "external/kernel/kernel.h"

#include "qtusercore/module/progressor.h"

#include "data/modeln.h"
#include "data/modelgroup.h"
#include "external/data/modelspace.h"
#include "external/data/modelspaceundo.h"
#include "external/utils/modelpositioninitializer.h"

#include "external/interface/visualsceneinterface.h"
#include "external/interface/selectorinterface.h"
#include "external/interface/reuseableinterface.h"
#include "external/interface/spaceinterface.h"

namespace creative_kernel
{
	static UIFunOO spFunAddModel = nullptr;
	static UIFunO  spFunDelModel = nullptr;
	static UIFunIO spFunAddGroup = nullptr;

	void setAddModelFun(UIFunOO fun)
	{
		spFunAddModel = fun;
	}

	void setDelModelFun(UIFunO fun)
	{
		spFunDelModel = fun;
	}

	void setAddGroupFun(UIFunIO fun)
	{
		spFunAddGroup = fun;
	}

	void callAddModelFun(QObject* group, QObject* model)
	{
		if (spFunAddModel != nullptr)
		{
			(*spFunAddModel)(group, model);
		}
	}

	void callDelModelFun(QObject* model)
	{
		if (spFunDelModel != nullptr)
		{
			(*spFunDelModel)(model);
		}
	}

	void callAddGroupFun(int index, QObject* group)
	{
		if (spFunAddGroup != nullptr)
		{
			(*spFunAddGroup)(index, group);
		}
	}


	void __mixChange(const QList<NUnionChangedStruct>& changes)
	{
		ModelSpaceUndo* stack = getKernel()->modelSpaceUndo();
		stack->mix(changes);
	}

	void __mirror(const QList<NMirrorStruct>& mirrors)
	{
		ModelSpaceUndo* stack = getKernel()->modelSpaceUndo();
		stack->mirror(mirrors);
	}

	void addModel(ModelN* model, bool reversible, qtuser_core::Progressor* progressor)
	{
		if (!model) return;

		ModelSpace* space = getKernel()->modelSpace();
		ModelGroup* group = space->currentGroup();

		if (!group)
		{
			group = new ModelGroup();
			addGroup(group, reversible);
		}

		addModel2Group(model, group, reversible);

		creative_kernel::onModelSceneChanged();
	}

	void addModel2Group(ModelN* model, ModelGroup* group, bool reversible)
	{
		if (!model || !group)
			return;

		if (reversible)
		{
			ModelSpaceUndo* stack = getKernel()->modelSpaceUndo();
			QList<ModelN*> models;
			models << model;
			QList<ModelN*> removes;
			stack->addModels2Group(group, models, removes);
			return;
		}

		_addModel2Group(model, group, true);
	}

	void addGroup(ModelGroup* group, bool reversible)
	{
		if (!group)
			return;

		if (reversible)
		{
			ModelSpaceUndo* stack = getKernel()->modelSpaceUndo();
			QList<ModelGroup*> groups;
			groups << group;
			stack->addGroups(groups);
			return;
		}

		_addGroup(group);
	}

	void modifyGroup(QList<ModelN*>& removes, QList<ModelN*>& adds, ModelGroup* group, bool reversible)
	{
		if ((removes.size() == 0 && adds.size() == 0) || !group)
			return;

		if (reversible)
		{
			ModelSpaceUndo* stack = getKernel()->modelSpaceUndo();
			stack->addModels2Group(group, adds, removes);
			return;
		}

		for (ModelN* m : removes)
		{
			_removeModelFromGroup(m, group);
		}
		for (ModelN* m : adds)
		{
			_addModel2Group(m, group);
		}
		_requestUpdate();
		creative_kernel::onModelSceneChanged();
	}

	void removeAllModel(bool reversible)
	{
		ModelSpace* space = getModelSpace();

		if (reversible)
		{
			ModelSpaceUndo* stack = getKernel()->modelSpaceUndo();
			for (ModelGroup* mg : space->modelGroups())
			{
                                QList<ModelN*> nlist;
                                QList<ModelN*> nmodels=mg->models();
                                stack->addModels2Group(mg, nlist, nmodels);
			}
			return;
		}

		for (ModelGroup* mg : space->modelGroups())
		{
			for (ModelN* m : mg->models())
			{
				_removeModelFromGroup(m, mg);
			}
		}
		creative_kernel::onModelSceneChanged();
	}

	void removeSelectionModels()
	{
		QList<ModelN*> models = selectionms();
		if (models.size() > 0)
		{
			ModelSpace* space = getModelSpace();
			ModelGroup* group = space->currentGroup();
			QList<ModelN*> nlist;
			modifyGroup(models, nlist, group, true);
		}
	}

	void replaceModelsMesh(const QList<ModelN*>& models, const QList<TriMeshPtr>& meshes,
		const QList<QString>& names, bool reversible, const QList<TriMeshPtr>* aux_meshes, QList<int>* aux_pos)
	{
		if (models.size() != meshes.size()
			&& models.size() != names.size())
			return;

		if (reversible)
		{
			ModelSpaceUndo* stack = getKernel()->modelSpaceUndo();
			stack->replaceModels(models, meshes, names, aux_meshes, aux_pos);
			return;
		}

		int count = models.size();
		for (int i = 0; i < count; ++i)
		{
			trimesh::TriMesh* aux_mesh = nullptr;
			int pos = -1;
			if (aux_meshes && aux_pos && aux_meshes->size() == aux_pos->size())
			{
				aux_mesh = aux_meshes->at(i).get();
				pos = aux_pos->at(i);
			}
			_replaceModelMesh(models.at(i), meshes.at(i), names.at(i), aux_mesh, pos);
		}

		updateFaceBases();
		requestVisUpdate(true);
	}

	void moveModel(ModelN* model, const QVector3D& start, const QVector3D& end, bool reversible, bool needcheck)
	{
		if (!model)
			return;
		if (reversible)
		{
			QList<NUnionChangedStruct> changeStructs;

			NUnionChangedStruct changeStruct;
			changeStruct.model = model;
			changeStruct.posActive = true;
			changeStruct.posChange.start = start;
			changeStruct.posChange.end = end;

			changeStructs.push_back(changeStruct);
			__mixChange(changeStructs);
			if (needcheck)
			{
				checkModelRange();
				checkBedRange();
			}
			return;
		}

		setModelPosition(model, end, true);
		if (needcheck)
		{
			checkModelRange();
			checkBedRange();
		}
	}

	void mixTSModel(ModelN* model, const QVector3D& tstart, const QVector3D& tend,
		const QVector3D& sstart, const QVector3D& send, bool reversible, bool needcheck)
	{
		if (!model)
			return;
		
		if (reversible)
		{
			QList<NUnionChangedStruct> changeStructs;

			NUnionChangedStruct changeStruct;
			changeStruct.model = model;
			changeStruct.posActive = true;
			changeStruct.posChange.start = tstart;
			changeStruct.posChange.end = tend;
			changeStruct.scaleActive = true;
			changeStruct.scaleChange.start = sstart;
			changeStruct.scaleChange.end = send;

			changeStructs.push_back(changeStruct);
			__mixChange(changeStructs);
			return;
		}

		setModelPosition(model, tend);
		setModelScale(model, send);
		updateModel(model);
		if (needcheck)
		{
			checkModelRange();
			checkBedRange();
		}
	}

	void mixTRModel(ModelN* model, const QVector3D& tstart, const QVector3D& tend,
		const QQuaternion& rstart, const QQuaternion& rend, bool reversible, bool needcheck)
	{
		if (!model)
			return;
		
		if (reversible)
		{
			QList<NUnionChangedStruct> changeStructs;

			NUnionChangedStruct changeStruct;
			changeStruct.model = model;
			changeStruct.posActive = true;
			changeStruct.posChange.start = tstart;
			changeStruct.posChange.end = tend;
			changeStruct.rotateActive = true;
			changeStruct.rotateChange.start = rstart;
			changeStruct.rotateChange.end = rend;

			changeStructs.push_back(changeStruct);
			__mixChange(changeStructs);
			return;
		}

		setModelPosition(model, tend);
		setModelRotation(model, rend);
		updateModel(model);
		if (needcheck)
		{
			checkModelRange();
			checkBedRange();
		}
	}

	void mixUnions(const QList<NUnionChangedStruct>& structs, bool reversible)
	{
		if (structs.size() == 0)
			return;

		if (reversible)
		{
			__mixChange(structs);
			return;
		}
	
		_mixUnions(structs, true);
	}	

	void _mixUnions(const QList<NUnionChangedStruct>& structs, bool redo)
	{
		for (const NUnionChangedStruct& changeStruct : structs)
		{
			ModelN* model = changeStruct.model;
			if (changeStruct.rotateActive)
			{
				model->updateDisplayRotation(redo);

				QQuaternion q = redo ? changeStruct.rotateChange.end : changeStruct.rotateChange.start;
				setModelRotation(model, q, false);
			}

			if (changeStruct.scaleActive)
			{
				QVector3D scale = redo ? changeStruct.scaleChange.end : changeStruct.scaleChange.start;
				setModelScale(model, scale, false);
			}

			if (changeStruct.posActive)
			{
				QVector3D pos = redo ? changeStruct.posChange.end : changeStruct.posChange.start;
				setModelPosition(model, pos, false);
			}

			updateModel(model);
		}

		checkModelRange();
		checkBedRange();
	}

	void rotateModelOnPlane(ModelN* model, QQuaternion& qq)
	{
		QQuaternion oldRotate = model->localQuaternion();
		QQuaternion q = oldRotate;
		QVector3D oldCenter = model->globalSpaceBox().center();

		q = qq * q;
		model->setLocalQuaternion(q);

		qtuser_3d::Box3D box = model->globalSpaceBox();
		QVector3D offset = oldCenter - box.center();
		QVector3D zoffset = QVector3D(offset.x(), offset.y(), -box.min.z());
		QVector3D localPosition = model->localPosition();
		QVector3D newPosition = localPosition + zoffset;

		mixTRModel(model, localPosition, newPosition, oldRotate, q, true);
	}

	void fillChangeStructs(QList<NUnionChangedStruct>& changes, bool start)
	{
		QList<ModelN*> selections = selectionms();
		if (start)
		{
			changes.clear();
			for (ModelN* model : selections)
			{
				NUnionChangedStruct change;
				change.posActive = true;
				change.scaleActive = true;
				change.rotateActive = true;
				change.model = model;
				change.posChange.start = model->localPosition();
				change.rotateChange.start = model->localQuaternion();
				change.scaleChange.start = model->localScale();
				changes.push_back(change);
			}
		}
		else
		{
			QList<NUnionChangedStruct> _changes;
			for (const NUnionChangedStruct& change : changes)
			{
				if (selections.contains(change.model))
				{
					NUnionChangedStruct c = change;
					c.posChange.end = change.model->localPosition();
					c.rotateChange.end = change.model->localQuaternion();
					c.scaleChange.end = change.model->localScale();
					_changes.push_back(c);
				}
			}
			changes.swap(_changes);
		}
	}

	void mirrorX(ModelN* model, bool reversible)
	{
		if (!model)
			return;

		_beforeMirror();

		if (reversible)
		{
			QList<NMirrorStruct> mirrorStructs;

			NMirrorStruct mirrorStruct;
			mirrorStruct.model = model;
			mirrorStruct.type = 1;

			mirrorStructs.push_back(mirrorStruct);
			__mirror(mirrorStructs);
			return;
		}

		_mirrorX(model, true);
	}

	void mirrorY(ModelN* model, bool reversible)
	{
		if (!model)
			return;

		_beforeMirror();
		if (reversible)
		{
			QList<NMirrorStruct> mirrorStructs;

			NMirrorStruct mirrorStruct;
			mirrorStruct.model = model;
			mirrorStruct.type = 2;

			mirrorStructs.push_back(mirrorStruct);
			__mirror(mirrorStructs);
			return;
		}

		_mirrorY(model, true);
	}

	void mirrorZ(ModelN* model, bool reversible)
	{
		if (!model)
			return;

		_beforeMirror();
		if (reversible)
		{
			QList<NMirrorStruct> mirrorStructs;

			NMirrorStruct mirrorStruct;
			mirrorStruct.model = model;
			mirrorStruct.type = 3;

			mirrorStructs.push_back(mirrorStruct);
			__mirror(mirrorStructs);
			return;
		}

		_mirrorZ(model, true);
	}

	void mirrorReset(ModelN* model, bool reversible)
	{
		if (!model)
			return;

		_beforeMirror();
		if (reversible)
		{
			QList<NMirrorStruct> mirrorStructs;

			NMirrorStruct mirrorStruct;
			mirrorStruct.model = model;
			mirrorStruct.type = 0;
			mirrorStruct.end = QMatrix4x4();
			mirrorStruct.start = model->mirrorMatrix();
			mirrorStructs.push_back(mirrorStruct);
			__mirror(mirrorStructs);
			return;
		}

		QMatrix4x4 m;
		_mirrorSet(model, m, true);
	}

	//void showModelFace(ModelN* model)
	//{
	//	model->showModelFace();
	//	requestVisUpdate(true);
	//}

	//void hideModelFace(ModelN* model)
	//{
	//	model->hideModelFace();
	//	requestVisUpdate(true);
	//}

	//void showModelLine(ModelN* model)
	//{
	//	//model->showModelLine();
	//	requestVisUpdate();
	//}

	//void hideModelLine(ModelN* model)
	//{
	//	//model->hideModelLine();
	//	requestVisUpdate();
	//}

	void setModelNozzle(ModelN* model, int nozzle)
	{
		if (model != nullptr)
		{
			model->setNozzle(nozzle);
			requestVisUpdate(true);
		}
	}

	void setModelShowType(ModelN* model, int showtype)
	{
		model->setShowType(showtype);
	}

	void _setModelVisible(ModelN* model, bool visible)
	{
		model->setVisibility(visible);
	}

	void _addModel2Group(ModelN* model, ModelGroup* group, bool update)
	{
		if (spFunAddModel != nullptr)
		{
			(*spFunAddModel)(group, model);
		}
		//getKernelUI()->treeModel()->addModel(group, model);
		group->addModel(model);

		tracePickable(model);

		checkModelRange();
		checkBedRange();
		if(update)
			requestVisUpdate(true);

		dirtyModelSpace();
	}

	void _removeModelFromGroup(ModelN* model, ModelGroup* group, bool update)
	{
		if (spFunDelModel != nullptr)
		{
			(*spFunDelModel)(model);
		}
		//getKernelUI()->treeModel()->delModel(model);

		group->removeModel(model);
		unTracePickable(model);

		if(update)
			requestVisUpdate(true);

		dirtyModelSpace();
	}

	void _addGroup(ModelGroup* group)
	{
		ModelSpace* space = getKernel()->modelSpace();
		
		//getKernelUI()->treeModel()->addGroup(space->modelGroups().size(), group);
		if (spFunAddGroup != nullptr)
		{
			(*spFunAddGroup)(space->modelGroups().size(), group);
		}
		space->addModelGroup(group);

		checkModelRange();
		checkBedRange();
		requestVisUpdate(true);

		dirtyModelSpace();
	}

	void _removeGroup(ModelGroup* group)
	{
		ModelSpace* space = getKernel()->modelSpace();

		space->removeModelGroup(group);

		requestVisUpdate(true);

		dirtyModelSpace();
	}

	void _replaceModelMesh(ModelN* model, TriMeshPtr mesh, const QString& name, trimesh::TriMesh* aux_mesh, int pos)
	{
		model->replaceTrimesh(mesh);
		model->setModelName(name);

		if (aux_mesh && pos >= 0)
		{
			const trimesh::xform xf;
			model->setAuxiliaryMesh(pos, *aux_mesh, xf);
		}

		requestVisUpdate(true);

		dirtyModelSpace();
	}

	void setModelPosition(ModelN* model, const QVector3D& end, bool update)
	{
		model->setLocalPosition(end, update);

		if (update)
		{
			selectedPickableInfoChanged(model);
			requestVisUpdate(true);
		}

		dirtyModelSpace();
	}

	void setModelRotation(ModelN* model, const QQuaternion& end, bool update)
	{
		model->setLocalQuaternion(end, update);

		//model->setDLPNeedRebuild();
		if (update)
		{
			selectedPickableInfoChanged(model);
			requestVisUpdate(true);
		}

		dirtyModelSpace();
	}

	void setModelScale(ModelN* model, const QVector3D& end, bool update)
	{
		model->setLocalScale(end, update);

		if (update)
		{
			selectedPickableInfoChanged(model);
			requestVisUpdate(true);
		}

		dirtyModelSpace();
	}

	void updateModel(ModelN* model)
	{
		model->updateMatrix();

		selectedPickableInfoChanged(model);
		requestVisUpdate(true);
	}

	void _requestUpdate()
	{
		requestVisUpdate(true);
	}

	void _beforeMirror()
	{

	}

	void _mirrorX(ModelN* model, bool update)
	{
		model->mirrorX();

		if(update)
			requestVisUpdate(true);

		dirtyModelSpace();
	}

	void _mirrorY(ModelN* model, bool update)
	{
		model->mirrorY();

		if (update)
			requestVisUpdate(true);

		dirtyModelSpace();
	}

	void _mirrorZ(ModelN* model, bool update)
	{
		model->_mirrorZ();

		if (update)
			requestVisUpdate(true);

		dirtyModelSpace();
	}

	void _mirrorSet(ModelN* model, const QMatrix4x4& matrix, bool update)
	{
		model->mirrorSet(matrix);

		if (update)
			requestVisUpdate(true);

		dirtyModelSpace();
	}

	CXFileOpenAndSaveManager& IOManager()
	{
		return *getKernel()->ioManager();
	}
}

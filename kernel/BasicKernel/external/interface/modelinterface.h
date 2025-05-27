#ifndef CREATIVE_KERNEL_MODELINTERFACE_1592788083031_H
#define CREATIVE_KERNEL_MODELINTERFACE_1592788083031_H
#include "basickernelexport.h"
#include "data/trimeshheader.h"
#include "data/undochange.h"
#include <QtCore/QList>

class CXFileOpenAndSaveManager;
namespace qtuser_core
{
	class Progressor;
}

namespace creative_kernel
{
	class ModelN;
	class ModelGroup;

	typedef void (*UIFunIO)(int index, QObject* group);
	typedef void (*UIFunOO)(QObject* group, QObject* model);
	typedef void (*UIFunOi)(QModelIndex index);
	typedef void (*UIFunO)(QObject* model);

	BASIC_KERNEL_API void setAddModelFun(UIFunOO fun);
	BASIC_KERNEL_API void setDelModelFun(UIFunO fun);
	BASIC_KERNEL_API void setAddGroupFun(UIFunIO fun);

	BASIC_KERNEL_API void callAddModelFun(QObject* group, QObject* model);
	BASIC_KERNEL_API void callDelModelFun(QObject* model);
	BASIC_KERNEL_API void callAddGroupFun(int index, QObject* group);


	BASIC_KERNEL_API void addModel(ModelN* model, bool reversible = false, qtuser_core::Progressor* progressor = nullptr);
	BASIC_KERNEL_API void addModel2Group(ModelN* model, ModelGroup* group, bool reversible = false);
	BASIC_KERNEL_API void addGroup(ModelGroup* group, bool reversible = false);
	BASIC_KERNEL_API void modifyGroup(QList<ModelN*>& removes, QList<ModelN*>& adds, ModelGroup* group, bool reversible = false);
	BASIC_KERNEL_API void removeAllModel(bool reversible = false);
	BASIC_KERNEL_API void removeSelectionModels();
	BASIC_KERNEL_API void replaceModelsMesh(const QList<ModelN*>& models, const QList<TriMeshPtr>& meshs,
		const QList<QString>& names, bool reversible = false, const QList<TriMeshPtr>* aux_meshes = nullptr, QList<int>* aux_pos = nullptr);

	BASIC_KERNEL_API void moveModel(ModelN* model, const QVector3D& start, const QVector3D& end, bool reversible = false, bool needcheck = true);
	BASIC_KERNEL_API void mixTSModel(ModelN* model, const QVector3D& tstart, const QVector3D& tend,
		const QVector3D& sstart, const QVector3D& send, bool reversible = false, bool needcheck = true);
	BASIC_KERNEL_API void mixTRModel(ModelN* model, const QVector3D& tstart, const QVector3D& tend,
		const QQuaternion& rstart, const QQuaternion& rend, bool reversible = false, bool needcheck = true);

	BASIC_KERNEL_API void mixUnions(const QList<NUnionChangedStruct>& structs, bool reversible = false);
					 void _mixUnions(const QList<NUnionChangedStruct>& structs, bool redo);

	BASIC_KERNEL_API void rotateModelOnPlane(ModelN* model, QQuaternion& q);
	BASIC_KERNEL_API void fillChangeStructs(QList<NUnionChangedStruct>& changes, bool start);

	BASIC_KERNEL_API void mirrorX(ModelN* model, bool reversible = false);
	BASIC_KERNEL_API void mirrorY(ModelN* model, bool reversible = false);
	BASIC_KERNEL_API void mirrorZ(ModelN* model, bool reversible = false);
	BASIC_KERNEL_API void mirrorReset(ModelN* model, bool reversible = false);

	//BASIC_KERNEL_API void showModelFace(ModelN* model);
	//BASIC_KERNEL_API void hideModelFace(ModelN* model);

	//BASIC_KERNEL_API void showModelLine(ModelN* model);
	//BASIC_KERNEL_API void hideModelLine(ModelN* model);

	BASIC_KERNEL_API void setModelNozzle(ModelN* model, int nozzle);

	BASIC_KERNEL_API void setModelShowType(ModelN* model, int showtype);

	BASIC_KERNEL_API void setModelRotation(ModelN* model, const QQuaternion& end, bool update = false);
	BASIC_KERNEL_API void setModelScale(ModelN* model, const QVector3D& end, bool update = false);
	BASIC_KERNEL_API void setModelPosition(ModelN* model, const QVector3D& end, bool update = false);

	BASIC_KERNEL_API void updateModel(ModelN* model);


	void _setModelVisible(ModelN* model, bool visible);
	void _addModel2Group(ModelN* model, ModelGroup* group, bool update = false);
	void _removeModelFromGroup(ModelN* model, ModelGroup* group, bool update = false);
	void _addGroup(ModelGroup* group);
	void _removeGroup(ModelGroup* group);

	void _replaceModelMesh(ModelN* model, TriMeshPtr mesh, const QString& name, trimesh::TriMesh* aux_mesh = nullptr, int pos = -1);

	void _requestUpdate();

	void _beforeMirror();
	void _mirrorX(ModelN* model, bool update = false);
	void _mirrorY(ModelN* model, bool update = false);
	void _mirrorZ(ModelN* model, bool update = false);
	void _mirrorSet(ModelN* model, const QMatrix4x4& matrix, bool update = false);



	BASIC_KERNEL_API CXFileOpenAndSaveManager& IOManager();
}

#define CXFILE creative_kernel::IOManager()
#endif // CREATIVE_KERNEL_MODELINTERFACE_1592788083031_H
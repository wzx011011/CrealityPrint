#ifndef CREATIVE_KERNEL_MODELSPACEUNDO_1592743196335_H
#define CREATIVE_KERNEL_MODELSPACEUNDO_1592743196335_H
#include "basickernelexport.h"
#include <QtWidgets/QUndoStack>
#include "data/modeln.h"
#include "data/modelgroup.h"
#include "data/undochange.h"

namespace creative_kernel
{
	class BASIC_KERNEL_API ModelSpaceUndo : public QUndoStack
	{
		Q_OBJECT
	public:
		ModelSpaceUndo(QObject* parent = nullptr);
		virtual ~ModelSpaceUndo();

		void addModels2Group(ModelGroup* group, QList<ModelN*>& models, QList<ModelN*>& removeModels);
		void addGroups(QList<ModelGroup*>& groups);
		void replaceModels(const QList<ModelN*>& models, const QList<TriMeshPtr>& meshs,
			const QList<QString>& names, const QList<TriMeshPtr>* aux_meshes = nullptr, QList<int>* aux_pos = nullptr);

		void mix(const QList<NUnionChangedStruct>& mixChange);
		void mirror(const QList<NMirrorStruct>& mirrors);
	};
}
#endif // CREATIVE_KERNEL_MODELSPACEUNDO_1592743196335_H
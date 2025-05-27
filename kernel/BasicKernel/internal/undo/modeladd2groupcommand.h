#ifndef CREATIVE_KERNEL_MODELADD2GROUPCOMMAND_1592790419297_H
#define CREATIVE_KERNEL_MODELADD2GROUPCOMMAND_1592790419297_H
#include "basickernelexport.h"
#include <QtWidgets/QUndoCommand>
#include <Qt3DCore/QNode>
#include "external/data/modeln.h"
#include "external/data/modelgroup.h"

namespace creative_kernel
{
	class BASIC_KERNEL_API ModelAdd2GroupCommand : public Qt3DCore::QNode, public QUndoCommand
	{
		Q_OBJECT
	public:
		ModelAdd2GroupCommand(Qt3DCore::QNode* parent = nullptr);
		virtual ~ModelAdd2GroupCommand();

		void undo() override;
		void redo() override;

		void setReverse(bool reverse);
		void setModels(const QList<ModelN*>& models);
		void setRemoveModels(const QList<ModelN*>& models);
		void setGroup(ModelGroup* group);
	private:
		void deal(bool remove);
	protected:
		QList<ModelN*> m_modelList;
		QList<ModelN*> m_modelRemoveList;
		ModelGroup* m_group;

		bool m_reverse;
	};
}
#endif // CREATIVE_KERNEL_MODELADD2GROUPCOMMAND_1592790419297_H
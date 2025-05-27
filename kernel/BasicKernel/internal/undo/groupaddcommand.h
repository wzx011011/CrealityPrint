#ifndef CREATIVE_KERNEL_GROUPADDCOMMAND_1592792531804_H
#define CREATIVE_KERNEL_GROUPADDCOMMAND_1592792531804_H
#include "basickernelexport.h"
#include <QtWidgets/QUndoCommand>
#include <Qt3DCore/QNode>
#include "external/data/modelgroup.h"

namespace creative_kernel
{
	class BASIC_KERNEL_API GroupAddCommand : public Qt3DCore::QNode, public QUndoCommand
	{
		Q_OBJECT
	public:
		GroupAddCommand(Qt3DCore::QNode* parent = nullptr);
		virtual ~GroupAddCommand();

		void undo() override;
		void redo() override;

		void setReverse(bool reverse);
		void setGroups(QList<ModelGroup*> groups);
	private:
		void deal(bool remove);
	protected:
		QList<ModelGroup*> m_groups;

		bool m_reverse;
	};
}
#endif // CREATIVE_KERNEL_GROUPADDCOMMAND_1592792531804_H
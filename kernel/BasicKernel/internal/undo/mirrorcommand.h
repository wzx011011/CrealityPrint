#ifndef CREATIVE_KERNEL_MIRRORCOMMAND_1592874176854_H
#define CREATIVE_KERNEL_MIRRORCOMMAND_1592874176854_H
#include "basickernelexport.h"
#include <QtWidgets/QUndoCommand>
#include "external/data/modeln.h"
#include "data/undochange.h"

namespace creative_kernel
{
	class BASIC_KERNEL_API MirrorCommand : public QObject, public QUndoCommand
	{
		Q_OBJECT
	public:
		MirrorCommand(QObject* parent = nullptr);
		virtual ~MirrorCommand();

		void setMirrors(const QList<NMirrorStruct>& mirrorStructs);
	protected:
		void undo() override;
		void redo() override;
	private:
		void call(bool reverse);
	protected:
		QList<NMirrorStruct> m_mirrors;
	};
}
#endif // CREATIVE_KERNEL_MIRRORCOMMAND_1592874176854_H
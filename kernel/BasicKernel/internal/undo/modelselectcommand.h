#ifndef _creative_kernel_MODELSELECTCOMMAND_1589331952870_H
#define _creative_kernel_MODELSELECTCOMMAND_1589331952870_H
#include "basickernelexport.h"
#include <QtWidgets/QUndoCommand>

namespace creative_kernel
{
	class ModelSelectCommand: public QUndoCommand
	{
	public:
		ModelSelectCommand();
		virtual ~ModelSelectCommand();

		void undo() override;
		void redo() override;
	protected:
	};
}
#endif // _creative_kernel_MODELSELECTCOMMAND_1589331952870_H

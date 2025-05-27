#include "undointerface.h"
#include "external/kernel/kernel.h"
#include "external/data/modelspaceundo.h"

namespace creative_kernel
{
	UndoProxy* getUndoProxy()
	{
		return getKernel()->undoProxy();
	}

	void bindUndoStack(QUndoStack* undoStack)
	{
		getKernel()->undoProxy()->setUndoStack(undoStack);
	}

	void bindModelSpaceUndoStack()
	{
		//UndoProxy* proxy = getKernel()->undoProxy();
		//ModelSpaceUndo* stack = getKernel()->modelSpaceUndo();
		//proxy->setUndoStack(stack);
	}

	void undo()
	{
		//getKernel()->undoProxy()->undo();
		getKernel()->undoGroup()->undo();
	}

	void redo()
	{
		//getKernel()->undoProxy()->redo();
		getKernel()->undoGroup()->redo();
	}
}
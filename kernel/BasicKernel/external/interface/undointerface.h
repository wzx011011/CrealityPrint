#ifndef CREATIVE_KERNEL_UNDOINTERFACE_1592736512631_H
#define CREATIVE_KERNEL_UNDOINTERFACE_1592736512631_H
#include "basickernelexport.h"
#include "qtuser3d/undo/undoproxy.h"

namespace creative_kernel
{
	BASIC_KERNEL_API UndoProxy* getUndoProxy();

	BASIC_KERNEL_API void bindUndoStack(QUndoStack* undoStack);
	BASIC_KERNEL_API void bindModelSpaceUndoStack();

	BASIC_KERNEL_API void undo();
	BASIC_KERNEL_API void redo();
}

//this interface can get undoproxy, undoproxy hold a undoStack one time, 
//undoproxy is associate with UI redo and undo button
//these objects are always available in plugins 
#endif // CREATIVE_KERNEL_UNDOINTERFACE_1592736512631_H
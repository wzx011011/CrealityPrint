#ifndef CREATIVE_KERNEL_UIINTERFACE_1592721604112_H
#define CREATIVE_KERNEL_UIINTERFACE_1592721604112_H
#include "creativekernelexport.h"

class ToolCommand;
namespace creative_kernel
{
	CREATIVE_KERNEL_API void addLCommand(ToolCommand* command);
	CREATIVE_KERNEL_API void removeLCommand(ToolCommand* command);

	CREATIVE_KERNEL_API QObject* getUIRoot();
	CREATIVE_KERNEL_API QObject* getUIAppWindow();
	CREATIVE_KERNEL_API QObject* getFooter();

    CREATIVE_KERNEL_API QObject* getUIRightPanel();

    CREATIVE_KERNEL_API QObject* getGLMainView();


	void modelfaceAddGroup(int index, QObject* group);
	void modelfaceAddModel(QObject* group, QObject* model);
	void modelfaceDelModel(QObject* model);


}
#endif // CREATIVE_KERNEL_UIINTERFACE_1592721604112_H

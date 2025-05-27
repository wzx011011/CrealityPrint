#include "uiinterface.h"
#include "kernel/kernelui.h"
#include "qtuserqml/plugin/toolcommandcenter.h"
#include "qtuserqml/plugin/treemodel.h"
#include "slice/cusModelListModel.h"

namespace creative_kernel
{
	void addLCommand(ToolCommand* command)
	{
		getKernelUI()->lCenter()->addCommand(command);
	}

	void removeLCommand(ToolCommand* command)
	{
		getKernelUI()->lCenter()->removeCommand(command);
	}

	QObject* getUIRoot()
	{
		return getKernelUI()->root();
	}

	QObject* getUIAppWindow()
	{
		return getKernelUI()->appWindow();
	}
    QObject* getUIRightPanel()
    {
        return getKernelUI()->rightPanel();
    }

	QObject* getFooter()
	{
		return getKernelUI()->footer();
	}

    QObject* getGLMainView()
    {
        return getKernelUI()->glMainView();
    }

	void modelfaceAddGroup(int index, QObject* group)
	{
        //getKernelUI()->treeModel()->addGroup(index, group);
	}

	void modelfaceAddModel(QObject* group, QObject* model)
	{
        Q_UNUSED(group)
        getKernelUI()->listModel()->addItem(model);
        //getKernelUI()->treeModel()->addModel(model, group);

        QObject* pZsliderObj = AbstractKernelUI::getSelf()->getUI("UIRoot")->findChild<QObject*>("zsliderObj");
        if(pZsliderObj) pZsliderObj->setProperty("flag", true);

        getKernelUI()->visibleAll(true);
        getKernelUI()->leftToolbar()->setProperty("enabled", QVariant::fromValue(true));
	}

	void modelfaceDelModel(QObject* model)
	{
        getKernelUI()->listModel()->delItem(model);
        //getKernelUI()->treeModel()->delModel(model);

        if (!getKernelUI()->listModel()->itemCount())
		{
            QObject* pZsliderObj = AbstractKernelUI::getSelf()->getUI("UIRoot")->findChild<QObject*>("zsliderObj");
            if(pZsliderObj) pZsliderObj->setProperty("flag", false);

            getKernelUI()->visibleAll(false);
			getKernelUI()->leftToolbar()->setProperty("enabled", QVariant::fromValue(false));
		}		    
	}
}

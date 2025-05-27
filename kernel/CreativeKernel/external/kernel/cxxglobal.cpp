#include "cxxglobal.h"

#include "interface/selectorinterface.h"
#include "interface/uiinterface.h"
#include "interface/visualsceneinterface.h"

#include "kernelui.h"
#include "interface/spaceinterface.h"
#include <qobject.h>
#include "interface/gadatainterface.h"
using namespace creative_kernel;
CXXGlobal::CXXGlobal(QObject* parent)
	:QObject(parent)
{

}

CXXGlobal::~CXXGlobal()
{

}

void CXXGlobal::previewMain()
{
	//---by TCJ
	QList<creative_kernel::ModelN*> selections = selectionms();
	bool flag = (selections.size() == 0) ? false : true;
	if (flag)
	{
		AbstractKernelUI::getSelf()->switchShowPop(true);
		AbstractKernelUI::getSelf()->switchPickMode();
	}
	else
	{
		//如果平台上有模型，默认选择一个
		QList<ModelN*> models = modelns();
		if (models.size() > 0)
		{
			ModelN* pickable = models.at(0);
			selectOne((qtuser_3d::Pickable*)pickable);
			AbstractKernelUI::getSelf()->switchShowPop(true);
			AbstractKernelUI::getSelf()->switchPickMode();
		}		
	}
	//---
	setVisOperationMode(nullptr);
	enableSupportMode(false);
	creative_kernel::sendDataToGA("Configuration Profile", "Printing Profile (Icon)");
}
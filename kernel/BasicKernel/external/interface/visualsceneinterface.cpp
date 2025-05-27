#include "visualsceneinterface.h"
#include "external/kernel/kernel.h"
#include "external/kernel/visualscene.h"
#include "qtuser3d/module/facepicker.h"

namespace creative_kernel
{
	void setVisOperationMode(qtuser_3d::SceneOperateMode* operateMode)
	{
		getKernel()->visScene()->setOperateMode(operateMode);
	}

	void onMachineSelectChange()
	{
		getKernel()->visScene()->onMachineSelectChange();
	}

	void visShow(Qt3DCore::QEntity* entity)
	{
		getKernel()->visScene()->show(entity);
	}

	void visHide(Qt3DCore::QEntity* entity)
	{
		getKernel()->visScene()->hide(entity);
	}

	void enableVisHandlers(bool enabled)
	{
		getKernel()->visScene()->enableHandler(enabled);
	}

	void requestVisUpdate(bool capture)
	{
		VisualScene* visScene = getKernel()->visScene();
		visScene->updateRender(capture);
	}

	qtuser_3d::FacePicker* visPickSource()
	{
		return getKernel()->visScene()->facePicker();
	}

	bool visPick(const QPoint& point, int* faceID)
	{
		return visPickSource()->pick(point, faceID);
	}

}
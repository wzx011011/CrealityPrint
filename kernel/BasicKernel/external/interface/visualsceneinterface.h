#ifndef CREATIVE_KERNEL_VISUALSCENEINTERFACE_1592728018928_H
#define CREATIVE_KERNEL_VISUALSCENEINTERFACE_1592728018928_H
#include "basickernelexport.h"
#include <Qt3DCore/QEntity>

namespace qtuser_3d
{
	class SceneOperateMode;
	class FacePicker;
}

namespace creative_kernel
{
	BASIC_KERNEL_API void setVisOperationMode(qtuser_3d::SceneOperateMode* operateMode);
	BASIC_KERNEL_API void visShow(Qt3DCore::QEntity* entity);
	BASIC_KERNEL_API void visHide(Qt3DCore::QEntity* entity);
	BASIC_KERNEL_API void enableVisHandlers(bool enabled);

	BASIC_KERNEL_API void onMachineSelectChange();

	BASIC_KERNEL_API void requestVisUpdate(bool capture = false);
	BASIC_KERNEL_API qtuser_3d::FacePicker* visPickSource();
	BASIC_KERNEL_API bool visPick(const QPoint& point, int* faceID);
}
#endif // CREATIVE_KERNEL_VISUALSCENEINTERFACE_1592728018928_H
#include "camerainterface.h"
#include "qtuser3d/camera/screencamera.h"
#include "external/kernel/kernel.h"
#include "external/kernel/reuseablecache.h"

using namespace qtuser_3d;
namespace creative_kernel
{
	CameraController* cameraController()
	{
		return getKernel()->cameraController();
	}

	ScreenCamera* visCamera()
	{
		return getKernel()->reuseableCache()->mainScreenCamera();
	}

	Ray visRay(const QPoint& point)
	{
		return getKernel()->reuseableCache()->mainScreenCamera()->screenRay(point);
	}

	float cameraScreenSpaceRatio(QVector3D position)
	{
		return visCamera()->screenSpaceRatio(position);
	}
}
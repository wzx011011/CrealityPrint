#include "printerentityupdater.h"

#include "interface/reuseableinterface.h"
#include "interface/renderinterface.h"
#include "interface/camerainterface.h"
#include "interface/spaceinterface.h"

#include "qtuser3d/entity/printerentity.h"

namespace creative_kernel
{
	PrinterEntityUpdater::PrinterEntityUpdater(QObject* parent)
		:QObject(parent)
	{
	}
	
	PrinterEntityUpdater::~PrinterEntityUpdater()
	{
	}

	void PrinterEntityUpdater::onBoxChanged(qtuser_3d::Box3D& box)
	{
		visCamera()->fittingBoundingBox(box);

		QVector3D machineCube = box.size();

		qtuser_3d::PrinterEntity* entity = getCachedPrinterEntity();
		entity->updateBox(box);

		renderOneFrame();
	}

	void PrinterEntityUpdater::onCameraChanged(qtuser_3d::ScreenCamera* screenCamera)
	{
		qtuser_3d::PrinterEntity* entity = getCachedPrinterEntity();
		entity->onCameraChanged(screenCamera);

		Qt3DRender::QCamera* camera = screenCamera->camera();
		if (camera)
		{
			QVector3D viewDir = camera->viewVector();
			viewDir.normalize();
			QVector3D up = QVector3D(0.0f, 0.0f, 1.0f);
			float a = 0.5f - 0.625f * QVector3D::dotProduct(viewDir, up);
			if (a > 0.5f)
				a = 0.5f;
			else if (a < 0.0f)
				a = 0.0f;

			QVector4D color = QVector4D(0.2275f, 0.2275f, 0.2353f, a);
			setZProjectColor(color);
		}
	}
}
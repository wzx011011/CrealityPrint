#ifndef CREATIVE_KERNEL_PRINTERENTITYUPDATER_1594459630592_H
#define CREATIVE_KERNEL_PRINTERENTITYUPDATER_1594459630592_H
#include "basickernelexport.h"
#include "modelspaceobserver.h"
#include "qtuser3d/camera/screencamera.h"

namespace creative_kernel
{
	class BASIC_KERNEL_API PrinterEntityUpdater : public QObject
		, public SpaceTracer
		, public qtuser_3d::ScreenCameraObserver
	{
		Q_OBJECT
	public:
		PrinterEntityUpdater(QObject* parent = nullptr);
		virtual ~PrinterEntityUpdater();

	protected:
		void onBoxChanged(qtuser_3d::Box3D& box) override;
        void onSceneChanged(qtuser_3d::Box3D &box) override {}
		void onGlobalBoxChanged(qtuser_3d::Box3D& box) override {}
		void onCameraChanged(qtuser_3d::ScreenCamera* camera) override;
	};
}
#endif // CREATIVE_KERNEL_PRINTERENTITYUPDATER_1594459630592_H

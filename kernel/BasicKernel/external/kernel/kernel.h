#ifndef _CREATIVE_BASE_UI_KERNEL_1589720896794_H
#define _CREATIVE_BASE_UI_KERNEL_1589720896794_H
#include "basickernelexport.h"
#include "qtusercore/module/singleton.h"
#include <QtWidgets/QUndoStack>
#include <QUndoGroup>

class UndoProxy;
class CameraController;
class CXFileOpenAndSaveManager;

namespace qtuser_3d
{
	class ScreenCamera;
}

namespace qtuser_core
{
	class JobExecutor;
}
namespace creative_kernel
{
	class Tips;
	class VisualScene;
	class ModelSpaceUndo;
	class ModelSpace;
	class ReuseableCache;
	class PrinterEntityUpdater;
	class ModelNSelector;
	class MachineSelector;
	class PrinterNetMng;

	class BASIC_KERNEL_API Kernel: public QObject
	{
		SINGLETON_DECLARE(Kernel)
		Q_OBJECT
	public:
		virtual ~Kernel();

		void intialize();
		void uninitialize();

		ReuseableCache* reuseableCache();


		CameraController* cameraController();
		CXFileOpenAndSaveManager* ioManager();

		PrinterNetMng* printerNetMng();

		ModelSpace* modelSpace();
		ModelNSelector* modelSelector();
		MachineSelector* machineSelector();

		UndoProxy* undoProxy();

		QUndoGroup* undoGroup();

		VisualScene* visScene();

		qtuser_core::JobExecutor* jobExecutor();
		Tips* tips();

		ModelSpaceUndo* modelSpaceUndo();
	public slots:
		void viewChanged(bool requestUpdate);
	protected:
		//external
		ReuseableCache* m_reusableCache;
		ModelSpace* m_modelSpace;
		//internal
		PrinterEntityUpdater* m_printerUpdater;

		PrinterNetMng* m_printerNetMng;

		UndoProxy* m_undoProxy;
		CameraController* m_cameraController;

		qtuser_core::JobExecutor* m_jobExecutor;
		Tips* m_tips;

		VisualScene* m_visualScene;
		ModelNSelector* m_modelSelector;
		MachineSelector* m_machineSelector;

		ModelSpaceUndo* m_modelSpaceUndo;
		QUndoGroup* m_undoGroup;
		CXFileOpenAndSaveManager* m_ioManager;

		static Kernel* sKernel;
	};

	SINGLETON_EXPORT(BASIC_KERNEL_API, Kernel)
}

using namespace creative_kernel;
#endif // _CREATIVE_BASE_UI_KERNEL_1589720896794_H

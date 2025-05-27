#include "kernel.h"

#include "data/modelspace.h"
#include "kernel/tips.h"
#include "kernel/modelnselector.h"
#include "kernel/machineselector.h"
#include "qtusercore/module/systemutil.h"

#include "qtuser3d/camera/cameracontroller.h"
#include "qtuser3d/undo/undoproxy.h"
#include "qtusercore/module/jobexecutor.h"

#include "kernel/visualscene.h"
#include "data/modelspaceundo.h"
#include "interface/renderinterface.h"
#include "interface/eventinterface.h"
#include "interface/spaceinterface.h"

#include "kernel/reuseablecache.h"

#include "internal/utils/printerentityupdater.h"
#include "utils/printernetmng.h"
#include "inout/cxopenandsavefilemanager.h"

using namespace qtuser_3d;
namespace creative_kernel
{
	SINGLETON_IMPL(Kernel)
	Kernel::Kernel()
		: m_undoProxy(nullptr)
		, m_cameraController(nullptr)
		, m_jobExecutor(nullptr)
		, m_tips(nullptr)
		, m_visualScene(nullptr)
	{
		m_modelSpace = new ModelSpace(this);

		m_cameraController = new CameraController(this);
		m_ioManager = new CXFileOpenAndSaveManager(this);

		m_undoProxy = new UndoProxy(this);

		m_jobExecutor = new qtuser_core::JobExecutor(this);
		m_tips = new Tips(this);

		m_visualScene = new VisualScene();

		m_modelSelector = new ModelNSelector(this);
		m_modelSelector->setPickerSource(m_visualScene->facePicker());

		m_machineSelector = new MachineSelector(this);

		m_undoGroup = new QUndoGroup(this);

		m_modelSpaceUndo = new ModelSpaceUndo(m_undoGroup);

		m_reusableCache = new ReuseableCache(this);

		m_printerUpdater = new PrinterEntityUpdater(this);

		m_printerNetMng = new PrinterNetMng(this);
	}

	Kernel::~Kernel()
	{
		if (m_undoProxy != nullptr)
		{
			delete m_undoProxy;
			m_undoProxy = nullptr;
		}
		if (m_cameraController != nullptr)
		{
			delete m_cameraController;
			m_cameraController = nullptr;
		}
		if (m_visualScene != nullptr)
		{
			delete m_visualScene;
			m_visualScene = nullptr;
		}
		if (m_modelSpace != nullptr)
		{
			delete m_modelSpace;
			m_modelSpace = nullptr;
		}
		if (m_printerNetMng != nullptr)
		{
			delete m_printerNetMng;
			m_printerNetMng = nullptr;
		}
	}

	ModelSpace* Kernel::modelSpace()
	{
		return m_modelSpace;
	}

	ModelNSelector* Kernel::modelSelector()
	{
		return m_modelSelector;
	}

	MachineSelector* Kernel::machineSelector()
	{
		return m_machineSelector;
	}

	UndoProxy* Kernel::undoProxy()
	{
		return m_undoProxy;
	}

	QUndoGroup* Kernel::undoGroup()
	{
		return m_undoGroup;
	}

	CameraController* Kernel::cameraController()
	{
		return m_cameraController;
	}

	CXFileOpenAndSaveManager* Kernel::ioManager()
	{
		return m_ioManager;
	}

	PrinterNetMng* Kernel::printerNetMng()
	{
		return m_printerNetMng;
	}

	void Kernel::intialize()
	{
		qDebug() << "kernel initialize";
		m_reusableCache->registerCache();
		m_cameraController->setScreenCamera(m_reusableCache->mainScreenCamera());

		connect(m_cameraController, SIGNAL(signalViewChanged(bool)), this, SLOT(viewChanged(bool)));

		m_modelSpace->addSpaceTracer(m_printerUpdater);
		m_reusableCache->mainScreenCamera()->addCameraObserver(m_printerUpdater);

		m_modelSelector->disableReverseSelect(true);

		registerRenderGraph(m_visualScene);
		renderDefaultRenderGraph();

		qDebug() << "kernel initialize over";
		showSysMemory();
	}

	void Kernel::uninitialize()
	{
		qDebug() << "kernel uninitialize start";
		m_modelSpace->uninitialize();

		renderRenderGraph(nullptr);
		m_reusableCache->blockRelation();
		m_cameraController->uninitialize();

		qDebug() << "kernel uninitialize over";
	}

	ReuseableCache* Kernel::reuseableCache()
	{
		return m_reusableCache;
	}

	VisualScene* Kernel::visScene()
	{
		return m_visualScene;
	}

	qtuser_core::JobExecutor* Kernel::jobExecutor()
	{
		return m_jobExecutor;
	}

	Tips* Kernel::tips()
	{
		return m_tips;
	}

	ModelSpaceUndo* Kernel::modelSpaceUndo()
	{
		return m_modelSpaceUndo;
	}

	void Kernel::viewChanged(bool requestUpdate)
	{
		if (m_visualScene->frameGraph()->parent())
		{
			m_visualScene->updateRender(requestUpdate);
		}
		else
		{
			renderOneFrame();
		}
	}
}

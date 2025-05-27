#include "slicepreviewflow.h"

#include <QtCore/qmath.h>
#include <Qt3DRender/QLineWidth>

#include <qtuser3d/camera/screencamera.h>
#include"qtuser3d/trimesh2/camera.h"

#include <qtuser3d/framegraph/colorpicker.h>
#include <qtuser3d/framegraph/surface.h>
#include <qtuser3d/framegraph/texturerendertarget.h>
#include <qtuser3d/trimesh2/conv.h>
#include <qtusercore/string/stringtool.h>
#include <qtuser3d/refactor/xentity.h>

#include "external/data/modeln.h"
#include "external/interface/camerainterface.h"
#include "external/interface/eventinterface.h"
#include "external/interface/renderinterface.h"
#include "external/interface/reuseableinterface.h"
#include "external/interface/selectorinterface.h"
#include "external/interface/spaceinterface.h"
#include "external/interface/visualsceneinterface.h"
#include "external/interface/uiinterface.h"
#include "external/slice/sliceattain.h"

#include "internal/render/gcodeprevieweffect.h"
#include "internal/multi_printer/printer.h"
#include "internal/render/slicepreviewnode.h"
#include "internal/render/slicepreviewscene.h"
#include "qtuser3d/module/selector.h"
#include "interface/reuseableinterface.h"
#include "interface/visualsceneinterface.h"
#include "interface/printerinterface.h"
#include "entity/worldindicatorentity.h"
#include "QtCore/qcoreapplication.h"
#include <QElapsedTimer>
#include "interface/appsettinginterface.h"

#include "kernel/kernel.h"
#include "kernel/reuseablecache.h"
#include "sliceattain.h"
#include <QElapsedTimer>

namespace creative_kernel
{
	SlicePreviewFlow::SlicePreviewFlow(Qt3DCore::QNode* parent)
		: QObject(parent)
#if INDEX_START_AT_ONE
		, m_indexOffset(1)
#else
		, m_indexOffset(0)
#endif
		, m_visualType(gcode::GCodeVisualType::gvt_structure)
		, m_speedModel(std::make_unique<cxgcode::GcodeSpeedListModel>())
		, m_structureModel(std::make_unique<cxgcode::GcodeStructureListModel>())
		, m_extruderModel(std::make_unique<cxgcode::GcodeExtruderListModel>())
		, m_extruderTableModel(std::make_unique<cxgcode::GcodeExtruderTableModel>())
		, m_layerHightModel(std::make_unique<cxgcode::GcodeLayerHightListModel>())
		, m_lineWidthModel(std::make_unique<cxgcode::GcodeLineWidthListModel>())
		, m_flowModel(std::make_unique<cxgcode::GcodeFlowListModel>())
		, m_layerTimeModel(std::make_unique<cxgcode::GcodeLayerTimeListModel>())
		, m_fanSpeedModel(std::make_unique<cxgcode::GcodeFanSpeedListModel>())
		, m_temperatureModel(std::make_unique<cxgcode::GcodeTemperatureListModel>())
		, m_accModel(std::make_unique<cxgcode::GcodePreviewRangeDivideModel>(gcode::GCodeVisualType::gvt_acc))

	{
		m_currentLayer = m_indexOffset;
		m_currentStep = m_indexOffset;
		setPosition();

		m_root = new Qt3DCore::QEntity();
		m_scene = new SlicePreviewScene(m_root);

		m_gcodeEffect = new GCodePreviewEffect(GCodePreviewEffect::PolyColor, m_root);
		m_gcodeMonoEffect = new GCodePreviewEffect(GCodePreviewEffect::MonoColor, m_root);

		for (int i = 0; i < (int)Orca::ExtrusionRole::Count; ++i)
		{
			if (i == (int)Orca::ExtrusionRole::Retract ||
				i == (int)Orca::ExtrusionRole::Unretract ||
				i == (int)Orca::ExtrusionRole::Noop ||
				i == (int)Orca::ExtrusionRole::Tool_change ||
				i == (int)Orca::ExtrusionRole::Color_change ||
				i == (int)Orca::ExtrusionRole::Pause_Print ||
				i == (int)Orca::ExtrusionRole::Custom_GCode ||
				i == (int)Orca::ExtrusionRole::Travel ||
				i == (int)Orca::ExtrusionRole::Wipe)
				m_roleState[i] = false;
			else 
				m_roleState[i] = true;
		}

		connect(m_structureModel.get(), &cxgcode::GcodeStructureListModel::itemCheckedChanged,
						this, &SlicePreviewFlow::showGCodeType);
	}

	SlicePreviewFlow::~SlicePreviewFlow()
	{
		delete m_scene;
		delete m_root;
	}

	void SlicePreviewFlow::registerContext() {}

	void SlicePreviewFlow::initialize()
	{
		m_scene->initialize();
		QList<QColor> colors = m_scene->previewNode()->getSpeedColorValues();
		m_speedModel->setColors(colors);
		m_layerHightModel->setColors(colors);
		m_lineWidthModel->setColors(colors);
		m_flowModel->setColors(colors);
		m_layerTimeModel->setColors(colors);
		m_fanSpeedModel->setColors(colors);
		m_temperatureModel->setColors(colors);
		m_accModel->setColors(colors);

		m_requestLayerGCodeTimer.setSingleShot(true);
		connect(&m_requestLayerGCodeTimer, &QTimer::timeout, this, [=]()
			{
				if (m_attain)
				{
					m_attain->requestLayerGcode(m_requestLayer);
					emit layerGCodesChanged();
				}
			});
	}

	SlicePreviewScene* SlicePreviewFlow::scene()
	{
		return m_scene;
	}

	Qt3DCore::QEntity* SlicePreviewFlow::sceneRoot()
	{
		 return m_root;
		//return m_scene;;
	}

	void SlicePreviewFlow::useCachePreview()
	{
		if (m_attain)
        {
            m_attain->saveTempGCode();
            QImage tmpImage = m_cachePreview;
            m_attain->saveTempGCodeWithImage(tmpImage);
        }
	}

	void SlicePreviewFlow::notifyClipValue()
	{
		if (!m_attain)
			return;

		{	//clip
			int firstLayer = m_showOnlyTopLayer ? m_currentLayer : 1;
			int first = m_attain->moveId(firstLayer, 0);
			int last = m_attain->moveId(m_currentLayer, m_currentStep);
			QVector2D clipValue(first, last);
			m_gcodeEffect->setStepLimit(clipValue);
			m_gcodeMonoEffect->setStepLimit(clipValue);
		}

		QVector3D pos = qtuser_3d::vec2qvector(m_attain->traitPosition(m_currentLayer, m_currentStep));
		Printer* printer = getSelectedPrinter();
		if (printer)
			pos += printer->globalBox().min;

		m_scene->setIndicatorPos(pos);

		//renderOneFrame();
	}
	
	void SlicePreviewFlow::notifyLayerRange()
	{
		if (!m_attain)
			return;

		{	//current layer range
			int first = m_attain->moveId(m_currentLayer, 0);
			// int layerLastStep = m_animationState ? m_currentStep : -1;
			int last = m_attain->moveId(m_currentLayer, -1);
			QVector2D range(first, last);
			m_gcodeEffect->setLayerStepRange(range);
			m_gcodeMonoEffect->setLayerStepRange(range);
		}	}

	bool SlicePreviewFlow::isAvailable()
	{
		return m_attain;
	}

	int SlicePreviewFlow::gCodeVisualType()
	{
		return m_gCodeVisualType;
	}

	void SlicePreviewFlow::setGCodeVisualType(int type)
	{
		
		m_visualType = (gcode::GCodeVisualType)type;
		m_scene->setGCodeVisualType((gcode::GCodeVisualType)type);
		m_gcodeEffect->setGCodeVisualType(type);

		if (m_attain) {
			m_attain->updateGCodePreviewType((GCodePreviewType)type);
		}

		renderOneFrame();
		m_gCodeVisualType = type;
		emit gCodeVisualTypeChanged();
	}

	void SlicePreviewFlow::showGCodeType(int type, bool show)
	{
		if (!m_attain || !m_previewNode)
			return;

		m_roleState[type] = show;
		if (type == (int)Orca::ExtrusionRole::Travel)
		{
			m_previewNode->setComponentEnabled(GCodeComponentType::Travel, show);
		}
		else if (type == (int)Orca::ExtrusionRole::Retract)
		{
			m_previewNode->setComponentEnabled(GCodeComponentType::Retract, show);
		}
		else if (type == (int)Orca::ExtrusionRole::Unretract)
		{
			m_previewNode->setComponentEnabled(GCodeComponentType::Unretract, show);
		}
		else if (type == (int)Orca::ExtrusionRole::Seam)
		{
			m_previewNode->setComponentEnabled(GCodeComponentType::Seam, show);
		}
		else if (type == (int)Orca::ExtrusionRole::Wipe)
		{
			m_previewNode->setComponentEnabled(GCodeComponentType::Wipe, show);
		}
		else if (type < (int)Orca::ExtrusionRole::erCount)
		{
			m_gcodeEffect->setRoleEnabled(type, show);
			m_gcodeEffect->applyExtrusionRole();
		}
	}

	void SlicePreviewFlow::setCurrent(int layer, int step)
	{

		if (!m_attain)
			return;

		m_currentLayer = layer;
		int layerSteps = m_attain->steps(m_currentLayer);
		m_stepsMax = layerSteps;

		m_currentStep = step > layerSteps ? layerSteps : step;

		requestGCodes(m_currentLayer);

		notifyLayerRange();
		notifyClipValue();
		setPosition();
		setLayerHeight();
		setTemperature();
		setAcc();
		setLineWidth();
		setFlow();
		setLayerTime();
		setFanSpeed();
		setSpeed();
		emit stepsChanged();
		emit currentStepSpeedChanged();
		emit currentStepSpeedChanged();
	}

	void SlicePreviewFlow::setCurrentLayer(int layer, bool randonStep)
	{
		if (!m_attain)
			return;

		m_currentLayer = layer;
		int layerSteps = m_attain->steps(m_currentLayer);
		m_currentStep = std::min(layerSteps, m_currentStep);
		m_stepsMax = layerSteps;
		notifyLayerRange();
		notifyClipValue();

		requestGCodes(m_currentLayer);
		setPosition();
		setLayerHeight();
		setTemperature();
		setAcc();
		setLineWidth();
		setFlow();
		setLayerTime();
		setFanSpeed();
		setSpeed();
		emit stepsChanged();
		// emit stepsMaxChanged();
		//emit layerGCodesChanged();
		emit currentStepSpeedChanged();
	}

	QAbstractListModel* SlicePreviewFlow::getSpeedModel() const {
		return m_speedModel.get();
	}

	QAbstractListModel* SlicePreviewFlow::getStructureModel() const {
		return m_structureModel.get();
	}

	QAbstractListModel* SlicePreviewFlow::getExtruderModel() const {
		return m_extruderModel.get();
	}

	QAbstractTableModel* SlicePreviewFlow::getExtruderTableModel() const {
		return m_extruderTableModel.get();
	}

	QAbstractListModel* SlicePreviewFlow::getLayerHightModel() const {
		return m_layerHightModel.get();
	}

	QAbstractListModel* SlicePreviewFlow::getLineWidthModel() const {
		return m_lineWidthModel.get();
	}

	QAbstractListModel* SlicePreviewFlow::getFlowModel() const {
		return m_flowModel.get();
	}

	QAbstractListModel* SlicePreviewFlow::getLayerTimeModel() const {
		return  m_layerTimeModel.get();
	}

	QAbstractListModel* SlicePreviewFlow::getFanSpeedModel() const {
		return m_fanSpeedModel.get();
	}

	QAbstractListModel* SlicePreviewFlow::getTemperatureModel() const {
		return m_temperatureModel.get();
	}

	QAbstractListModel* SlicePreviewFlow::getAccModel() const {
		return m_accModel.get();
	}

	void SlicePreviewFlow::setCurrentStep(int step)
	{
		m_currentStep = step;
		notifyLayerRange();
		notifyClipValue();
		setPosition();
		setTemperature();
		setAcc();
		setLineWidth();
		setFlow();
		setLayerTime();
		setFanSpeed();
		setSpeed();
		// emit stepsChanged();
		emit currentStepSpeedChanged();
	}

	int SlicePreviewFlow::findViewIndexFromStep(int nStep)
	{
		if (!m_attain)
			return -1;

		return m_attain->findViewIndexFromStep(m_currentLayer, nStep);
	}

	int SlicePreviewFlow::findStepFromViewIndex(int nViewIndex)
	{
		if (!m_attain)
			return -1;

		return m_attain->findStepFromViewIndex(m_currentLayer, nViewIndex);
	}

	void SlicePreviewFlow::showOnlyLayer(int layer)
	{
		m_showOnlyTopLayer = layer == -1 ? false : true;
		notifyClipValue();
	}

	void SlicePreviewFlow::setAnimationState(int state)
	{
		m_animationState = state;
		notifyLayerRange();
	}

	void SlicePreviewFlow::setCurrentLayerFocused(bool focused)
	{
		m_animationState = focused;
		notifyLayerRange();
	}

	int SlicePreviewFlow::currentLayer()
	{
		return m_attain ? m_currentLayer : 0;
	}

	int SlicePreviewFlow::layers()
	{
		return m_attain ? m_attain->layers() : 0;
	}

	int SlicePreviewFlow::steps()
	{
		if (!m_attain)
			return 0;

		return m_attain->steps(m_currentLayer);
	}

	int SlicePreviewFlow::stepsMax()
	{
		return m_stepsMax;
	}
	float SlicePreviewFlow::height()
	{
		if (!m_attain)
			return 0;
		trimesh::vec3 pos = m_attain->traitPosition(m_currentLayer+1, 0);
		return pos.z;
	}

	QStringList SlicePreviewFlow::layerGCodes()
	{
		if (!m_attain)
			return QStringList();

		QStringList codes = m_attain->layerGcode();
		return codes;
	}

	void SlicePreviewFlow::requestGCodes(int layer)
	{
		m_requestLayer = layer;
		m_requestLayerGCodeTimer.start(200);
	}

	QVector3D SlicePreviewFlow:: position()
	{
		return m_position;
	};


	void SlicePreviewFlow::setPosition()
	{
		if (!m_attain)
			return ;

		trimesh::vec3 trimeshVec= m_attain->traitPosition(m_currentLayer, m_currentStep);
		QVector3D qVec(trimeshVec.x, trimeshVec.y, trimeshVec.z);
		m_position = qVec;
		emit positionChanged();
	}

	float  SlicePreviewFlow::layerHeight()
	{
		return m_layerHeight;
	}

	void SlicePreviewFlow::setLayerHeight()
	{
		if (!m_attain)
			return;

		m_layerHeight = m_attain->layerHeight(m_currentLayer);
		emit layerHeightChanged();
	}

	float  SlicePreviewFlow::temperature()
	{
		return m_temperature;
	}

	void SlicePreviewFlow::setTemperature()
	{
		if (!m_attain)
			return;

		m_temperature = m_attain->traitTemperature(m_currentLayer, m_currentStep);
		emit temperatureChanged();
	}

	float  SlicePreviewFlow::acc()
	{
		return m_acc;
	}

	void SlicePreviewFlow::setAcc()
	{
		if (!m_attain)
			return;
		m_acc = m_attain->traitAcc(m_currentLayer, m_currentStep);
		emit accChanged();
	}

	float SlicePreviewFlow::lineWidth()
	{
		return m_lineWidth;
	}

	void SlicePreviewFlow::setLineWidth()
	{
		if (!m_attain)
			return;
		m_lineWidth = m_attain->traitLineWidth(m_currentLayer, m_currentStep);
		emit lineWidthChanged();
	}

	float SlicePreviewFlow::flow()
	{
		return m_flow;
	}

	void SlicePreviewFlow::setFlow()
	{
		if (!m_attain)
			return;
		m_flow = m_attain->traitFlow(m_currentLayer, m_currentStep);
		emit flowChanged();
	}

	float SlicePreviewFlow::layerTime()
	{
		return m_layerTime;
	}

	void SlicePreviewFlow::setLayerTime()
	{
		if (!m_attain)
			return;
		m_layerTime = m_attain->traitLayerTime(m_currentLayer, m_currentStep);
		emit layerTimeChanged();
	}

	float SlicePreviewFlow::fanSpeed()
	{
		return m_fanSpeed;
	}

	void SlicePreviewFlow::setFanSpeed()
	{
		if (!m_attain)
			return;
		m_fanSpeed = m_attain->traitFanSpeed(m_currentLayer, m_currentStep);
		emit fanSpeedChanged();
	}

	float SlicePreviewFlow::speed()
	{
		return m_speed;
	}

	void SlicePreviewFlow::setSpeed()
	{
		if (!m_attain)
			return;
		m_speed = m_attain->traitSpeed(m_currentLayer, m_currentStep);
		emit speedChanged();
	}

	float SlicePreviewFlow::currentStepSpeed()
	{
		if (!m_attain)
			return 10.0f;

		return m_attain->traitDuration(m_currentLayer, m_currentStep);
	}

	void SlicePreviewFlow::clear()
	{
		if (m_attain == NULL)
			return;

		m_attain->deleteLater();
		m_attain = NULL;
	}

	
	void SlicePreviewFlow::previewSliceAttain(SliceAttain* attain, int layer)
	{
		if (attain == nullptr) return;

		//提前预览阶段使用 gvt_structure 类型，其他类型数据可能不准确
		m_scene->setGCodeVisualType(gcode::GCodeVisualType::gvt_structure);

		//Qt3DRender::QGeometry* geometry = attain->createGeometry();
		//m_scene->setGeometry(geometry, Qt3DRender::QGeometryRenderer::Points);
		
		m_scene->setLayerHeight(attain->layerHeight());
		m_scene->setLineWidth(attain->lineWidth());

		QMatrix4x4 pose = qtuser_3d::xform2QMatrix(attain->modelMatrix());
		Printer* printer = getSelectedPrinter();
		if (printer)
			pose.translate(printer->globalBox().min);
		m_scene->setPreviewNodeModelMatrix(pose);

		m_scene->setIndicatorVisible(indicatorVisible());

		m_layerHightModel->setRange(attain->minLayerHeight(), attain->maxLayerHeight());

		setCurrentLayer(layer, false);
		setCurrentStep(attain->steps(layer));

		emit layersChanged();
	}

	void SlicePreviewFlow::setSliceAttain(SliceAttain* attain)
	{
		bool reload = false;
		if(m_attain && attain && 
			m_attain->id() == attain->id())
		{
			return;
		}
		if (attain)
		{
			int totalLayers = attain->layers(), totalLayers2 = m_lastTotalLayers;
			reload = totalLayers == totalLayers2;
		}

		m_attain = attain;

		if (m_attain == NULL)
		{
			setPreviewNode(NULL);
			m_scene->clear();
			m_scene->setIndicatorVisible(false);
		}
		else if (isUsingPreviewScene())
		{
			setPreviewEnabled(false);
			 m_scene->setIndicatorVisible(indicatorVisible());

			QMatrix4x4 pose = qtuser_3d::xform2QMatrix(attain->modelMatrix());
			Printer* printer = getSelectedPrinter();
			if (printer)
				pose.translate(printer->globalBox().min);

			m_attain->buildGeometries((GCodePreviewType)m_visualType, m_gcodeEffect, m_gcodeMonoEffect, pose);
			setPreviewNode(m_attain->previewNode());

			m_speedModel->setRange(m_attain->minSpeed(), m_attain->maxSpeed());
			m_structureModel->setOrcaTimeParts(m_attain->getTimeParts_orca(),m_attain->printTime());
			m_layerHightModel->setRange(m_attain->minLayerHeight(), m_attain->maxLayerHeight());
			m_lineWidthModel->setRange(m_attain->minLineWidth(), m_attain->maxLineWidth());
			m_flowModel->setRange(m_attain->minFlowOfStep(), m_attain->maxFlowOfStep());
			m_layerTimeModel->setRange(m_attain->minTimeOfLayer(), m_attain->maxTimeOfLayer());
			//m_fanSpeedModel->setRange(0.0, 1.0);
			m_temperatureModel->setRange(m_attain->minTemperature(), m_attain->maxTemperature());
			m_accModel->setRange(m_attain->minAcc(), m_attain->maxAcc());
			setGCodeVisualType(m_gCodeVisualType);

			if (!reload)
			{
				int layer = m_attain->layers();
				int step = 99999999;
				setCurrent(layer, step);
			}
			else
			{
				setCurrent(m_currentLayer, m_currentStep);
			}


			m_lastTotalLayers = m_attain->layers();
			//m_scene->setLayerHeight(m_attain->layerHeight());
			//m_scene->setLineWidth(m_attain->lineWidth());

			setPreviewEnabled(true);
			setCurrentLayerFocused(false);
		}

		emit layersChanged();

		setExtruderTableModel();
	}

	SliceAttain* SlicePreviewFlow::attain()
	{
		return m_attain;
	}

	SliceAttain* SlicePreviewFlow::takeAttain()
	{
		//return m_attain.take();
		SliceAttain* attain = m_attain;
		m_attain = NULL;
		return attain;
	}

    bool SlicePreviewFlow::indicatorVisible()
    {
        return m_indicatorVisible;
    }

    void SlicePreviewFlow::setIndicatorVisible(bool visible)
    {
        if (m_indicatorVisible != visible)
        {
            m_indicatorVisible = visible;
            emit indicatorVisibleChanged();
			
			m_scene->setIndicatorVisible(m_indicatorVisible);
			renderOneFrame();
        }   
    }

	bool SlicePreviewFlow::isAttainDisplayCompletly()
	{
		if (!m_attain)
			return false;

		return m_currentLayer == m_attain->layers();
	}

	void SlicePreviewFlow::setPreviewNode(GCodePreviewNode* node)
	{
		if (m_previewNode)
		{
			m_previewNode->setEnabled(false);
			disconnect(m_previewNode, &QObject::destroyed, this, &SlicePreviewFlow::onPreviewNodeDestroyed);
		}

		m_previewNode = node;

		if (m_previewNode)
		{
			m_previewNode->setParent(m_root);
			m_previewNode->setEnabled(true);
			connect(m_previewNode, &QObject::destroyed, this, &SlicePreviewFlow::onPreviewNodeDestroyed);

			for (int i = 0; i < (int)Orca::ExtrusionRole::Count; ++i)
			{
				showGCodeType(i, m_roleState[i]);
			}
		}
	}

	void SlicePreviewFlow::onPreviewNodeDestroyed()
	{
		m_previewNode = NULL;
	}

	void SlicePreviewFlow::setExtruderTableModel()
	{
		if (!m_attain || !m_extruderTableModel)
			return;

		std::vector<std::pair<int, double>> volumes_per_extruder;
		std::vector<std::pair<int, double>> flush_per_filament;
		std::vector<std::pair<int, double>> volumes_per_tower;

		const QVariantList& nozzleList = m_attain->getNozzleColorList(); 
		setNozzleColorList(nozzleList);
		m_attain->getFilamentsList(volumes_per_extruder, flush_per_filament, volumes_per_tower);
 		m_extruderTableModel->setData(volumes_per_extruder, flush_per_filament, volumes_per_tower);
	}

	void SlicePreviewFlow::setNozzleColorList(const QVariantList& list)
	{
		{
			QList<cxgcode::GcodeExtruderData> dataList;

			for (size_t i = 0; i < list.size(); i++)
			{
				cxgcode::GcodeExtruderData data;
				data.color = list.at(i).value<QColor>();
				data.index = i + 1;
				dataList.push_back(data);
			}
			m_extruderTableModel->setColorList(dataList);
			m_extruderModel->setDataList(dataList);
		}

		m_gcodeEffect->setNozzleColorList(list);
		}

		QString SlicePreviewFlow::printerName()
		{
			if (m_attain)
				return m_attain->printerName();
			else
				return "";
		}
	}



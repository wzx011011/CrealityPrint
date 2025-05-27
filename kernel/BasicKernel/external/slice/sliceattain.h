#ifndef _NULLSPACE_SLICEATTAIN_1590249988869_H
#define _NULLSPACE_SLICEATTAIN_1590249988869_H
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include "data/kernelenum.h"
#include "Qt3DCore/QNode"

#include "crslice/gcode/sliceresult.h"
#include "cxgcode/simplegcodebuilder.h"

#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QGeometryRenderer>

#include "qtuser3d/math/box3d.h"

#include <unordered_map>
#include "trimesh2/Vec.h"
#include "trimesh2/TriMesh.h"

#include <crslice/gcode/define.h>
#include "crslice/header.h"

#include "crslice2/cacheslice.h"

#include <QTimer>

namespace qtuser_3d
{
	class XEntity;
	class XEffect;
};

namespace Qt3DCore
{
	class QEntity;
};

namespace Qt3DRender
{
	class QBuffer;
};

namespace creative_kernel
{
	enum class SliceAttainType
	{
		sat_slice,
		sat_file
	};

	enum class GCodeComponentType : unsigned int
	{
		Retract = 0,
		Unretract,
		Seam,
		Tool_change,
		Color_change,
		Pause_Print,
		Custom_GCode,
		Travel,
		Wipe,
		Extrude,
		Count
	};

	enum class GCodePreviewType : unsigned int
	{
		Speed = 0,
		Role,
		Extruder,
		Height,
		Width,
		VolumeRate,
		LayerTime,
		FanSpeed,
		Temperature,
		Acc
	};

	class GCodePreviewNode;
	class GCodePreviewEffect;
	class SliceAttain : public QObject, public gcode::GcodeTracer
	{
		Q_OBJECT
	public:
		SliceAttain(SliceResultPointer result, const QString& sliceName, SliceAttainType type, QObject* parent = nullptr);
		virtual ~SliceAttain();

		int moveId(int layer, int step);
		int firstMoveId(int layer);
		bool isLastStep(int layer, int step);
		int id();
		void setId(int id);

		void build(ccglobal::Tracer* tracer = nullptr);
		void build_paraseGcode(ccglobal::Tracer* tracer = nullptr);
		void preparePreviewData(ccglobal::Tracer* tracer = nullptr);

		void setSourceGCodeFile(const QString& fileName);
		Q_INVOKABLE QString sliceName();
		Q_INVOKABLE QString sourceFileName();
		Q_INVOKABLE bool isFromGcode();
		Q_INVOKABLE bool isFromFile();

		Q_INVOKABLE QString nozzleDiameter();
		Q_INVOKABLE QString printerName();
		Q_INVOKABLE int printTime();
		Q_INVOKABLE QString material_weight();
		Q_INVOKABLE QString printing_time();
		Q_INVOKABLE QString material_money();
		Q_INVOKABLE QString material_length();
		void requestLayerGcode(int layer);
		QStringList layerGcode();
		void setOutputnameFormat(const QString& format) { m_outputnameFormat = format; }
		QString outputnameFormat() { return m_outputnameFormat; }
		trimesh::box3 box();
		Q_INVOKABLE int layers();
		int steps(int layer);
		int totalSteps();
		int nozzle();
		float minSpeed();
		float maxSpeed();

		float minTimeOfLayer();
		float maxTimeOfLayer();

		float minFlowOfStep();
		float maxFlowOfStep();

		float minLineWidth();
		float maxLineWidth();

		float minLayerHeight();
		float maxLayerHeight();

		float minTemperature();
		float maxTemperature();

		float minAcc();
		float maxAcc();

		Q_INVOKABLE float layerHeight(int layer);
		float layerHeight();
		float lineWidth();

		std::vector<std::pair<int,float>> getTimeParts_orca() ;
		bool isCuraProducer();

		Q_INVOKABLE QVariantList getNozzleColorList();
		void getFilamentsList(std::vector<std::pair<int,double>>& volumes_per_extruder, std::vector<std::pair<int, double>>& flush_per_filament, std::vector<std::pair<int, double>>& volumes_per_tower);

		void loadGCodePath();
		void getDataFromOrca();

		QImage* getImageFromGcode();

		float traitSpeed(int layer, int step);
		trimesh::vec3 traitPosition(int layer, int step);
		float traitDuration(int layer, int step);
		
		float traitLayerHeight(int layer, int step);
		float traitAcc(int layer, int step);
		float traitLineWidth(int layer, int step);
		float traitFlow(int layer, int step);
		float traitLayerTime(int layer, int step);
		float traitFanSpeed(int layer, int step);
		float traitTemperature(int layer, int step);

		trimesh::fxform modelMatrix();

		int findViewIndexFromStep(int layer, int nStep);
		int findStepFromViewIndex(int layer, int nViewIndex);
		void updateStepIndexMap(int layer);

        float getMachineHeight();
        float getMachineWidth();
        float getMachineDepth();
	
		/* render */
		void buildGeometries(GCodePreviewType type, GCodePreviewEffect* polyEffect, GCodePreviewEffect* monoEffect, const QMatrix4x4& pose);
		QList<Qt3DRender::QGeometry*> getGeometries(GCodeComponentType type);
		GCodePreviewNode* previewNode();
		void updateGCodePreviewType(GCodePreviewType type);


        void saveGCode(const QString& fileName, QImage* previewImage = nullptr);
		void saveTempGCode();
		void saveTempGCodeWithImage(QImage& image);

		void saveGCodeImage();		// oc parse

		QString tempGCodeFileName();
        QString tempGcodeThumbnail();
		QString tempGCodeImageFileName();

		void tick(const std::string& tag) {};
		void getPathData(const trimesh::vec3 point, float e, int type, bool isSeam = false,bool isOrca = false)override;
		void getPathDataG2G3(const trimesh::vec3 point, float i, float j, float e, int type,int p, bool isG2 = true, bool isOrca = false, bool isseam = false) override;
		void setParam(gcode::GCodeParseInfo& pathParam)override;
		void setLayer(int layer)override;
		void setLayers(int layer)override;
		void setSpeed(float s)override;
		void setAcc(float acc)override;
		void setTEMP(float temp)override;
		void setExtruder(int nr)override;
		void setFan(float fan)override;
		void setZ(float z, float h = -1)override;
		void setE(float e)override;
		void setWidth(float width) override;
		void setLayerHeight(float height) override;
		void setLayerPause(int pause) override;
		void setTime(float time)override;
		void getNotPath()override;
		void set_data_gcodelayer(int layer, const std::string& gcodelayer)override;//set a layer data
		void setNozzleColorList(std::string& colorList, std::string& defaultColorList, std::string& defaultType)override;
		void getNozzleColorList(std::vector<std::string>& colorList, std::vector<std::string>& defaultColorList, std::vector<std::string>& defaultType);
		void onSupports(int layerIdx, float z, float thickness, const std::vector<std::vector<trimesh::vec3>>& paths)override;
		void setSceneBox(const trimesh::box3& box)override;
		void writeImages(const std::vector<std::pair<trimesh::ivec2, std::vector<unsigned char>>>& images) override;

		qtuser_3d::Box3D getGCodePathBoundingBox();
		QMap<QString, QPair<QString, int64_t> > getSliceWarningDetails();
		void setSliceWarningDetails(const QMap<QString, QPair<QString, int64_t> >& sliceWarningDetails);

		void setGCodePath(crslice2::GCodePath gcodePath);
		crslice2::GCodePath gcodePath();

	private:
		int addData(char* buffer, char* data, int index, int size, int repeat);
		Qt3DRender::QBuffer* createBuffer(GCodePreviewType type, int stepCount, const std::vector<crslice2::StepInfoPtr>& steps);
		const crslice2::StepInfo& findStep(int layer, int step, bool* ok);

	protected:
		QTimer m_layerGCodeTimer;;

		Qt3DCore::QNode* m_parent{ NULL };

		int m_id {-1};
		crslice2::GCodePath m_gcodePath;
		QMap<GCodeComponentType, QList<Qt3DRender::QAttribute*>> m_attributesCache;
		QMap<GCodeComponentType, QList<Qt3DRender::QGeometry*>> m_geometriesCache;

		GCodePreviewNode* m_previewNode{ NULL };

		bool m_isBuildGeometries=false;
		GCodePreviewType m_previewType;

		cxgcode::SimpleGCodeBuilder builder;
		SliceResultPointer m_result;

		Qt3DRender::QGeometry* m_cache;
		//Qt3DRender::QAttribute* m_attribute;

		SliceAttainType m_type;

		QString m_sliceName;
		QString m_sourceGCodeFile;
		QString m_tempGCodeFileName;
		QString m_tempGCodeImageFileName;

		int m_readLayer{-1};
		QStringList m_layerCodes;
		QList<int> m_layerFileSeekMap;

		QMap<QString, QPair<QString, int64_t> > m_sliceWarningDetails;
		QString m_outputnameFormat;
	};
}

#endif // _NULLSPACE_SLICEATTAIN_1590249988869_H

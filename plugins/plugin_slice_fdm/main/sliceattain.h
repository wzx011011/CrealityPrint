#ifndef _NULLSPACE_SLICEATTAIN_1590249988869_H
#define _NULLSPACE_SLICEATTAIN_1590249988869_H
#include <QtCore/QObject>
#include <QtCore/QUrl>

#include "us/globalsetting.h"
#include "core/layerlines.h"

#include <Qt3DRender/QGeometry>
#include "qtuser3d/math/box3d.h"

#include <unordered_map>
#include "trimesh2/Vec.h"
#include "trimesh2/TriMesh.h"

enum class AttainType
{
	at_fdm,
};
namespace cxsw
{
	class DLPData;
}

class FDMLayerBuilder;
class AttainInfo
{
public:
	AttainInfo();
	~AttainInfo();

	AttainType attainType;
	qtuser_3d::Box3D printerBox;

	int layers;
	double volume;
	int printTime;
	float materialLenth;

	//fdm
	qtuser_3d::Box3D box;
	FDMLayerBuilder* builder;
	int nNozzle;
	std::vector<int> steps;

	//cura
	std::string prefixCode;
	std::vector<std::string> layerCode;
	std::string tailCode;
};

class CXSLiceReport;
class SliceAttain: public QObject
{
public:
	SliceAttain(QObject* parent = nullptr);
	virtual ~SliceAttain();

	CXSLiceReport* getReport();

	void build(std::vector<std::string>& layers
		, std::string& prefix, std::string& tailfix, us::GlobalSetting* globalSettings);

	qtuser_3d::Box3D box();
	qtuser_3d::Box3D printerBox();

	int layers();
	float volume();
	int steps(int layer);
	int nozzle();
	time_t printingTime();

	std::string layerGcode(int layer);
	//fdm
	void take(Qt3DRender::QGeometry** geometry, std::unordered_map<int, trimesh::vec4>& indicator);
	void saveGCode(const QString& fileName, QImage* previewImage);
protected:
	void calculatePrintTimeAndMatial(const std::string& prefixCode);
	void fromGlobalSetting(us::GlobalSetting* globalSettings);
protected:
	AttainInfo m_info;
	CXSLiceReport* m_report;

	Qt3DRender::QGeometry* m_cache;
	std::unordered_map<int, trimesh::vec4> m_orderCache;
};


#endif // _NULLSPACE_SLICEATTAIN_1590249988869_H

#ifndef _FDMLAYERBUILDER_1604127418528_H
#define _FDMLAYERBUILDER_1604127418528_H
#include "core/layerlines.h"
#include <Qt3DRender/QGeometry>

#include "qtuser3d/math/box3d.h"
#include <unordered_map>

class FDMLayerBuilder
{
public:
	FDMLayerBuilder();
	~FDMLayerBuilder();

	static FDMLayerBuilder* buildFromGCode(const std::string& prevCode, const std::vector<std::string>& layerCode, float lineWidth,
		std::vector<int>* steps);

	// 四棱柱 GCode 预览
	void buildGeometry(Qt3DRender::QGeometry** geometry, std::unordered_map<int, trimesh::vec4>** indicator);
	// 基于 billboard 和细分着色器实现的  GCode 预览方案
	void buildBillboardGeometry(Qt3DRender::QGeometry** geometry, std::unordered_map<int, trimesh::vec4>** indicator);
	// 基于 billboard 和顶点着色器实现的 GCode 预览方案
	void buildBillboardNoTessGeometry(Qt3DRender::QGeometry** geometry, std::unordered_map<int, trimesh::vec4>& indicator);
	void clear();
	int totalLayers();
	int totalNozzleNr();
	qtuser_3d::Box3D totalBox();
protected:
	std::vector<LayerPolyLine> polyLines;
	unsigned extendR;
	int lineCount;

	std::vector< LineMove> moveLines;

	qtuser_3d::Box3D box;
	int nNozzle;
	int layers;
};

#endif // _FDMLAYERBUILDER_1604127418528_H
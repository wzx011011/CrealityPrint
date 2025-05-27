#ifndef _NULLSPACE_SLICELAYER_LAYER_LINES_H
#define _NULLSPACE_SLICELAYER_LAYER_LINES_H
#include "enginewrapperexport.h"
#include <vector>

#include "sliceline.h"

enum class ENGINE_WRAPPER_API LinePointType
{
	NoneType = 0, 
	StartPoint = 1,
	MidPoint = 2,
	EndPoint = 3,
	CirclePoint = 0x100
};

class ENGINE_WRAPPER_API LinePoint
{
public:
	LinePoint();
	LinePoint(trimesh::vec3 pt, LinePointType pointType = LinePointType::NoneType);
	LinePoint(trimesh::vec2 pt, LinePointType pointType = LinePointType::NoneType);

	bool isSamePoint(const LinePoint& lp) const;

public:
	trimesh::vec3 m_pt;
	LinePointType m_type;

};


class ENGINE_WRAPPER_API LineMove
{
public:
	LineMove();
	LineMove(trimesh::vec3 startpt, trimesh::vec3 endpt, float step);

public:
	trimesh::vec3 m_start;
	trimesh::vec3 m_end;
	float m_step;
};



struct ENGINE_WRAPPER_API SliceLineAttributes
{
	float step;
	int speedFlag;
	int nozzle;
	// 层起始标记
	int layerStartFlag;
	SliceLineType linetype;

	int m_startPointPos;
	int m_endPointPos;

	SliceLineAttributes();
};


class ENGINE_WRAPPER_API LayerPolyLine
{
public:
	LayerPolyLine();

	int addLine(const SliceLine& sliceline, float step);

	int offsetNormal(trimesh::vec3 n1, trimesh::vec3 n2, trimesh::vec3& nre);

	int callLineNormals();

	int extentedNPoly(int npoly);

	void getLineStartEnd(const SliceLineAttributes& sla, LinePoint*& start, LinePoint*& end, int layerpos);
	void getPointPositon(const SliceLineAttributes& sla, trimesh::vec3*& startPosition, trimesh::vec3*& endPosition, int layerpos);
	void getLineNormal(int linepos, trimesh::vec3*& normal, int layerpos);
	int getSpeedFlag(float speed);
	void clearAll();

private:
	int oneLineExtendNormal(const trimesh::vec3& start, const trimesh::vec3& end, int r, int pos);

	int initExtensionParam(int r);

	int changeLineNormals2PointNormals();

	int callExtensionPointByNormals();

public:
	std::vector < std::vector< LinePoint>> m_points;
	
	std::vector < std::vector< SliceLineAttributes>> m_lineAttributes;

	std::vector < std::vector<trimesh::vec3>> m_linesNormals;

	std::vector < std::vector<trimesh::vec3>> m_extenedNormals;
	std::vector < std::vector<trimesh::vec3>> m_extenedPositions;

	float m_layer;
	float m_thickness;
	float m_lineWidth;

private:
	int m_r;
	std::vector<trimesh::vec3> m_baseNormals;
	std::vector<float> m_sliceRadius;

};

#endif // CREATIVE_KERNEL_BOX2DGRID_1594095137563_H
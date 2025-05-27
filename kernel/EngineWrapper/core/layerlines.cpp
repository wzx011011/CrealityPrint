#include "layerlines.h"
#include "trimesh2/quaternion.h"

#define EPISSION_2D 0.0000001


LinePoint::LinePoint() : m_type(LinePointType::NoneType)
{
}

LinePoint::LinePoint(trimesh::vec3 pt, LinePointType pointType) : m_pt(pt), m_type(pointType)
{
}

LinePoint::LinePoint(trimesh::vec2 pt, LinePointType pointType) : m_pt(pt), m_type(pointType)
{
}

bool LinePoint::isSamePoint(const LinePoint& lp) const
{
	trimesh::vec3 p = this->m_pt - lp.m_pt;
	if (len(p) < EPISSION_2D)
	{
		return true;
	}
	return false;
}



LineMove::LineMove() {}

LineMove::LineMove(trimesh::vec3 startpt, trimesh::vec3 endpt, float step)
	: m_start(startpt)
	, m_end(endpt)
	, m_step(step)
{}



SliceLineAttributes::SliceLineAttributes() : step(0), speedFlag(1), linetype(SliceLineType::NoneType), m_startPointPos(0), m_endPointPos(0), nozzle(0)
{
	//
}




LayerPolyLine::LayerPolyLine() : m_layer(0.0f), m_thickness(0.0f), m_lineWidth(0.0f), m_r(0)
{
	//
}

int LayerPolyLine::addLine(const SliceLine& sliceline, float step)
{
	int ret = 0;
	LinePoint lpts(sliceline.start, LinePointType::StartPoint), lpte(sliceline.end, LinePointType::EndPoint);

	SliceLineAttributes sla;
	sla.linetype = sliceline.type;

	sla.step = step;
	sla.nozzle = sliceline.nozzle;
	//int speedFlag = int(sliceline.speed / 1000);
	//if (speedFlag > 9)
	//{
	//	speedFlag = 9;
	//}
	sla.speedFlag = getSpeedFlag(sliceline.speed);

	sla.layerStartFlag = sliceline.layerStartFlag ? 1.0 : 0.0;

	std::vector< LinePoint>* lpLoop = nullptr;;
	std::vector< SliceLineAttributes>* labLoop = nullptr;

	if (m_points.size() > 0)
	{
		lpLoop = &m_points.back();;
		labLoop = &m_lineAttributes.back();
	}

//	if (lpLoop == nullptr)
	{
		m_points.push_back(std::vector< LinePoint>());
		lpLoop = &m_points.back();

		m_lineAttributes.push_back(std::vector< SliceLineAttributes>());
		labLoop = &m_lineAttributes.back();

		lpLoop->push_back(lpts);
		sla.m_startPointPos = lpLoop->size() - 1;
		lpLoop->push_back(lpte);
		sla.m_endPointPos = lpLoop->size() - 1;
		ret = 2;
	}
	//else 
	//{
	//	LinePoint& last_lp = lpLoop->back();
	//	if (last_lp.isSamePoint(lpts))
	//	{
	//		if (lpLoop->at(0).isSamePoint(lpte))
	//		{
	//			sla.m_startPointPos = lpLoop->size() - 1;
	//			last_lp.m_type = LinePointType::MidPoint;

	//			sla.m_endPointPos = 0;
	//			lpLoop->at(0).m_type = LinePointType::MidPoint;
	//			ret = 0;
	//		}
	//		else
	//		{
	//			sla.m_startPointPos = lpLoop->size() - 1;

	//			last_lp.m_type = LinePointType::MidPoint;
	//			lpLoop->push_back(lpte);
	//			sla.m_endPointPos = lpLoop->size() - 1;
	//			ret = 1;
	//		}
	//	}
	//	else
	//	{
	//		m_points.push_back(std::vector< LinePoint>());
	//		lpLoop = &m_points.back();

	//		m_lineAttributes.push_back(std::vector< SliceLineAttributes>());
	//		labLoop = &m_lineAttributes.back();

	//		lpLoop->push_back(lpts);
	//		sla.m_startPointPos = lpLoop->size() - 1;
	//		lpLoop->push_back(lpte);
	//		sla.m_endPointPos = lpLoop->size() - 1;
	//		ret = 2;
	//	}
	//}
	labLoop->push_back(sla);

	return ret;
}


int LayerPolyLine::offsetNormal(trimesh::vec3 n1, trimesh::vec3 n2, trimesh::vec3& nre)
{
	nre = (n1 + n2) / 2;
	float len = len2(nre);
	if (len < 0.0000001)
	{
		len = 1;
	}
	else if (len < 0.25)
	{
		len = 0.25;
	}
	nre /= len;
	return 0;
}

int LayerPolyLine::callLineNormals()
{
	int total = 0;
	for(int i = 0; i < m_lineAttributes.size(); i++)
	{ 
		std::vector< SliceLineAttributes>& slaSets = m_lineAttributes[i];
		for (SliceLineAttributes& sla : slaSets)
		{
			trimesh::vec3& start = m_points[i][sla.m_startPointPos].m_pt;
			trimesh::vec3& end = m_points[i][sla.m_endPointPos].m_pt;

			oneLineExtendNormal(start, end, m_r, i);
			total += m_linesNormals[i].size();
		}
	}
	return total;
}

int LayerPolyLine::oneLineExtendNormal(const trimesh::vec3& start, const trimesh::vec3& end, int r, int pos)
{
	trimesh::vec3 dir = end - start;
	trimesh::normalize(dir);
	trimesh::quaternion q = q.fromDirection(dir, trimesh::vec3(0.0f, 0.0f, 1.0f));
	normalize(dir);

	for (unsigned i = 0; i < r; ++i)
	{
		trimesh::vec3 n = q * m_baseNormals[i];
		m_linesNormals[pos].push_back(n);
	}
	return 0;
}

int LayerPolyLine::changeLineNormals2PointNormals()
{
	for (int i = 0; i < m_linesNormals.size(); i++)
	{
		std::vector<trimesh::vec3>& lnSets = m_linesNormals[i];
		std::vector<LinePoint>& lpSets = m_points[i];
		std::vector<trimesh::vec3>& enSet = m_extenedNormals[i];

		int realLen = lnSets.size() / m_r;

		for (int j = 0; j < lpSets.size(); j++)
		{
			LinePoint& lp = lpSets[j];

			if (lp.m_type == LinePointType::StartPoint)
			{
				for (int k = 0; k < m_r; k++)
				{
					enSet.push_back(lnSets[j * m_r + k]);
				}
			}
			else if (lp.m_type == LinePointType::MidPoint)
			{
				int prej = j - 1;
				if (j == 0)
				{
					prej = realLen - 1;
				}
				for (int k = 0; k < m_r; k++)
				{
					trimesh::vec3 n;
					offsetNormal(lnSets[prej * m_r + k], lnSets[j * m_r + k], n);
					enSet.push_back(n);
				}
			}
			else if (lp.m_type == LinePointType::EndPoint)
			{
				for (int k = 0; k < m_r; k++)
				{
					enSet.push_back(lnSets[(j - 1) * m_r + k]);
				}
			}
		}
	}

	
	return 0;
}

void LayerPolyLine::getLineStartEnd(const SliceLineAttributes& sla, LinePoint*& start, LinePoint*& end, int layerpos)
{
	start = &m_points[layerpos][sla.m_startPointPos];
	end = &m_points[layerpos][sla.m_endPointPos];
}

void LayerPolyLine::getPointPositon(const SliceLineAttributes& sla, trimesh::vec3*& startPosition, trimesh::vec3*& endPosition, int layerpos)
{
	int startPos = sla.m_startPointPos * m_r;
	int endPos = sla.m_endPointPos * m_r;
	startPosition = &m_extenedPositions[layerpos][startPos];
	endPosition = &m_extenedPositions[layerpos][endPos];
}

void LayerPolyLine::getLineNormal(int linepos, trimesh::vec3*& normal, int layerpos)
{
	int pos = linepos * m_r;
	normal = &m_linesNormals[layerpos][pos];
}

int LayerPolyLine::getSpeedFlag(float speed)
{
	if (speed <=600.00)
	{
		return 0;
	} 
	else if(speed <= 1800.00)
	{
		return 1;
	}
	else if (speed <= 3000.0)
	{
		return 2;
	}
	else if (speed <= 4200.0)
	{
		return 3;
	}
	else if (speed <= 6000.0)
	{
		return 4;
	}
	else if (speed <= 9000.0)
	{
		return 5;
	}
	else if (speed <= 12000.0)
	{
		return 6;
	}
	else if (speed <= 18000.0)
	{
		return 7;
	}
	else if (speed <= 24000.0)
	{
		return 8;
	}
	else if (speed <= 30000.0)
	{
		return 9;
	}
}

int LayerPolyLine::callExtensionPointByNormals()
{
	for (int k = 0; k < m_points.size(); k++)
	{
		std::vector< LinePoint>& ptSet = m_points[k];
		std::vector<trimesh::vec3>& enSet = m_extenedNormals[k];
		std::vector<trimesh::vec3>& epSet = m_extenedPositions[k];

		for (int i = 0; i < ptSet.size(); i++)
		{
			int pos = i * m_r;

			trimesh::vec3 pt = ptSet[i].m_pt;
			trimesh::vec3* normal = &enSet[pos];
			for (int j = 0; j < m_r; j++)
			{
				trimesh::vec3& n = normal[j];
				trimesh::vec3 s = pt +n * m_sliceRadius[j];
				epSet.push_back(s);
			}
		}
	}

	return 0;
}

int LayerPolyLine::extentedNPoly(int npoly)
{
	initExtensionParam(npoly);

	if (m_lineAttributes.size() == 0 || m_points.size() == 0)
	{
		return 0;
	}

	m_linesNormals.resize(m_lineAttributes.size());
	for (int i = 0; i < m_lineAttributes.size(); i++)
	{
		m_linesNormals[i].reserve(m_lineAttributes[i].size() * m_r);
	}

	m_extenedNormals.resize(m_points.size());
	for (int i = 0; i < m_points.size(); i++)
	{
		m_extenedNormals.reserve(m_points[i].size() * m_r);
	}
	m_extenedPositions.resize(m_points.size());

	callLineNormals();
	changeLineNormals2PointNormals();

	callExtensionPointByNormals();

	return 1;
}

void LayerPolyLine::clearAll()
{
	m_points.clear();
	m_lineAttributes.clear();
	m_baseNormals.clear();
	m_extenedNormals.clear();
	m_extenedPositions.clear();
	m_linesNormals.clear();
}

int LayerPolyLine::initExtensionParam(int r)
{
	m_r = r;
	
	m_baseNormals.clear();

	float pi = 3.1415926f;
	float pi2 = pi * 2;
	float pid2 = pi / 2.0f;

	float deltaTheta = pi2 / (float)r;

	float sradius = m_thickness / 2.0f;
	float lradius = m_lineWidth / 2.0f;
	m_sliceRadius.resize(m_r);

	for (unsigned i = 0; i < r; ++i)
	{
		float a = deltaTheta * (float)i;

		float cosValue = cosf(a);
		float sinValue = sinf(a);

		m_baseNormals.push_back(trimesh::vec3(cosValue, sinValue, 0.0f));

		if (qAbs(cosValue) < 0.0001)
		{
			m_sliceRadius[i] = sradius;
		}
		else
		{
			double tana = sinValue / cosValue;
			double t = sqrt(lradius * lradius * tana * tana + sradius * sradius);
			double x = (lradius * sradius) / t;
			double y = (lradius * sradius * tana) / t;
			m_sliceRadius[i] = sqrt(x * x + y * y);
		}
	}

	return 0;
}

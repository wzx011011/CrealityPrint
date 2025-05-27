#include "splitjob.h"

#include "data/modeln.h"

#include "utils/matrixutil.h"
#include "trimesh2/TriMesh_algo.h"
#include "trimesh2/XForm.h"
#include <stack>

#include "utils/convexhullcalculator.h"
#include "qtuser3d/math/const.h"

#include "trimesh2/Vec3Utils.h"
#include "utils/polygoncache.h"
#include "mmesh/trimesh/polygonstack.h"
#include "data/modeln.h"
#include "data/modelgroup.h"

#include "interface/visualsceneinterface.h"
#include "interface/modelinterface.h"
#include "interface/cacheinterface.h"

#include "mmesh/trimesh/trimeshutil.h"
#include "splitslot/interface.h"
#include "splitslot/split.h"


using namespace creative_kernel;
SplitJob::SplitJob(QObject* parent)
	:Job(parent)
	, m_model(nullptr)
{
}

SplitJob::~SplitJob()
{
	qDeleteAll(m_meshes);
	m_meshes.clear();
}

QString SplitJob::name()
{
	return "Split";
}

QString SplitJob::description()
{
	return "";
}

void SplitJob::setModel(creative_kernel::ModelN* model)
{
	m_model = model;
}

void SplitJob::setPlane(const qtuser_3d::Plane& plane)
{
	m_plane = plane;
}

void SplitJob::work(qtuser_core::Progressor* progressor)
{
	if (!m_model) return;
#if 1
	splitslot::SplitSlotParam aparam;
	aparam.depth = 2.0;
	aparam.gap = 2.0;
	aparam.haveSlot = false;
	aparam.redius = 2.0;
	aparam.xyOffset = 0.3;
	aparam.zOffset = 0.3;

	QMatrix4x4 m = m_model->globalMatrix();

	trimesh::fxform xf = qMatrix2Xform(m);
	//QMatrix4x4 invm = m.inverted();
	splitslot::SplitPlane aplane;
	aplane.normal = m_plane.dir;
	aplane.position = /*invm * */m_plane.center;

	progressor->progress(0.1f);
	m_model->mesh()->bbox;

	trimesh::TriMesh amesh = *m_model->mesh();
	for (trimesh::point& apoint: amesh.vertices)
	{
		apoint = xf * apoint;
	}

	splitslot::splitSlot(&amesh, aplane, aparam, m_meshes);
	progressor->progress(0.9f);
	for (trimesh::TriMesh* amesh:m_meshes)
	{
		mmesh::dumplicateMesh(amesh);
		//calculate convex box
		ConvexHullCalculator::calculate(amesh, progressor);
	}
	progressor->progress(1.0f);
#else
	QMatrix4x4 m = m_model->globalMatrix();
	QMatrix4x4 invm = m.inverted();
	QVector3D local = invm * m_plane.center;
	QVector4D localDir = invm * QVector4D(m_plane.dir, 0.0f);
	QVector3D localD = localDir.toVector3D();
	localD.normalize();

	trimesh::TriMesh* mesh = m_model->mesh();
	trimesh::vec3 pos(local.x(), local.y(), local.z());
	trimesh::vec3 dir(localD.x(), localD.y(), localD.z());
	trimesh::box3 box = mesh->bbox;

	std::vector<float> distances;
	size_t vertex_size = mesh->vertices.size();

	if (vertex_size == 0)
		return;

	distances.resize(vertex_size);

#define min_value 1e-4
	bool allPositive = true;
	bool allNegtive = true;
	for (int i = 0; i < vertex_size; ++i)
	{
		trimesh::vec3 d = mesh->vertices.at(i) - pos;
		distances.at(i) = dir.dot(d);

		if (distances.at(i) < -min_value)
			allPositive = false;
		if (distances.at(i) > min_value)
			allNegtive = false;
	}

	if (allPositive || allNegtive)
		return;

	progressor->progress(0.1f);
	if (progressor->interrupt())
	{
		return;
	}

	std::vector<trimesh::TriMesh::Face> mesh1;
	std::vector<trimesh::TriMesh::Face> mesh2;
	std::vector<trimesh::TriMesh::Face> collideFaces;

	for (trimesh::TriMesh::Face& f : mesh->faces)
	{
		float l0 = distances.at(f.x);
		float l1 = distances.at(f.y);
		float l2 = distances.at(f.z);
		if (l0 > 0.0f && l1 > 0.0f && l2 > 0.0f)
		{
			mesh1.push_back(f);
		}else if (l0 < 0.0f && l1 < 0.0f && l2 < 0.0f)
		{
			mesh2.push_back(f);
		}
		else
			collideFaces.push_back(f);
	}

	progressor->progress(0.2f);
	if (progressor->interrupt())
		return;

	trimesh::TriMesh* m1 = new trimesh::TriMesh();
	trimesh::TriMesh* m2 = new trimesh::TriMesh();
	m_meshes.push_back(m1);
	m_meshes.push_back(m2);

	m1->faces.swap(mesh1);
	m2->faces.swap(mesh2);

	auto fillmesh = [&mesh](trimesh::TriMesh* m) {
		for (trimesh::TriMesh::Face& f : m->faces)
		{
			m->vertices.push_back(mesh->vertices.at(f.x));
			m->vertices.push_back(mesh->vertices.at(f.y));
			m->vertices.push_back(mesh->vertices.at(f.z));
		}

		int index = 0;
		//remap
		for (trimesh::TriMesh::Face& f : m->faces)
		{
			f.x = index++;
			f.y = index++;
			f.z = index++;
		}
	};

	fillmesh(m1);
	progressor->progress(0.3f);
	if (progressor->interrupt())
		return;

	fillmesh(m2);
	progressor->progress(0.4f);
	if (progressor->interrupt())
		return;

	auto addmesh = [](trimesh::TriMesh* mesh, const trimesh::vec3& v1, const trimesh::vec3& v2, const trimesh::vec3& v3) {
		int index = (int)mesh->vertices.size();
		mesh->vertices.push_back(v1);
		mesh->vertices.push_back(v2);
		mesh->vertices.push_back(v3);

		mesh->faces.push_back(trimesh::TriMesh::Face(index, index + 1, index + 2));
	};

	std::vector<trimesh::vec3> lines;
	//process collide
	auto fcollid = [&box, &addmesh, &mesh, &distances, &m1, &m2, &lines](int t, int i1, int i2) 
	{
		trimesh::vec3& tv = mesh->vertices.at(t);
		trimesh::vec3& v1 = mesh->vertices.at(i1);
		trimesh::vec3& v2 = mesh->vertices.at(i2);
		float dv = distances.at(t);
		float d1 = distances.at(i1);
		float d2 = distances.at(i2);
		
		if (d1 == 0.0f && d2 == 0.0f)
		{
			if (dv >= 0.0f)
			{
				addmesh(m1, tv, v1, v2);
			
				lines.push_back(v1);
				lines.push_back(v2);
			}
			else
			{
				addmesh(m2, tv, v1, v2);
			
				lines.push_back(v2);
				lines.push_back(v1);
			}
		}
		else if (d1 == 0.0f && dv * d2 >= 0.0f)
		{
			if (dv > 0.0f || dv == 0.0f && d2 > 0.0)
			{
				addmesh(m1, tv, v1, v2);
				if (dv == 0.0f)
				{
					lines.push_back(tv);
					lines.push_back(v1);
				}
			}
			else
			{
				addmesh(m2, tv, v1, v2);
				if (dv == 0.0f)
				{
					lines.push_back(v1);
					lines.push_back(tv);
				}
			}
		}
		else if (d2 == 0.0f && dv * d1 >= 0.0f)
		{
			if (dv > 0.0f || dv == 0.0f && d1 > 0.0f)
			{
				addmesh(m1, tv, v1, v2);
				if (dv == 0.0f)
				{
					lines.push_back(v2);
					lines.push_back(tv);
				}
			}
			else
			{
				addmesh(m2, tv, v1, v2);
				if (dv == 0.0f)
				{
					lines.push_back(tv);
					lines.push_back(v2);
				}
			}
		}
		else
		{
			trimesh::vec3 c1 = (dv / (dv - d1)) * v1 - (d1 / (dv - d1)) * tv;
			trimesh::vec3 c2 = (dv / (dv - d2)) * v2 - (d2 / (dv - d2)) * tv;

#ifdef _DEBUG
			if (c1.x < box.min.x || c1.x > box.max.x || c1.y < box.min.y || c1.y > box.max.y
				|| c2.x < box.min.x || c2.x > box.max.x || c2.y < box.min.y || c2.y > box.max.y)
			{
				printf("error");
			}
#endif

			if (dv > 0.0f)
			{
				addmesh(m1, tv, c1, c2);
				addmesh(m2, c2, c1, v2);
				addmesh(m2, c1, v1, v2);

				lines.push_back(c1);
				lines.push_back(c2);
			}
			else if(dv < 0.0f)
			{
				addmesh(m2, tv, c1, c2);
				addmesh(m1, c2, c1, v2);
				addmesh(m1, c1, v1, v2);

				lines.push_back(c2);
				lines.push_back(c1);
			}
			else
			{
				if (d1 > 0.0f)
				{
					addmesh(m1, tv, v1, v2);
				}
				else if (d1 < 0.0f)
				{
					addmesh(m2, tv, v1, v2);
				}
			}
		}
	};

	int faceNum = (int)mesh->faces.size();
	for (int i = 0; i < faceNum; ++i)
	{
		trimesh::TriMesh::Face& f = mesh->faces.at(i);
		float l0 = distances.at(f.x) * distances.at(f.y);
		float l1 = distances.at(f.y) * distances.at(f.z);
		float l2 = distances.at(f.x) * distances.at(f.z);
		if (distances.at(f.x) == 0.0f && distances.at(f.y) == 0.0f && distances.at(f.z) == 0.0f)
			continue;

		if (l0 >= 0.0f && (l1 <= 0.0f || l2 <= 0.0f))
		{
			fcollid(f.z, f.x, f.y);
		}
		else if (l0 < 0.0f)
		{
			if (l1 <= 0.0f)
			{
				fcollid(f.y, f.z, f.x);
			}
			else if (l2 <= 0.0f)
			{
				fcollid(f.x, f.y, f.z);
			}
		}

		if (i % 1000 == 0)
		{
			progressor->progress(0.5f * (float)i / (float)faceNum + 0.4f);
			if (progressor->interrupt())
			{
				break;
			}
		}
	}

	progressor->progress(0.7f);

	//fill hole
	lines2polygon(lines, m_polygons, m_points);

	progressor->progress(0.85f);
	if (progressor->interrupt())
	{
		return;
	}

	std::vector<trimesh::TriMesh::Face> faces;
	std::vector<trimesh::dvec2> polygons2;
	polygons2.reserve(m_points.size());

	trimesh::vec3 zn = trimesh::vec3(0.0f, 0.0f, 1.0f);
	trimesh::vec3 axis = dir TRICROSS zn;
	float angle = trimesh::vv_angle(axis, zn);
	trimesh::xform r = trimesh::xform::rot((double)angle, axis);
	for (size_t i = 0; i < m_points.size(); ++i)
	{
		trimesh::vec3 v = m_points.at(i);
		trimesh::dvec3 dv = trimesh::dvec3(v.x, v.y, v.z);
		trimesh::dvec3 p = r * dv;
		polygons2.push_back(trimesh::dvec2(p.x, p.y));
	}

	mmesh::PolygonStack pstack;
	pstack.generates(m_polygons, polygons2, faces, 0);

#if 0
	creative_kernel::PolygonCache cache;
	cache.m_points = polygons2;
	cache.m_polygons = m_polygons;
	cache.save(QString("%1").arg((long)(&polygons2)));
#endif

	progressor->progress(0.95f);
	if (progressor->interrupt())
	{
		return;
	}

	bool fillHole = true;
	if (fillHole)
	{
		int start1 = (int)m1->vertices.size();
		int start2 = (int)m2->vertices.size();

		m1->vertices.insert(m1->vertices.end(), m_points.begin(), m_points.end());
		m2->vertices.insert(m2->vertices.end(), m_points.begin(), m_points.end());
		for (trimesh::TriMesh::Face& fs : faces)
		{
			trimesh::TriMesh::Face f1 = fs;
			f1 += trimesh::ivec3(start1, start1, start1);
			int t = f1[2];
			f1[2] = f1[1];
			f1[1] = t;
			m1->faces.push_back(f1);

			trimesh::TriMesh::Face f2 = fs;
			f2 += trimesh::ivec3(start2, start2, start2);
			m2->faces.push_back(f2);
		}
	}

	mmesh::dumplicateMesh(m1);
	mmesh::dumplicateMesh(m2);
	//calculate convex box
	ConvexHullCalculator::calculate(m1, progressor);
	ConvexHullCalculator::calculate(m2, progressor);
	progressor->progress(1.0f);
#endif
}

void SplitJob::failed()
{

}

void SplitJob::successed(qtuser_core::Progressor* progressor)
{
	creative_kernel::ModelGroup* group = qobject_cast<creative_kernel::ModelGroup*>(m_model->parent());
	if (m_meshes.size() > 0 && group)
	{
		QList<ModelN*> newModels;
		QString name = m_model->objectName().left(m_model->objectName().lastIndexOf("."));
		QString suffix = m_model->objectName().right(m_model->objectName().length() - m_model->objectName().lastIndexOf("."));
		int id = 0;
		for (trimesh::TriMesh* mesh : m_meshes)
		{
			if (mesh->vertices.size() == 0 || mesh->faces.size() == 0)
			{
				delete mesh;
				continue;
			}
			++id;

			creative_kernel::ModelN* m = new creative_kernel::ModelN();
			QString subName = QString("%1-split-%2").arg(name).arg(id) + suffix;
			m->setObjectName(subName);

			//trimesh::fxform xf = qMatrix2Xform(m_model->globalMatrix());
			size_t vertexCount = mesh->vertices.size();
			for (size_t i = 0; i < vertexCount; ++i)
			{
				trimesh::vec3 v = mesh->vertices.at(i);
				mesh->vertices.at(i) = /*xf **/ v;
			}
			
			mesh->clear_bbox();
			mesh->need_bbox();
			trimesh::vec3 offset = mmesh::moveTrimesh2Center(mesh);

			TriMeshPtr ptr(mesh);
			creative_kernel::cache(ptr);
			m->setTrimesh(ptr);		
			m->setLocalPosition(-QVector3D(offset.x, offset.y, offset.z));
			m->SetInitPosition(m->localPosition());
			//m->needDetectError();
			newModels.push_back(m);
		}
		m_meshes.clear();

		if (newModels.size() > 0)
		{
			QList<ModelN*> removes;
			removes << m_model;

			modifyGroup(removes, newModels, group, true);
		}

		requestVisUpdate(true);
	}
}

void SplitJob::lines2polygon(std::vector<trimesh::vec3>& lines, std::vector<std::vector<int>>& polygons, std::vector<trimesh::vec3>& uniPoints)
{
	size_t size = lines.size();
	size_t segsize = size / 2;

	class point_cmp
	{
	public:
		point_cmp(float e = FLT_MIN) :eps(e) {}

		bool operator()(const trimesh::vec3& v0, const trimesh::vec3& v1) const
		{
			if (fabs(v0.x - v1.x) <= eps)
			{
				if (fabs(v0.y - v1.y) <= eps)
				{
					return (v0.z < v1.z - eps);
				}
				else return (v0.y < v1.y - eps);
			}
			else return (v0.x < v1.x - eps);
		}
	private:
		float eps;
	};

	typedef std::map<trimesh::vec3, int, point_cmp> unique_point;
	typedef unique_point::iterator point_iterator;

	struct segment
	{
		int start;
		int end;
	};

	typedef std::map<trimesh::vec3, int, point_cmp> unique_point;
	typedef unique_point::iterator point_iterator;
	unique_point points;

	auto f = [&points](const trimesh::vec3& v)->int {
		int index = -1;
		point_iterator it = points.find(v);
		if (it != points.end())
		{
			index = (*it).second;
		}
		else
		{
			index = (int)points.size();
			points.insert(unique_point::value_type(v, index));
		}

		return index;
	};

	std::vector<segment> segments(segsize);
	for (size_t i = 0; i < segsize; ++i)
	{
		trimesh::vec3 v1 = lines.at(2 * i);
		trimesh::vec3 v2 = lines.at(2 * i + 1);

		segments.at(i).start = f(v1);
		segments.at(i).end = f(v2);
	}

	std::vector<trimesh::vec3> vecpoints(points.size());
	for (auto it = points.begin(); it != points.end(); ++it)
	{
		vecpoints.at((*it).second) = (*it).first;
	}

	std::vector<segment*> segmap(points.size(), nullptr);
	for (segment& s : segments)
	{
		segmap.at(s.start) = &s;
	}

	std::vector<bool> used(points.size(), false);
	
	auto check = [&used]() ->int{
		int index = -1;
		size_t size = used.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (!used.at(i))
			{
				index = (int)i;
				break;
			}
		}
		return index;
	};

	struct IndexPolygon
	{
		std::list<int> polygon;
		int start;
		int end;

		bool closed()
		{
			return (polygon.size() >= 2) && (polygon.front() == polygon.back());
		}
	};

	std::vector<IndexPolygon> indexPolygons;
	int index = check();
	while (index >= 0)
	{
		used.at(index) = true;
		segment* seg = segmap.at(index);
		if (seg)
		{
			int s = seg->start;
			int e = seg->end;

			bool find = false;
			for (IndexPolygon& polygon : indexPolygons)
			{
				if (s == polygon.end)
				{
					polygon.polygon.push_back(e);
					polygon.end = e;
					find = true;
				}
				else if (e == polygon.start)
				{
					polygon.polygon.push_front(s);
					polygon.start = s;
					find = true;
				}

				if (find)
					break;
			}

			if (!find)
			{
				IndexPolygon polygon;
				polygon.polygon.push_back(s);
				polygon.polygon.push_back(e);
				polygon.start = s;
				polygon.end = e;
				indexPolygons.emplace_back(polygon);
			}
		}
		index = check();
	}
	size_t indexPolygonSize = indexPolygons.size();
	std::map<int, IndexPolygon*> IndexPolygonMap;
	for (size_t i = 0; i < indexPolygonSize; ++i)
	{
		IndexPolygon& p1 = indexPolygons.at(i);
		if(!p1.closed())
			IndexPolygonMap.insert(std::pair<int, IndexPolygon*>(p1.start, &p1));
	}

	////sort
	//for (size_t i = 0; i < indexPolygonSize; ++i)
	//{
	//	IndexPolygon& p1 = indexPolygons.at(i);
	//	for (size_t j = i + 1; j < indexPolygonSize; ++j)
	//	{
	//		IndexPolygon& p2 = indexPolygons.at(j);

	//		if (p1.end > p2.start)
	//		{
	//			std::swap(p1.polygon, p2.polygon);
	//			std::swap(p1.start, p2.start);
	//			std::swap(p1.end, p2.end);
	//		}
	//	}
	//}
	//combime
	for (size_t i = 0; i < indexPolygonSize; ++i)
	{
		IndexPolygon& p1 = indexPolygons.at(i);

		if (p1.polygon.size() == 0 || p1.closed())
			continue;

		auto it = IndexPolygonMap.find(p1.end);
		while (it != IndexPolygonMap.end())
		{

			IndexPolygon& p2 = *(*it).second;
			if (p2.polygon.size() == 0)
				break;

			bool merged = false;
			if (p1.start == p2.end)
			{
				p1.start = p2.start;
				for (auto iter = p2.polygon.rbegin(); iter != p2.polygon.rend(); ++iter)
				{
					if ((*iter) != p1.polygon.front()) p1.polygon.push_front((*iter));
				}
				merged = true;
			}
			else if (p1.end == p2.start)
			{
				p1.end = p2.end;
				for (auto iter = p2.polygon.begin(); iter != p2.polygon.end(); ++iter)
				{
					if ((*iter) != p1.polygon.back()) p1.polygon.push_back((*iter));
				}
				merged = true;
			}

			if (merged)
			{
				p2.polygon.clear();
			}
			else
				break;

			it = IndexPolygonMap.find(p1.end);
		}

		//for (size_t j = i + 1; j < indexPolygonSize; ++j)
		//{
		//	IndexPolygon& p2 = indexPolygons.at(j);
		//	if (p2.polygon.size() == 0)
		//		continue;

		//	bool merged = false;
		//	if (p1.start == p2.end)
		//	{
		//		p1.start = p2.start;
		//		for (auto it = p2.polygon.rbegin(); it != p2.polygon.rend(); ++it)
		//		{
		//			if ((*it) != p1.polygon.front()) p1.polygon.push_front((*it));
		//		}
		//		merged = true;
		//	}else if (p1.end == p2.start)
		//	{
		//		p1.end = p2.end;
		//		for (auto it = p2.polygon.begin(); it != p2.polygon.end(); ++it)
		//		{
		//			if ((*it) != p1.polygon.back()) p1.polygon.push_back((*it));
		//		}
		//		merged = true;
		//	}

		//	if (merged)
		//	{
		//		p2.polygon.clear();
		//	}
		//}
	}

	size_t polygonSize = indexPolygons.size();
	if (polygonSize > 0)
	{
		polygons.reserve(polygonSize);
		for (size_t i = 0; i < polygonSize; ++i)
		{
			std::vector<int> polygon;
			IndexPolygon& ipolygon = indexPolygons.at(i);
			for (int iindex : ipolygon.polygon)
			{
				polygon.push_back(iindex);
			}

			if (polygon.size() > 0)
			{
				polygons.emplace_back(polygon);
			}
		}
	}
	uniPoints.swap(vecpoints);
}

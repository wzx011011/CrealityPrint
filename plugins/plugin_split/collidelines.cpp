#include "collidelines.h"
#include "trimesh2/XForm.h"
#include "utils/matrixutil.h"

CollideLines::CollideLines(QObject* parent)
	:QObject(parent)
{
}

CollideLines::~CollideLines()
{
}

//{
//	QMatrix4x4 m = model->matrix();
//	QMatrix4x4 invm = m.inverted();
//	QVector3D local = invm * plane.center;
//	QVector4D localDir = invm * QVector4D(plane.dir, 0.0f);
//	QVector3D localD = localDir.toVector3D();
//	localD.normalize();
//
//	trimesh::TriMesh* mesh = model->mesh();
//	trimesh::vec3 pos(local.x(), local.y(), local.z());
//	trimesh::vec3 dir(localD.x(), localD.y(), localD.z());
//
//	std::vector<float> distances;
//	size_t vertex_size = mesh->vertices.size();
//	distances.resize(vertex_size);
//
//	for (int i = 0; i < vertex_size; ++i)
//	{
//		trimesh::vec3 d = mesh->vertices.at(i) - pos;
//		distances.at(i) = dir.dot(d);
//	}
//
//	trimesh::fxform xf = qMatrix2Xform(m);
//	auto fcollid = [&lines, &xf, &mesh, &distances](int t, int i1, int i2) {
//		trimesh::vec3& tv = mesh->vertices.at(t);
//		trimesh::vec3& v1 = mesh->vertices.at(i1);
//		trimesh::vec3& v2 = mesh->vertices.at(i2);
//		float dv = distances.at(t);
//		float d1 = distances.at(i1);
//		float d2 = distances.at(i2);
//
//		trimesh::vec3 c1 = (dv / (dv - d1)) * v1 - (d1 / (dv - d1)) * tv;
//		trimesh::vec3 c2 = (dv / (dv - d2)) * v2 - (d2 / (dv - d2)) * tv;
//
//		lines.push_back(xf * c1);
//		lines.push_back(xf * c2);
//	};
//
//	lines.reserve(vertex_size);
//	for (trimesh::TriMesh::Face& f : mesh->faces)
//	{
//		float l0 = distances.at(f.x) * distances.at(f.y);
//		float l1 = distances.at(f.y) * distances.at(f.z);
//		float l2 = distances.at(f.x)* distances.at(f.z);
//		if (l0 == 0.0f && l1 == 0.0f && l2 == 0.0f)
//			continue;
//
//		if (l0 >= 0.0f && (l1 <= 0.0f || l2 <= 0.0f))
//		{
//			fcollid(f.z, f.x, f.y);
//		}
//		else if (l0 < 0.0f)
//		{
//			if (l1 <= 0.0f)
//			{
//				fcollid(f.y, f.x, f.z);
//			}
//			else if (l2 <= 0.0f)
//			{
//				fcollid(f.x, f.z, f.y);
//			}
//		}
//	}
//}

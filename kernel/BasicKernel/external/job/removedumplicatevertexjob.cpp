#include "removedumplicatevertexjob.h"
#include "external/job/meshloadinfo.h"

#include "qtuser3d/math/const.h"
#include <map>
#include <vector>
#include <unordered_map>
#include <QtCore/QTime>
#include <QtCore/QDebug>

#include "qtusercore/module/profiler.h"
namespace creative_kernel
{
	RemoveDumplicateVertexJob::RemoveDumplicateVertexJob(QObject* parent)
		:Job(parent)
		, m_meshLoadInfo(nullptr)
	{
	}

	RemoveDumplicateVertexJob::~RemoveDumplicateVertexJob()
	{
	}

	QString RemoveDumplicateVertexJob::name()
	{
		return "RemoveDumplicateVertex";
	}

	QString RemoveDumplicateVertexJob::description()
	{
		return "";
	}

	void RemoveDumplicateVertexJob::failed()
	{
		qDebug() << "fail";
	}

	void RemoveDumplicateVertexJob::successed(qtuser_core::Progressor* progressor)
	{
		qDebug() << "success";
	}

	void RemoveDumplicateVertexJob::work(qtuser_core::Progressor* progressor)
	{
		qDebug() << "work";
		if (m_meshLoadInfo && m_meshLoadInfo->m_mesh && m_meshLoadInfo->m_mesh->faceUVs.empty())
		{
			QStringList keys;
			keys << "time";
			PROFILE_START("dumplicate", keys);
			
			QTime t;
			t.start();
			hash_(progressor);

			PROFILE_TICK("dumplicate");

			qDebug() << t.elapsed();
			if (m_meshLoadInfo->m_optimizedMesh)//add by yangchengwen 
				qDebug() << "vertex Num " << m_meshLoadInfo->m_optimizedMesh->vertices.size();

			PROFILE_OUTPUT("dumplicate", "dumplicate.csv");
		}
	}

	void RemoveDumplicateVertexJob::hash_(qtuser_core::Progressor* progressor)
	{
		trimesh::TriMesh* mesh = m_meshLoadInfo->m_mesh;
		size_t vertexNum = mesh->vertices.size();

		struct hash_vec3
		{
			size_t operator()(const trimesh::vec3& v)const
			{
				return abs(v.x) * 10000 / 23 + abs(v.y) * 10000 / 19 + abs(v.z) * 10000 / 17;
			}
		};
		struct equal_vec3
		{
			bool operator()(const trimesh::vec3& v1, const trimesh::vec3& v2) const
			{
				return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
			}
		};
		typedef std::unordered_map<trimesh::vec3, int, hash_vec3, equal_vec3> unique_point;
		unique_point points(vertexNum * 3 / 10 + 1);

		typedef unique_point::iterator point_iterator;

		size_t faceNum = mesh->faces.size();

		if (vertexNum == 0 || faceNum == 0)
			return;

		m_meshLoadInfo->m_optimizedMesh = new trimesh::TriMesh();


		std::vector<int> vertexMapper;
		vertexMapper.resize(vertexNum, -1);

		bool interupt = false;
		progressor->progress(0.1f);
		for (size_t i = 0; i < vertexNum; ++i)
		{
			trimesh::point p = mesh->vertices.at(i);
			point_iterator it = points.find(p);
			if (it != points.end())
			{
				int index = (*it).second;
				vertexMapper.at(i) = index;
			}
			else
			{
				int index = (int)points.size();
				points.insert(unique_point::value_type(p, index));

				vertexMapper.at(i) = index;
			}


			if (i % 4000 == 1)
			{
				progressor->progress(0.65f * ((float)i / (float)vertexNum) + 0.1f);
				if (progressor->interrupt())
				{
					interupt = true;
					break;
				}
			}
		}

		progressor->progress(0.75f);
		if (interupt || progressor->interrupt())
		{
			return;
		}

		trimesh::TriMesh* omesh = m_meshLoadInfo->m_optimizedMesh;
		omesh->vertices.resize(points.size());
		for (point_iterator it = points.begin(); it != points.end(); ++it)
		{
			omesh->vertices.at(it->second) = it->first;
		}

		progressor->progress(0.8f);
		if (progressor->interrupt())
		{
			return;
		}

		omesh->faces.swap(mesh->faces);
		for (size_t i = 0; i < faceNum; ++i)
		{
			trimesh::TriMesh::Face& of = omesh->faces.at(i);
			for (int j = 0; j < 3; ++j)
			{
				int index = of[j];
				of[j] = vertexMapper[index];
			}

			if (i % 10000 == 1)
			{
				progressor->progress(0.2f * ((float)i / (float)faceNum) + 0.8f);
				if (progressor->interrupt())
				{
					break;
				}
			}
		}

		progressor->progress(1.0f);
	}
}

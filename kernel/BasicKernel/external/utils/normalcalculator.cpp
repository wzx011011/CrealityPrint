#include "normalcalculator.h"
#include <qdebug.h>

NormalCalculator::NormalCalculator(QObject* parent)
	:QObject(parent)
{
}
NormalCalculator::~NormalCalculator()
{
}

void NormalCalculator::calculate(trimesh::TriMesh* mesh, qtuser_core::Progressor* progressor)
{
	float d = 0.1f;

	int nv = mesh->vertices.size();
	if (!nv || int(mesh->normals.size()) == nv)
		return;

	mesh->normals.clear();
	mesh->normals.resize(nv);

	qDebug() << "load>>> NormalCalculator::calculate";

	progressor->progress(0.15f);
	bool interrupt = false;
	int nf = mesh->faces.size();
	for (int i = 0; i < nf; i++)
	{
		const trimesh::point& p0 = mesh->vertices[mesh->faces[i][0]];
		const trimesh::point& p1 = mesh->vertices[mesh->faces[i][1]];
		const trimesh::point& p2 = mesh->vertices[mesh->faces[i][2]];
		trimesh::vec a = p0 - p1, b = p1 - p2, c = p2 - p0;
		float l2a = trimesh::len2(a), l2b = len2(b), l2c = len2(c);
		if (!l2a || !l2b || !l2c)
			continue;
		trimesh::vec facenormal = a TRICROSS b;
		trimesh::normalize(facenormal);
		mesh->normals[mesh->faces[i][0]] += facenormal;// * (1.0f / (l2a * l2c));
		mesh->normals[mesh->faces[i][1]] += facenormal;// * (1.0f / (l2b * l2a));
		mesh->normals[mesh->faces[i][2]] += facenormal;// * (1.0f / (l2c * l2b));

		if (i % 10000 == 1)
		{
			progressor->progress(0.75f * ((float)i / (float)nf) + 0.15f);
			if (progressor->interrupt())
			{
				interrupt = true;
				break;
			}
		}
	}

	progressor->progress(0.9f);
	if (!interrupt)
	{
		for (int i = 0; i < nv; i++)
			trimesh::normalize(mesh->normals[i]);
	}
}

#include "slicemodel.h"
#include "core/layerlines.h"
#include "trimesh2/TriMesh.h"
#include "mmesh/trimesh/trimeshutil.h"

SliceModel::SliceModel()
{
	colorType = 1;
	isThirdparty = false;
}

SliceModel::~SliceModel()
{
}

void SliceModel::save(const QString& name)
{
	int layerNum = (int)m_sliceLayers.size();
	if (layerNum == 0)
		return;

	std::vector< LayerPolyLine> slicePolys;
	int count = 0;
	float layer = 0.0f;

	unsigned r = 4;

	for (const SliceLayer& sliceLayer : m_sliceLayers)
	{
		float step = 0.0f;

		slicePolys.push_back(LayerPolyLine());

		LayerPolyLine& lpl = slicePolys.back();
		lpl.m_layer = layer;
		lpl.m_thickness = sliceLayer.thickness;
		lpl.m_lineWidth = lpl.m_thickness;

		for (const SliceLine& sliceLine : sliceLayer.m_sliceLines)
		{
			if (sliceLine.type != SliceLineType::NoneType)
			{
				lpl.addLine(sliceLine, step);

				++count;
				//QVector3D start(sliceLine.start.at(0), sliceLine.start.at(1), sliceLine.start.at(2));
				//QVector3D end(sliceLine.end.at(0), sliceLine.end.at(1), sliceLine.end.at(2));
				//m_box += start;
				//m_box += end;

				step += 1.0f;
			}
		}
		layer += 1.0f;

		lpl.extentedNPoly(r);
	}

	unsigned vertexNum = count * 2 * r;
	std::vector<trimesh::vec3> positions;
	positions.reserve(vertexNum);
	unsigned triangleNum = count * 3 * r;
	std::vector<trimesh::uivec3> indices;
	indices.reserve(triangleNum);

	unsigned vertexIndex = 0;

	float step = 0;

	for (LayerPolyLine& lpl : slicePolys)
	{
		for (int l = 0; l < lpl.m_lineAttributes.size(); l++)
		{
			std::vector< SliceLineAttributes>& slaSet = lpl.m_lineAttributes[l];

			for (int k = 0; k < slaSet.size(); k++)
				//for (SliceLineAttributes& sla : lpl.m_lineAttributes)
			{
				SliceLineAttributes& sla = slaSet[k];

				LinePoint* start = nullptr, * end = nullptr;
				lpl.getLineStartEnd(sla, start, end, l);

				trimesh::vec3* normal;
				trimesh::vec3* startPosition = nullptr, * endPosition = nullptr;

				lpl.getPointPositon(sla, startPosition, endPosition, l);
				lpl.getLineNormal(k, normal, l);

				for (int i = 0; i < r; i++)
				{
					positions.push_back(*startPosition++);
					positions.push_back(*endPosition++);
				}
				step = sla.step;

				for (unsigned i = 0; i < r; ++i)
				{
					indices.push_back(trimesh::uivec3(vertexIndex + 2 * i, vertexIndex + 2 * i + 1, vertexIndex + ((2 * i + 3) % (2 * r))));
					indices.push_back(trimesh::uivec3(vertexIndex + 2 * i, vertexIndex + ((2 * i + 3) % (2 * r)), vertexIndex + ((2 * i + 2) % (2 * r))));
				}

				if (start->m_type == LinePointType::StartPoint)
				{
					indices.push_back(trimesh::uivec3(vertexIndex, vertexIndex + 2, vertexIndex + 4));
					indices.push_back(trimesh::uivec3(vertexIndex, vertexIndex + 4, vertexIndex + 6));
				}
				if (end->m_type == LinePointType::EndPoint)
				{
					indices.push_back(trimesh::uivec3(vertexIndex + 1, vertexIndex + 3, vertexIndex + 5));
					indices.push_back(trimesh::uivec3(vertexIndex + 1, vertexIndex + 5, vertexIndex + 7));
				}

				vertexIndex += 2 * r;
			}

		}
		lpl.clearAll();
	}

	int vertexSize = positions.size();
	int triangleSize = indices.size();
	if (vertexSize && triangleSize)
	{
		trimesh::TriMesh* mesh = new trimesh::TriMesh();
		mesh->vertices.resize(vertexSize);
		mesh->faces.resize(triangleSize);

		memcpy(&mesh->vertices.at(0), &positions.at(0), 3 * vertexSize * sizeof(float));
		memcpy(&mesh->faces.at(0), &indices.at(0), 3 * triangleSize * sizeof(int));

		mmesh::reverseTriMesh(mesh);
		mesh->write(name.toStdString());
		delete mesh;
	}
}

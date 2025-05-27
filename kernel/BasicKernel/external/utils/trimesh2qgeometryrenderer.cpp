#include "trimesh2qgeometryrenderer.h"
#include "trimesh2/TriMesh.h"

#include <Qt3DRender/QBuffer>
#include "qtuser3d/geometry/geometrycreatehelper.h"
#include "qtuser3d/math/angles.h"
#include <QtGui/qvector3d.h>
#include <vector>
#include <map>
#include <omp.h>
#include <QTime>
#include <unordered_map>


//Trimesh2Geometry(QObject* parent)
//{
//	//
//}
//
//~Trimesh2Geometry()
//{
//	//
//}


int twoFaceCollinear(const trimesh::TriMesh::Face& f1, const trimesh::TriMesh::Face& f2, std::vector<int>& faceLine)
{
	faceLine.clear();
	std::map<int, int> fmap;
	for (int i = 0; i < 3; i++)
	{
		fmap[f1[i]] = 1;
	}
	for (int i = 0; i < 3; i++)
	{
		if (fmap.find(f2[i]) != fmap.end())
		{
			faceLine.push_back(f2[i]);
		}
	}
	return faceLine.size();
}



creative_kernel::GeometryData* genGeometryFaceData(trimesh::TriMesh* mesh, qtuser_core::Progressor* progressor)
{
	if (!mesh || (mesh->faces.size() == 0) || (mesh->vertices.size() == 0)) return nullptr;

	creative_kernel::GeometryData* geometryData = new creative_kernel::GeometryData();

	QTime qtime;
	qtime.start();

	if (progressor != nullptr)
	{
		progressor->progress(0.0f);
	}

	int fcount = (int)mesh->faces.size();
	int count = fcount * 3;

	geometryData->m_fcount = fcount;
	geometryData->m_count = count;

	QByteArray &positionByteArray = geometryData->m_positionByteArray;
	QByteArray &normalByteArray = geometryData->m_normalByteArray;
	QByteArray &texcoordArray = geometryData->m_texcoordArray;

	positionByteArray.resize(count * 3 * sizeof(float));
	normalByteArray.resize(count * 3 * sizeof(float));
	texcoordArray.resize(count * 2 * sizeof(float));

	if (count == (int)mesh->vertices.size())
	{
		memcpy(positionByteArray.data(), &mesh->vertices[0], positionByteArray.size());
		if (mesh->normals.size() != count)
		{
			mesh->need_normals();
			memcpy(normalByteArray.data(), &mesh->normals[0], normalByteArray.size());
		}
	}
	else
	{
		trimesh::vec3* vertexData = (trimesh::vec3*)positionByteArray.data();
		trimesh::vec3* normalData = (trimesh::vec3*)normalByteArray.data();
		trimesh::vec2* texcoordData = (trimesh::vec2*)texcoordArray.data();
//#pragma omp parallel for shared(vertexData, mesh)
		for (int i = 0; i < fcount; ++i)
		{
			trimesh::TriMesh::Face& f = mesh->faces.at(i);
			for (int j = 0; j < 3; ++j)
			{
				vertexData[i * 3 + j] = mesh->vertices.at(f[j]);
			}

			// 向量计算时最好用双精度数，特别是叉乘和归一化，尽量不要用单精度去存储中间结果，以避免精度丢失，进而对临界值的计算造成影响
			trimesh::vec3 v01 = mesh->vertices.at(f[1]) - mesh->vertices.at(f[0]);
			trimesh::dvec3 v01d = trimesh::dvec3(v01.x, v01.y, v01.z);
			trimesh::vec3 v02 = mesh->vertices.at(f[2]) - mesh->vertices.at(f[0]);
			trimesh::dvec3 v02d = trimesh::dvec3(v02.x, v02.y, v02.z);
			//trimesh::vec3 n = v01 TRICROSS v02;
			trimesh::dvec3 nd = v01d TRICROSS v02d;
			//trimesh::normalize(n);
			trimesh::normalize(nd);

			for (int j = 0; j < 3; ++j)
			{
				//normalData[i * 3 + j] = n;
				normalData[i * 3 + j] = nd;
			}

			if (texcoordArray.size())
			{
				if (i >= mesh->faceUVs.size()
					|| i >= mesh->textureIDs.size())
				{
					//确保传入shader中的数据正常
					texcoordData[3 * i] = trimesh::vec2(-1.0, -1.0);
					texcoordData[3 * i+1] = trimesh::vec2(-1.0, -1.0);
					texcoordData[3 * i+2] = trimesh::vec2(-1.0, -1.0);
				}
				else
				{
					trimesh::TriMesh::Face& faceUV = mesh->faceUVs.at(i);
					int textureID = mesh->textureIDs[i];
					switch (textureID)
					{
					default:
					{
						trimesh::vec2 uv0(-1.0, -1.0);
						trimesh::vec2 uv1(-1.0, -1.0);
						trimesh::vec2 uv2(-1.0, -1.0);

						int textureID = mesh->textureIDs[i];
						if (textureID > -1 && !mesh->materials.empty())
						{
							const trimesh::Material& material = mesh->materials[textureID];
							const trimesh::vec2& map_startUV = material.map_startUVs[trimesh::Material::MapType::DIFFUSE];
							const trimesh::vec2& map_endUV = material.map_endUVs[trimesh::Material::MapType::DIFFUSE];
							int UVsizeMax = mesh->UVs.size();
							if (faceUV.x > -1.0 && (faceUV.x < UVsizeMax))
							{
								uv0 = mesh->UVs.at(faceUV.x);
								uv0 = map_startUV + uv0 * (map_endUV - map_startUV);
							}
							if (faceUV.y > -1.0 && (faceUV.y < UVsizeMax))
							{
								uv1 = mesh->UVs.at(faceUV.y);
								uv1 = map_startUV + uv1 * (map_endUV - map_startUV);
							}
							if (faceUV.z > -1.0 && (faceUV.z < UVsizeMax))
							{
								uv2 = mesh->UVs.at(faceUV.z);
								uv2 = map_startUV + uv2 * (map_endUV - map_startUV);
							}
						}

						texcoordData[3 * i] = trimesh::vec2(uv0.x, uv0.y);
						texcoordData[3 * i + 1] = trimesh::vec2(uv1.x, uv1.y);
						texcoordData[3 * i + 2] = trimesh::vec2(uv2.x, uv2.y);
					}
					}
				}
			}

			if (progressor != nullptr)
			{
				if ((i + 1) % 10000 == 0)
				{
					progressor->progress((float)i / (float)fcount);
					if (progressor->interrupt())
					{
						if (geometryData != nullptr)
						{
							delete geometryData;
							return nullptr;
						}
					}
				}
			}

		}
	}
	if (progressor != nullptr)
	{
		progressor->progress(1.0f);
	}
	qDebug() << "genGeometryFaceData run time = " << qtime.elapsed();
	return geometryData;
}

creative_kernel::GeometryData* genGeometryLineData(trimesh::TriMesh* mesh, qtuser_core::Progressor* progressor)
{
	if (!mesh || (mesh->faces.size() == 0) || (mesh->vertices.size() == 0)) return nullptr;

	creative_kernel::GeometryData* geometryData = new creative_kernel::GeometryData();

	QTime qtime;
	qtime.start();
	if (progressor != nullptr)
	{
		progressor->progress(0.0f);
	}

	int vcount = (int)mesh->vertices.size();
	int fcount = (int)mesh->faces.size();
	int count = fcount * 6;

	geometryData->m_vcount = vcount;
	geometryData->m_fcount = fcount;
	geometryData->m_count = count;

	QByteArray &positionByteArray = geometryData->m_positionByteArray;
	positionByteArray.resize(vcount * 3 * sizeof(float));
	QByteArray &indexByteArray = geometryData->m_indexByteArray;
	indexByteArray.resize(count * sizeof(uint));

	memcpy(positionByteArray.data(), &mesh->vertices[0], positionByteArray.size());

	uint* data = (uint*)indexByteArray.data();
//#pragma omp parallel for shared(mesh, data)
	for (int i = 0; i < fcount; ++i)
	{
		trimesh::TriMesh::Face& f = mesh->faces.at(i);
		data[6 * i + 0] = f[0];
		data[6 * i + 1] = f[1];
		data[6 * i + 2] = f[0];
		data[6 * i + 3] = f[2];
		data[6 * i + 4] = f[1];
		data[6 * i + 5] = f[2];

		if (progressor != nullptr)
		{
			if ((i + 1) % 10000 == 0)
			{
				progressor->progress((float)i / (float)fcount);
				if (progressor->interrupt())
				{
					if (geometryData != nullptr)
					{
						delete geometryData;
						return nullptr;
					}
				}
			}
		}
	}
	if (progressor != nullptr)
	{
		progressor->progress(1.0f);
	}
	qDebug() << "genGeometryLineData run time = " << qtime.elapsed();
	return geometryData;
}

creative_kernel::GeometryData* genGeometryEdgeData(trimesh::TriMesh* mesh, qtuser_core::Progressor* progressor, float angleThreshold)
{
	return nullptr;

	if (!mesh || (mesh->faces.size() == 0) || (mesh->vertices.size() == 0)) return nullptr;

	creative_kernel::GeometryData* geometryData = new creative_kernel::GeometryData();

	QTime qtime;
	qtime.start();

	if (progressor != nullptr)
	{
		progressor->progress(0.0f);
	}

	int fcount = (uint)mesh->faces.size();
	int vcount = (uint)mesh->vertices.size();

	std::vector<trimesh::vec3> allFaceNormal;
	allFaceNormal.resize(fcount);

	std::vector<std::unordered_map<int, int>> allVertexFace;
	allVertexFace.resize(vcount);

	//std::vector<std::vector<int>> allFaceLine;
	//allFaceLine.resize(fcount);

	std::vector<trimesh::point> edge;

	qDebug() << "fcount = " << fcount;

	{
//#pragma omp parallel for shared(allFaceLine, mesh, allVertexFace, allFaceNormal)
		for (int i = 0; i < fcount; i++)
		{
			trimesh::TriMesh::Face& f = mesh->faces.at(i);
			trimesh::vec3 v01 = mesh->vertices.at(f[1]) - mesh->vertices.at(f[0]);
			trimesh::vec3 v02 = mesh->vertices.at(f[2]) - mesh->vertices.at(f[0]);
			trimesh::vec3 n = v01 TRICROSS v02;
			trimesh::normalize(n);
			allFaceNormal[i] = n;

			if (f[0] == f[1] || f[0] == f[2] || f[1] == f[2])
			{
				continue;
			}

			for (int k = 0; k < 2; k++)
			{
				std::unordered_map<int, int>& vertexFace = allVertexFace[f[k]];

				for (int k2 = k + 1; k2 < 3; k2++)
				{
					std::unordered_map<int, int>& vertexFace2 = allVertexFace[f[k2]];

					std::unordered_map<int, int>::iterator itr2 = vertexFace2.begin();
					while (itr2 != vertexFace2.end())
					{
						std::unordered_map<int, int>::iterator itr = vertexFace.find(itr2->first);
						if (itr != vertexFace.end())
						{
							int cfaceId = itr2->first;
							trimesh::TriMesh::Face& f2 = mesh->faces.at(cfaceId);
							std::vector<int> faceLine;

							int ret = twoFaceCollinear(f, f2, faceLine);
							if (ret == 2)
							{
								trimesh::vec3 nf2 = allFaceNormal[cfaceId];
								QVector3D qfn1(n[0], n[1], n[2]);
								QVector3D qfn2(nf2[0], nf2[1], nf2[2]);

								float angle = qtuser_3d::angleOfVector3D2(qfn1, qfn2);
								if (abs(angle) >= angleThreshold)
								{
									//allFaceLine[i].push_back(faceLine[0]);
									//allFaceLine[i].push_back(faceLine[1]);
//#pragma omp critical
									{
										edge.push_back(mesh->vertices.at(faceLine[0]));
										edge.push_back(mesh->vertices.at(faceLine[1]));
									}
								}
								//vertexFace2.erase(itr2);
								//vertexFace.erase(itr);

								//std::unordered_map<int, int>::iterator itr2_d = vertexFace2.find(i);
								//if (itr2_d != vertexFace2.end())
								//{
								//	vertexFace2.erase(itr2_d);
								//}
								//std::unordered_map<int, int>::iterator itr_d = vertexFace.find(i);
								//if (itr_d != vertexFace.end())
								//{
								//	vertexFace.erase(itr_d);
								//}

								break;
							}
						}
						itr2++;
					}
				}
			}
			allVertexFace[f[0]][i] = 1;
			allVertexFace[f[1]][i] = 1;
			allVertexFace[f[2]][i] = 1;

			if (progressor != nullptr)
			{
				if ((i + 1) % 10000 == 0)
				{
					progressor->progress((float)i / (float)fcount);
					
					if (progressor->interrupt())
					{
						if (geometryData != nullptr)
						{
							delete geometryData;
							return nullptr;
						}
					}
				}
			}
		}
	}

	//for (int i = 0; i < allFaceLine.size(); i++)
	//{
	//	if (allFaceLine[i].size() > 0)
	//	{
	//		std::vector<int> &faceLine = allFaceLine[i];
	//		for (int j = 0; j < faceLine.size(); j += 2)
	//		{
	//			edge.push_back(mesh->vertices.at(faceLine[j]));
	//			edge.push_back(mesh->vertices.at(faceLine[j+1]));
	//		}
	//	}
	//}

	vcount = (uint)edge.size();
	if (vcount == 0)
	{
		qDebug() << "trimesh2GeometryEdge run time = " << qtime.elapsed();
		return nullptr;
	}
	geometryData->m_vcount = vcount;

	qDebug() << "edge.size() = " << fcount;

	QByteArray &positionByteArray = geometryData->m_positionByteArray;
	positionByteArray.resize(vcount * 3 * sizeof(float));
	memcpy(positionByteArray.data(), &edge[0], positionByteArray.size());

	if (progressor != nullptr)
	{
		progressor->progress(1.0f);
		progressor->progress(0.0f);
	}
	qDebug() << "trimesh2GeometryEdge run time = " << qtime.elapsed();
	return geometryData;
}

creative_kernel::GeometryData* genPLGeometryData(trimesh::TriMesh* mesh, qtuser_core::Progressor* progressor)
{
	if (!mesh || (mesh->vertices.size() == 0)) return nullptr;

	creative_kernel::GeometryData* geometryData = new creative_kernel::GeometryData();

	QTime qtime;
	qtime.start();

	if (progressor != nullptr)
	{
		progressor->progress(0.0f);
	}

	int pointCount = (int)mesh->vertices.size();
	geometryData->m_vcount = pointCount;

	QByteArray& positionByteArray = geometryData->m_positionByteArray;
	positionByteArray.resize(pointCount * 3 * sizeof(float));

	memcpy(positionByteArray.data(), &mesh->vertices[0], positionByteArray.size());

	if (progressor != nullptr)
	{
		progressor->progress(1.0f);
	}
	qDebug() << "genGeometryFaceData run time = " << qtime.elapsed();
	return geometryData;
}





Qt3DRender::QGeometry* genGeometryFaceData(creative_kernel::GeometryData* geometryData, Qt3DCore::QNode* parent)
{
	if (geometryData == nullptr || geometryData->m_positionByteArray.size() == 0 || geometryData->m_normalByteArray.size() == 0)
	{
		return nullptr;
	}
	Qt3DRender::QBuffer* positionBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
	Qt3DRender::QBuffer* normalBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
	positionBuffer->setData(geometryData->m_positionByteArray);
	normalBuffer->setData(geometryData->m_normalByteArray);
	
	Qt3DRender::QAttribute* positionAttribute = new Qt3DRender::QAttribute(positionBuffer, Qt3DRender::QAttribute::defaultPositionAttributeName(), Qt3DRender::QAttribute::Float, 3, geometryData->m_count);
	Qt3DRender::QAttribute* normalAttribute = new Qt3DRender::QAttribute(normalBuffer, Qt3DRender::QAttribute::defaultNormalAttributeName(), Qt3DRender::QAttribute::Float, 3, geometryData->m_count);

	Qt3DRender::QAttribute* texcoordAttribute = nullptr;
	if (geometryData->m_texcoordArray.size() > 0)
	{
		Qt3DRender::QBuffer* texcoordBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
		trimesh::vec2* texdted = (trimesh::vec2*)(geometryData->m_texcoordArray.data());
		texcoordBuffer->setData(geometryData->m_texcoordArray);
		texcoordAttribute = new Qt3DRender::QAttribute(texcoordBuffer, Qt3DRender::QAttribute::defaultTextureCoordinateAttributeName(), Qt3DRender::QAttribute::Float, 2, geometryData->m_count);
	}

	return qtuser_3d::GeometryCreateHelper::create(parent, positionAttribute, normalAttribute, texcoordAttribute);
}

Qt3DRender::QGeometry* genGeometryLineData(creative_kernel::GeometryData* geometryData, Qt3DCore::QNode* parent)
{
	if (geometryData == nullptr || geometryData->m_positionByteArray.size() == 0 || geometryData->m_indexByteArray.size() == 0)
	{
		return nullptr;
	}
	Qt3DRender::QBuffer* positionBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
	Qt3DRender::QBuffer* indexBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer);

	positionBuffer->setData(geometryData->m_positionByteArray);
	Qt3DRender::QAttribute* positionAttribute = new Qt3DRender::QAttribute(positionBuffer, Qt3DRender::QAttribute::defaultPositionAttributeName(), Qt3DRender::QAttribute::Float, 3, geometryData->m_vcount);

	indexBuffer->setData(geometryData->m_indexByteArray);
	Qt3DRender::QAttribute* indexAttribute = new Qt3DRender::QAttribute(indexBuffer, "index", Qt3DRender::QAttribute::UnsignedInt, 1, geometryData->m_count);
	indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);

	return qtuser_3d::GeometryCreateHelper::create(parent, positionAttribute, indexAttribute);
}

Qt3DRender::QGeometry* genGeometryEdgeData(creative_kernel::GeometryData* geometryData, Qt3DCore::QNode* parent)
{
	if (geometryData == nullptr || geometryData->m_positionByteArray.size() == 0)
	{
		return nullptr;
	}
	Qt3DRender::QBuffer* positionBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
	positionBuffer->setData(geometryData->m_positionByteArray);

	Qt3DRender::QAttribute* positionAttribute = new Qt3DRender::QAttribute(positionBuffer, Qt3DRender::QAttribute::defaultPositionAttributeName(), Qt3DRender::QAttribute::Float, 3, geometryData->m_vcount);

	return qtuser_3d::GeometryCreateHelper::create(parent, positionAttribute, nullptr);
}

Qt3DRender::QGeometry* genPLGeometryData(creative_kernel::GeometryData* geometryData, Qt3DCore::QNode* parent)
{
	if (geometryData == nullptr || geometryData->m_positionByteArray.size() == 0)
	{
		return nullptr;
	}
	Qt3DRender::QBuffer* positionBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
	positionBuffer->setData(geometryData->m_positionByteArray);

	Qt3DRender::QAttribute* positionAttribute = new Qt3DRender::QAttribute(positionBuffer, Qt3DRender::QAttribute::defaultPositionAttributeName(), Qt3DRender::QAttribute::Float, 3, geometryData->m_vcount);

	return qtuser_3d::GeometryCreateHelper::create(parent, positionAttribute, nullptr);
}

Qt3DRender::QGeometry* trimesh2GeometryFace(trimesh::TriMesh* mesh, Qt3DCore::QNode* parent, qtuser_core::Progressor* progressor)
{
	if (!mesh || (mesh->faces.size() == 0) || (mesh->vertices.size() == 0)) return nullptr;

	creative_kernel::GeometryData* geometryData = genGeometryFaceData(mesh, progressor);
	Qt3DRender::QGeometry* geometry = genGeometryFaceData(geometryData, parent);
	if (geometryData)
	{
		delete geometryData;
		geometryData = nullptr;
	}
	return geometry;
}

Qt3DRender::QGeometry* trimesh2GeometryLine(trimesh::TriMesh* mesh, Qt3DCore::QNode* parent, qtuser_core::Progressor* progressor)
{
	if (!mesh || (mesh->faces.size() == 0) || (mesh->vertices.size() == 0)) return nullptr;

	creative_kernel::GeometryData* geometryData = genGeometryLineData(mesh, progressor);
	Qt3DRender::QGeometry* geometry = genGeometryLineData(geometryData, parent);
	if (geometryData)
	{
		delete geometryData;
		geometryData = nullptr;
	}
	return geometry;
}


Qt3DRender::QGeometry* trimesh2GeometryEdge(trimesh::TriMesh* mesh, Qt3DCore::QNode* parent, qtuser_core::Progressor* progressor, float angleThreshold)
{
	if (!mesh || (mesh->faces.size() == 0) || (mesh->vertices.size() == 0)) return nullptr;

	creative_kernel::GeometryData* geometryData = genGeometryEdgeData(mesh, progressor, angleThreshold);
	Qt3DRender::QGeometry* geometry = genGeometryEdgeData(geometryData, parent);
	if (geometryData)
	{
		delete geometryData;
		geometryData = nullptr;
	}
	return geometry;
}


Qt3DRender::QGeometry* trimeshPL2Geometry(trimesh::TriMesh* mesh, Qt3DCore::QNode* parent, qtuser_core::Progressor* progressor)
{
	if (!mesh || (mesh->vertices.size() == 0)) return nullptr;

	creative_kernel::GeometryData* geometryData = genPLGeometryData(mesh, progressor);
	Qt3DRender::QGeometry* geometry = genPLGeometryData(geometryData, parent);
	if (geometryData)
	{
		delete geometryData;
		geometryData = nullptr;
	}
	return geometry;
}





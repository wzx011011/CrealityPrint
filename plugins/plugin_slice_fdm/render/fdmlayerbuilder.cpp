#include "fdmlayerbuilder.h"

#include <QtGui/QVector3D>
#include <QtCore/QDebug>

#include "core/slicemodel.h"
#include "core/slicemodelbuilder.h"

#include "qtuser3d/geometry/bufferhelper.h"
#include "qtuser3d/geometry/geometrycreatehelper.h"
#include "qtusercore/string/stringtool.h"

FDMLayerBuilder::FDMLayerBuilder()
	:lineCount(0)
	, extendR(4)
	, layers(0)
{

}

FDMLayerBuilder::~FDMLayerBuilder()
{
	clear();
}

void FDMLayerBuilder::buildGeometry(Qt3DRender::QGeometry** geometry, std::unordered_map<int, trimesh::vec4>** indicator)
{
	if (!geometry || !indicator || lineCount <= 0)
		return;

	*indicator = new std::unordered_map<int, trimesh::vec4>();
	int count = lineCount;
	int r = extendR;
	unsigned vertexNum = count * 2 * r;
	std::vector<trimesh::vec3> positions;
	positions.reserve(vertexNum);
	std::vector<trimesh::vec3> normals;
	normals.reserve(vertexNum);
	std::vector<trimesh::vec2> flags;
	flags.reserve(vertexNum);
	std::vector<trimesh::vec4> drawFlags;
	drawFlags.reserve(vertexNum);
	// 传递给着色器的标志位，这些标志位会在顶点间进行插值
	std::vector<trimesh::vec4> smoothFlags;
	smoothFlags.reserve(vertexNum);

	unsigned triangleNum = count * 3 * r;
	std::vector<trimesh::uivec3> indices;
	indices.reserve(triangleNum);

	unsigned vertexIndex = 0;

	for (LayerPolyLine& lpl : polyLines)
	{
		for (int l = 0; l < lpl.m_lineAttributes.size(); l++)
		{
			std::vector< SliceLineAttributes>& slaSet = lpl.m_lineAttributes[l];

			for (int k = 0; k < slaSet.size(); k++)
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
					positions.push_back(*startPosition);
					positions.push_back(*endPosition);
					normals.push_back(*normal);
					normals.push_back(*normal++);

					flags.push_back(trimesh::vec2(lpl.m_layer, sla.step));
					flags.push_back(trimesh::vec2(lpl.m_layer, sla.step));

					int pos = (int)lpl.m_layer * 100000 + (int)sla.step;

					trimesh::vec3& lastpt = positions.back();
					(**indicator)[pos] = trimesh::vec4(lastpt[0], lastpt[1], lastpt[2], (float)sla.speedFlag);

					drawFlags.push_back(trimesh::vec4((float)sla.speedFlag, (float)sla.linetype, (float)sla.nozzle, 0.0f));
					drawFlags.push_back(trimesh::vec4((float)sla.speedFlag, (float)sla.linetype, (float)sla.nozzle, 0.0f));

					smoothFlags.push_back(trimesh::vec4((float)sla.layerStartFlag * 0.0f, 0.0f, 0.0f, 0.0f));
					smoothFlags.push_back(trimesh::vec4((float)sla.layerStartFlag * (trimesh::dist(*startPosition, *endPosition)), 0.0f, 0.0f, 0.0f));

					startPosition++;
					endPosition++;
				}

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

	Qt3DRender::QAttribute* positionAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&positions[0][0], qtuser_3d::AttribueSlot::Position, vertexNum);
	Qt3DRender::QAttribute* normalAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&normals[0][0], qtuser_3d::AttribueSlot::Normal, vertexNum);
	Qt3DRender::QAttribute* flagAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute("vertexFlag", (const char*)&flags[0][0], 2, vertexNum);
	Qt3DRender::QAttribute* drawFlagAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute("vertexDrawFlag", (const char*)&drawFlags[0][0], 4, vertexNum);
	Qt3DRender::QAttribute* smoothFlagAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute("vertexSmoothFlag", (const char*)&smoothFlags[0][0], 4, vertexNum);
	Qt3DRender::QAttribute* indexAttribute = qtuser_3d::BufferHelper::CreateIndexAttribute((const char*)&indices[0][0], 3 * indices.size());

	*geometry = qtuser_3d::GeometryCreateHelper::createEx(nullptr, positionAttribute, normalAttribute, flagAttribute, indexAttribute, drawFlagAttribute, smoothFlagAttribute);

}

void FDMLayerBuilder::buildBillboardGeometry(Qt3DRender::QGeometry** geometry, std::unordered_map<int, trimesh::vec4>** indicator)
{
	if (!geometry || !indicator || lineCount <= 0)
		return;

	*indicator = new std::unordered_map<int, trimesh::vec4>();
	int count = lineCount;
	//int r = extendR;
	unsigned vertexNum = count * 2;
	//unsigned vertexNum = 4;
	std::vector<trimesh::vec3> positions;
	positions.reserve(vertexNum);
	std::vector<trimesh::vec3> normals;
	normals.reserve(vertexNum);
	std::vector<trimesh::vec4> flags;
	flags.reserve(vertexNum);
	std::vector<trimesh::vec4> drawFlags;
	drawFlags.reserve(vertexNum);
	// 传递给着色器的标志位，这些标志位会在顶点间进行插值
	std::vector<trimesh::vec4> smoothFlags;
	smoothFlags.reserve(vertexNum);

	unsigned vertexIndex = 0;

	for (LayerPolyLine& lpl : polyLines)
	{
		for (int l = 0; l < lpl.m_lineAttributes.size(); l++)
		{
			std::vector< SliceLineAttributes>& slaSet = lpl.m_lineAttributes[l];

			for (int k = 0; k < slaSet.size(); k++)
			{
				SliceLineAttributes& sla = slaSet[k];

				LinePoint* start = nullptr, * end = nullptr;
				lpl.getLineStartEnd(sla, start, end, l);

				trimesh::vec3 startPosition = start->m_pt;
				trimesh::vec3 endPosition = end->m_pt;
				float distSE = trimesh::dist(startPosition, endPosition);
				positions.push_back(startPosition);
				positions.push_back(endPosition);

				trimesh::vec3 normal;
				//lpl.getLineNormal(k, normal, l);
				normals.push_back(normal);

				flags.push_back(trimesh::vec4(lpl.m_layer, sla.step, lpl.m_lineWidth, lpl.m_thickness));
				flags.push_back(trimesh::vec4(lpl.m_layer, sla.step, lpl.m_lineWidth, lpl.m_thickness));

				int pos = (int)lpl.m_layer * 100000 + (int)sla.step;

				trimesh::vec3& lastpt = positions.back();
				(**indicator)[pos] = trimesh::vec4(lastpt[0], lastpt[1], lastpt[2], (float)sla.speedFlag);

				drawFlags.push_back(trimesh::vec4((float)sla.speedFlag, (float)sla.linetype, (float)sla.nozzle, 0.0f));
				drawFlags.push_back(trimesh::vec4((float)sla.speedFlag, (float)sla.linetype, (float)sla.nozzle, 0.0f));

				smoothFlags.push_back(trimesh::vec4((float)sla.layerStartFlag * 0.0f, distSE, 0.0f, 0.0f));
				smoothFlags.push_back(trimesh::vec4((float)sla.layerStartFlag * distSE, distSE, 0.0f, 0.0f));
			}
		}
		lpl.clearAll();
	}

	Qt3DRender::QAttribute* positionAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&positions[0][0], qtuser_3d::AttribueSlot::Position, vertexNum);
	Qt3DRender::QAttribute* normalAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&normals[0][0], qtuser_3d::AttribueSlot::Normal, vertexNum);
	Qt3DRender::QAttribute* flagAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute("vertexFlag", (const char*)&flags[0][0], 4, vertexNum);
	Qt3DRender::QAttribute* drawFlagAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute("vertexDrawFlag", (const char*)&drawFlags[0][0], 4, vertexNum);
 	Qt3DRender::QAttribute* smoothFlagAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute("vertexSmoothFlag", (const char*)&smoothFlags[0][0], 4, vertexNum);
	//Qt3DRender::QAttribute* indexAttribute = qtuser_3d::BufferHelper::CreateIndexAttribute((const char*)&indices[0][0], 3 * indices.size());

	*geometry = qtuser_3d::GeometryCreateHelper::createEx(nullptr, positionAttribute, normalAttribute, flagAttribute, drawFlagAttribute, smoothFlagAttribute);
}

void FDMLayerBuilder::buildBillboardNoTessGeometry(Qt3DRender::QGeometry** geometry, std::unordered_map<int, trimesh::vec4>& indicator)
{
	if (!geometry || lineCount <= 0)
		return;

	int count = lineCount;
	//int r = extendR;
	unsigned vertexNum = count * 6;
	//unsigned vertexNum = 3;
	std::vector<trimesh::vec4> positionsEnd;
	positionsEnd.reserve(vertexNum);
	std::vector<trimesh::vec3> positionsBegin;
	positionsBegin.reserve(vertexNum);
	std::vector<trimesh::vec4> flags;
	flags.reserve(vertexNum);
	std::vector<trimesh::vec4> drawFlags;
	drawFlags.reserve(vertexNum);
	// 传递给着色器的标志位，这些标志位会在顶点间进行插值
	std::vector<trimesh::vec4> smoothFlags;
	smoothFlags.reserve(vertexNum);

	unsigned vertexIndex = 0;

	for (LayerPolyLine& lpl : polyLines)
	{
		for (int l = 0; l < lpl.m_lineAttributes.size(); l++)
		{
			std::vector< SliceLineAttributes>& slaSet = lpl.m_lineAttributes[l];

			for (int k = 0; k < slaSet.size(); k++)
			{
				SliceLineAttributes& sla = slaSet[k];

				LinePoint* start = nullptr, * end = nullptr;
				lpl.getLineStartEnd(sla, start, end, l);

				trimesh::vec3 startPosition = start->m_pt;
				trimesh::vec3 endPosition = end->m_pt;
				float distSE = trimesh::dist(startPosition, endPosition);

				positionsEnd.push_back(trimesh::vec4(startPosition.x, startPosition.y, startPosition.z, 1.0f));
				positionsEnd.push_back(trimesh::vec4(endPosition.x, endPosition.y, endPosition.z, -1.0f));
				positionsEnd.push_back(trimesh::vec4(endPosition.x, endPosition.y, endPosition.z, 1.0f));
				positionsEnd.push_back(trimesh::vec4(endPosition.x, endPosition.y, endPosition.z, 1.0f));
				positionsEnd.push_back(trimesh::vec4(startPosition.x, startPosition.y, startPosition.z, -1.0f));
				positionsEnd.push_back(trimesh::vec4(startPosition.x, startPosition.y, startPosition.z, 1.0f));

				positionsBegin.push_back(trimesh::vec3(endPosition.x, endPosition.y, endPosition.z));
				positionsBegin.push_back(trimesh::vec3(startPosition.x, startPosition.y, startPosition.z));
				positionsBegin.push_back(trimesh::vec3(startPosition.x, startPosition.y, startPosition.z));
				positionsBegin.push_back(trimesh::vec3(startPosition.x, startPosition.y, startPosition.z));
				positionsBegin.push_back(trimesh::vec3(endPosition.x, endPosition.y, endPosition.z));
				positionsBegin.push_back(trimesh::vec3(endPosition.x, endPosition.y, endPosition.z));

				flags.push_back(trimesh::vec4(lpl.m_layer, sla.step, lpl.m_lineWidth, lpl.m_thickness));
				flags.push_back(trimesh::vec4(lpl.m_layer, sla.step, lpl.m_lineWidth, lpl.m_thickness));
				flags.push_back(trimesh::vec4(lpl.m_layer, sla.step, lpl.m_lineWidth, lpl.m_thickness));
				flags.push_back(trimesh::vec4(lpl.m_layer, sla.step, lpl.m_lineWidth, lpl.m_thickness));
				flags.push_back(trimesh::vec4(lpl.m_layer, sla.step, lpl.m_lineWidth, lpl.m_thickness));
				flags.push_back(trimesh::vec4(lpl.m_layer, sla.step, lpl.m_lineWidth, lpl.m_thickness));

				int pos = (int)lpl.m_layer * 100000 + (int)sla.step;

				trimesh::vec3& lastpt = endPosition;
				indicator[pos] = trimesh::vec4(lastpt[0], lastpt[1], lastpt[2], (float)sla.speedFlag);

				drawFlags.push_back(trimesh::vec4((float)sla.speedFlag, (float)sla.linetype, (float)sla.nozzle, 0.0f));
				drawFlags.push_back(trimesh::vec4((float)sla.speedFlag, (float)sla.linetype, (float)sla.nozzle, 0.0f));
				drawFlags.push_back(trimesh::vec4((float)sla.speedFlag, (float)sla.linetype, (float)sla.nozzle, 0.0f));
				drawFlags.push_back(trimesh::vec4((float)sla.speedFlag, (float)sla.linetype, (float)sla.nozzle, 0.0f));
				drawFlags.push_back(trimesh::vec4((float)sla.speedFlag, (float)sla.linetype, (float)sla.nozzle, 0.0f));
				drawFlags.push_back(trimesh::vec4((float)sla.speedFlag, (float)sla.linetype, (float)sla.nozzle, 0.0f));

				smoothFlags.push_back(trimesh::vec4((float)sla.layerStartFlag * 0.0f, 0.0f, 0.0f, 0.0f));
				smoothFlags.push_back(trimesh::vec4((float)sla.layerStartFlag * distSE, 0.0f, 0.0f, 0.0f));
				smoothFlags.push_back(trimesh::vec4((float)sla.layerStartFlag * distSE, 0.0f, 0.0f, 0.0f));
				smoothFlags.push_back(trimesh::vec4((float)sla.layerStartFlag * distSE, 0.0f, 0.0f, 0.0f));
				smoothFlags.push_back(trimesh::vec4((float)sla.layerStartFlag * 0.0f, 0.0f, 0.0f, 0.0f));
				smoothFlags.push_back(trimesh::vec4((float)sla.layerStartFlag * 0.0f, 0.0f, 0.0f, 0.0f));
			}
		}
		lpl.clearAll();
	}	

	Qt3DRender::QAttribute* positionEAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute("vertexEndPosition", (const char*)&positionsEnd[0][0], 4, vertexNum);
	Qt3DRender::QAttribute* positionBAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute("vertexBeginPosition", (const char*)&positionsBegin[0][0], 3, vertexNum);
	Qt3DRender::QAttribute* flagAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute("vertexFlag", (const char*)&flags[0][0], 4, vertexNum);
	Qt3DRender::QAttribute* drawFlagAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute("vertexDrawFlag", (const char*)&drawFlags[0][0], 4, vertexNum);
	Qt3DRender::QAttribute* smoothFlagAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute("vertexSmoothFlag", (const char*)&smoothFlags[0][0], 4, vertexNum);

	*geometry = qtuser_3d::GeometryCreateHelper::createEx(nullptr, positionEAttribute, positionBAttribute, flagAttribute, drawFlagAttribute, smoothFlagAttribute);
}

void FDMLayerBuilder::clear()
{
	for (LayerPolyLine& poly : polyLines)
		poly.clearAll();
}

int FDMLayerBuilder::totalLayers()
{
	return layers;
}

int FDMLayerBuilder::totalNozzleNr()
{
	return nNozzle;
}

qtuser_3d::Box3D FDMLayerBuilder::totalBox()
{
	return box;
}

FDMLayerBuilder* FDMLayerBuilder::buildFromGCode(const std::string& prevCode, const std::vector<std::string>& layerCode, float lineWidth,
		std::vector<int>* steps)
{
	qInfo() << "build FDM Layer start.";

	std::string tag = ";Layer Height:";
	float default_height = 0;
	int pos = prevCode.find(tag);
	if (pos >= 0)
	{
		std::string t = prevCode.substr(pos + tag.length());
		pos = t.find("\n");
		if (pos > 0)
		{
			t = t.substr(0, pos);
			default_height = qtuser_core::StrToFloat(t);
			//default_height /= 1000.0;
		}
	}

	bool spiralFlag = false;
	std::string spiralTag = ";Vase Model:true";
	pos = prevCode.find(spiralTag);
	if (pos >= 0)
	{
		spiralFlag = true;
	}
	
	//;machine is belt:
	int isbelt = 0;
	int ipos1 = prevCode.find(";machine is belt:true");
	int ipos2 = prevCode.find("Crealitybelt");
	if (ipos1>=0)
	{  
		isbelt = 1;//creality print belt
	}
	else if (ipos2 >= 0)
	{
		isbelt = 2;//creality slicer belt
	}

	std::string tag2 = ";machine belt offset:";
	float offset = 10.0;
	int pos2 = prevCode.find(tag2);
	if (pos2 >= 0)
	{
		std::string t = prevCode.substr(pos2 + tag2.length());
		pos2 = t.find("\n");
		if (pos2 > 0)
		{
			t = t.substr(0, pos2);
			offset = qtuser_core::StrToFloat(t);
		}
	}

	tag2 = ";machine belt offset Y:";
	float offsetY = 0.0;
	pos2 = prevCode.find(tag2);
	if (pos2 >= 0)
	{
		std::string t = prevCode.substr(pos2 + tag2.length());
		pos2 = t.find("\n");
		if (pos2 > 0)
		{
			t = t.substr(0, pos2);
			offsetY = qtuser_core::StrToFloat(t);
		}
	}

	SliceModel sliceModel;
	SliceModelBuilder::buildSliceModel(&sliceModel, layerCode, default_height,offset, offsetY, spiralFlag, isbelt);
	int layers = (int)sliceModel.m_sliceLayers.size();
	if (layers <= 0)
		return nullptr;

	FDMLayerBuilder* builder = new FDMLayerBuilder();
	builder->nNozzle = 1;
	builder->layers = layers;

	if(steps)
		steps->resize(layers);

	int count = 0;
	float layer = 0.0f;
	unsigned r = 4;

	builder->polyLines.resize(layers);
	for (int i = 0; i < layers; ++i)
	{
		const SliceLayer& sliceLayer = sliceModel.m_sliceLayers.at(i);
		float step = 0.0f;

		LayerPolyLine& lpl = builder->polyLines.at(i);
		lpl.m_layer = layer;
		lpl.m_thickness = sliceLayer.thickness;
		lpl.m_lineWidth = lineWidth;

		for (const SliceLine& sliceLine : sliceLayer.m_sliceLines)
		{
			if (sliceLine.type != SliceLineType::NoneType)
			{
				if (sliceLine.type != SliceLineType::MoveOnly && sliceLine.type != SliceLineType::Travel)
				{
					lpl.addLine(sliceLine, step);

					++count;
					builder->box += QVector3D(sliceLine.start.x, sliceLine.start.y, sliceLine.start.z);
					builder->box += QVector3D(sliceLine.end.x, sliceLine.end.y, sliceLine.end.z);

					

					if (builder->nNozzle < sliceLine.nozzle + 1)
					{
						builder->nNozzle = sliceLine.nozzle + 1;
					}
				}
				else
				{
					builder->moveLines.push_back(LineMove(sliceLine.start, sliceLine.end, step));
				}
				step += 1.0f;
				
			}
		}
		if(steps)
			steps->at((int)layer) = (int)step;
		layer += 1.0f;

		// 使用四棱柱 GCode 预览时需要打开
		//lpl.extentedNPoly(r);
	}

	builder->lineCount = count;

	qInfo() << "build FDM Layer end.";

	return builder;
}
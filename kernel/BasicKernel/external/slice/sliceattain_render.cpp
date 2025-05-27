#include "sliceattain.h"
#include "qtusercore/string/resourcesfinder.h"
#include "qtusercore/module/systemutil.h"
#include "qtuser3d/trimesh2/conv.h"
#include "qtuser3d/geometry/geometrycreatehelper.h"
#include "qtuser3d/geometry/bufferhelper.h"
#include "qtuser3d/refactor/xentity.h"
#include "cxkernel/utils/utils.h"

#include <Qt3DRender/QAttribute>
#include "interface/visualsceneinterface.h"
#include <Qt3DCore/QEntity>
#include "gcodepreviewnode.h"

#include "interface/machineinterface.h"
#include "internal/render/gcodeprevieweffect.h"

using namespace qtuser_3d;
namespace creative_kernel
{
	
	int SliceAttain::addData(char* buffer, char* data, int index, int size, int repeat) 
	{
		for (int i = 0; i < repeat; ++i)
			buffer[index++] = *data;

		return index;
	};

	Qt3DRender::QBuffer* SliceAttain::createBuffer(GCodePreviewType type, int stepCount, const std::vector<crslice2::StepInfoPtr>& steps)
	{
		auto createFlagBufferWithRange = [=](char* cbuffer, int structOffset, int stepCount, crslice2::GCodeRange& range, const std::vector<crslice2::StepInfoPtr>& stepInfos)
		{
			int index = 0;
			int singleSize = sizeof(char);
			for (const crslice2::StepInfoPtr& step : stepInfos)
			{
				const crslice2::StepInfo& stepInfo = m_gcodePath.getStepInfo(step.moveId);
				float data = *(float*)((char*)&stepInfo + structOffset);
				char cdata = range.percent(data);
				index = addData(cbuffer, &cdata, index, singleSize, step.span);
			}

			Qt3DRender::QBuffer* qBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
			qBuffer->setData(QByteArray((char*)cbuffer, singleSize * stepCount));
			return qBuffer;
		};

		auto createFlagBuffer = [=](char* cbuffer, int structOffset, int stepCount, const std::vector<crslice2::StepInfoPtr>& stepInfos)
		{
			int index = 0;
			int singleSize = sizeof(char);
			for (const crslice2::StepInfoPtr& step : stepInfos)
			{
				const crslice2::StepInfo& stepInfo = m_gcodePath.getStepInfo(step.moveId);
				char data = *((char*)&stepInfo + structOffset);
				index = addData(cbuffer, &data, index, singleSize, step.span);
			}

			Qt3DRender::QBuffer* qBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
			qBuffer->setData(QByteArray((char*)cbuffer, singleSize * stepCount));
			return qBuffer;
		};

		char* cbuffer = new char[stepCount];
		crslice2::StepInfo tempStepInfo;
		Qt3DRender::QBuffer* buffer = NULL;
		switch (type)
		{
		case GCodePreviewType::Speed:
		{
			buffer = createFlagBufferWithRange(cbuffer, (char*)&tempStepInfo.speed - (char*)&tempStepInfo, stepCount, m_gcodePath.speedRange, steps);
		} break;
		case GCodePreviewType::Role:
		{
			buffer = createFlagBuffer(cbuffer, (char*)&tempStepInfo.role - (char*)&tempStepInfo, stepCount, steps);
		} break;
		case GCodePreviewType::Extruder:
		{
			buffer = createFlagBuffer(cbuffer, (char*)&tempStepInfo.extruder - (char*)&tempStepInfo, stepCount, steps);
		} break;
		case GCodePreviewType::Height:
		{
			buffer = createFlagBufferWithRange(cbuffer, (char*)&tempStepInfo.height - (char*)&tempStepInfo, stepCount, m_gcodePath.heightRange, steps);
		} break;
		case GCodePreviewType::Width:
		{
			buffer = createFlagBufferWithRange(cbuffer, (char*)&tempStepInfo.width - (char*)&tempStepInfo, stepCount, m_gcodePath.widthRange, steps);
		} break;
		case GCodePreviewType::VolumeRate:
		{
			buffer = createFlagBufferWithRange(cbuffer, (char*)&tempStepInfo.volumetricRate - (char*)&tempStepInfo, stepCount, m_gcodePath.volumetricRateRange, steps);
		} break;
		case GCodePreviewType::LayerTime:
		{
			buffer = createFlagBufferWithRange(cbuffer, (char*)&tempStepInfo.layerDuration - (char*)&tempStepInfo, stepCount, m_gcodePath.layerDurationRange, steps);
		} break;
		case GCodePreviewType::FanSpeed:
		{
			buffer = createFlagBufferWithRange(cbuffer, (char*)&tempStepInfo.fanSpeed - (char*)&tempStepInfo, stepCount, m_gcodePath.fanSpeedRange, steps);
		} break;
		case GCodePreviewType::Temperature:
		{
			buffer = createFlagBufferWithRange(cbuffer, (char*)&tempStepInfo.temperature - (char*)&tempStepInfo, stepCount, m_gcodePath.temperatureRange, steps);
		} break;
		case GCodePreviewType::Acc:
		{
			buffer = createFlagBufferWithRange(cbuffer, (char*)&tempStepInfo.acc - (char*)&tempStepInfo, stepCount, m_gcodePath.accRange, steps);
		} break;
		}
		delete cbuffer;
		
		return buffer;
	}

	void SliceAttain::buildGeometries(GCodePreviewType type, GCodePreviewEffect* polyEffect, GCodePreviewEffect* monoEffect, const QMatrix4x4& pose)
	{
		if (m_isBuildGeometries == true)
		{
			QVariantList qvColor = getNozzleColorList();
			polyEffect->setNozzleColorList(qvColor);
			m_previewNode->setPose(pose);
			return;
		}

		if (m_gcodePath.blocks.empty())
			return;

		m_isBuildGeometries = true;
		m_previewType = type;

		QVariantList qvColor = getNozzleColorList();
		polyEffect->setNozzleColorList(qvColor);
		m_previewNode = new GCodePreviewNode();
		m_previewNode->setPose(pose);
		m_previewNode->setEffect(polyEffect, monoEffect);

		for (int t = 0; t < (int)GCodeComponentType::Count; t++)
		{
			QList<Qt3DRender::QGeometry*> geometries;
			//QList<Qt3DRender::QGeometryRenderer*> geometries;
			if (t < 0 || t > m_gcodePath.blocks.size())
				continue;


			crslice2::GCodeBlockRenderData& blockData = m_gcodePath.blocks[t];
			int renderGroupCount = blockData.verticeGroups.size();
			for (int i = 0; i < renderGroupCount; ++i)
			{
				std::vector<float>& vertices = blockData.verticeGroups[i];
				std::vector<unsigned short>& indices = blockData.indexGroups[i];
				std::vector<crslice2::StepInfoPtr>& steps = blockData.stepInfoGroups[i];

				//Qt3DRender::QGeometryRenderer* render = new Qt3DRender::QGeometryRenderer(m_root);
				Qt3DRender::QGeometry* geometry = new Qt3DRender::QGeometry(NULL);
				//render->setGeometry(geometry);

				{	// vertices + normal
					if (t == (int)GCodeComponentType::Travel)
					{	//travel has no normal
						int vlength = vertices.size() * sizeof(float);
						Qt3DRender::QBuffer* qBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
						qBuffer->setData(QByteArray((const char*)vertices.data(), vlength));
						Qt3DRender::QAttribute* positionAttribute = new Qt3DRender::QAttribute(qBuffer, Qt3DRender::QAttribute::defaultPositionAttributeName(),
							Qt3DRender::QAttribute::Float, 3, vlength / 3, 0, 12);
						geometry->addAttribute(positionAttribute);
					}
					else
					{
						int vlength = vertices.size() * sizeof(float);
						Qt3DRender::QBuffer* qBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
						qBuffer->setData(QByteArray((const char*)vertices.data(), vlength));
						Qt3DRender::QAttribute* positionAttribute = new Qt3DRender::QAttribute(qBuffer, Qt3DRender::QAttribute::defaultPositionAttributeName(),
							Qt3DRender::QAttribute::Float, 3, vlength / 6, 0, 24);
						geometry->addAttribute(positionAttribute);

						Qt3DRender::QAttribute* normalAttribute = new Qt3DRender::QAttribute(qBuffer, Qt3DRender::QAttribute::defaultNormalAttributeName(),
							Qt3DRender::QAttribute::Float, 3, vlength / 6, 12, 24);
						geometry->addAttribute(normalAttribute);
					}
				}
				{ // indices
					int indexCount = indices.size();
					Qt3DRender::QBuffer* indexBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer);
					indexBuffer->setData(QByteArray((const char*)indices.data(), sizeof(unsigned short) * indexCount));
					Qt3DRender::QAttribute* indexAttribute = new Qt3DRender::QAttribute(indexBuffer, Qt3DRender::QAttribute::UnsignedShort, 1, indexCount);
					indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
					geometry->addAttribute(indexAttribute);
				}
				if (!steps.empty()) { // other 
					int stepCount = 0;
					for (const crslice2::StepInfoPtr& stepInfo : steps)
						stepCount += stepInfo.span;

					{	// step
						float* fbuffer = new float[stepCount];
						int index = 0;
						int singleSize = sizeof(float);
						for (const crslice2::StepInfoPtr& step : steps)
						{
							for (int i = 0; i < step.span; ++i)
								fbuffer[index++] = step.moveId;
						}

						Qt3DRender::QBuffer* qBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
						qBuffer->setData(QByteArray((char*)fbuffer, singleSize * stepCount));
						Qt3DRender::QAttribute* stepAttribute = new Qt3DRender::QAttribute(qBuffer, "vertexStep", Qt3DRender::QAttribute::Float, 1, stepCount);
						geometry->addAttribute(stepAttribute);
						delete fbuffer;
					}
					if (t == (int)GCodeComponentType::Extrude)
					{	
						{
							// role
							char* fbuffer = new char[stepCount];
							int index = 0;
							int singleSize = sizeof(char);
							for (const crslice2::StepInfoPtr& step : steps)
							{
								const crslice2::StepInfo& stepInfo = m_gcodePath.getStepInfo(step.moveId);
								for (int i = 0; i < step.span; ++i)
									fbuffer[index++] = stepInfo.role;
							}
							Qt3DRender::QBuffer* qBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
							qBuffer->setData(QByteArray((char*)fbuffer, singleSize * stepCount));
							Qt3DRender::QAttribute* stepAttribute = new Qt3DRender::QAttribute(qBuffer, "vertexRole", Qt3DRender::QAttribute::Byte, 1, stepCount);
							geometry->addAttribute(stepAttribute);
							delete fbuffer;
						}
						{
							Qt3DRender::QBuffer* qBuffer = createBuffer(type, stepCount, steps);
							Qt3DRender::QAttribute* attribute = new Qt3DRender::QAttribute(qBuffer, "vertexFlag", Qt3DRender::QAttribute::Byte, 1, stepCount);
							m_attributesCache[GCodeComponentType(t)] << attribute;
							geometry->addAttribute(attribute);
						}
					}
				}
				geometries << geometry;
			}
			// m_geometriesCache[GCodeComponentType(t)] = geometries;
			m_previewNode->setGeometries(GCodeComponentType(t), geometries);
		}

		if (m_isBuildGeometries)
		{
			for (auto& block : m_gcodePath.blocks)
			{
				block.verticeGroups.clear();
				block.indexGroups.clear();
			}
		}
	}

	QList<Qt3DRender::QGeometry*> SliceAttain::getGeometries(GCodeComponentType type)
	{
		return m_geometriesCache[type];
	}

	GCodePreviewNode* SliceAttain::previewNode()
	{
		return m_previewNode;
	}

	void SliceAttain::updateGCodePreviewType(GCodePreviewType type)
	{
		if (!m_isBuildGeometries || m_previewType == type)
			return;

		m_previewType = type;
		//for (int t = 0; t < (int)GCodeComponentType::Count; t++)
		{
			int t = (int)GCodeComponentType::Extrude;
			crslice2::GCodeBlockRenderData& blockData = m_gcodePath.blocks[t];
			int renderGroupCount = blockData.stepInfoGroups.size();
			int attributeIndex = 0;
			for (int i = 0; i < renderGroupCount; ++i)
			{
				std::vector<crslice2::StepInfoPtr>& steps = blockData.stepInfoGroups[i];
				Qt3DRender::QAttribute* attr = m_attributesCache[GCodeComponentType(t)][attributeIndex++];

				int stepCount = 0;
				for (const crslice2::StepInfoPtr& stepPtr : steps)
					stepCount += stepPtr.span;

				Qt3DRender::QBuffer* qBuffer = createBuffer(type, stepCount, steps);
				attr->setBuffer(qBuffer);
			}
		}
	}

	QVariantList SliceAttain::getNozzleColorList()
	{
		QVariantList qvColor;
		if (m_gcodePath.tools.empty())
		{
			std::vector<trimesh::vec3> colors = currentColors();
			for (const trimesh::vec3& color : colors)
			{
				qvColor << QColor(color[0], color[1], color[2]);
			}
		}
		else 
		{
			for (const crslice2::GCodePath::ExtruderColor& color : m_gcodePath.tools)
			{
				qvColor << QColor(color.r, color.g, color.b);
			}
		}
		return qvColor;
	}
}

#include "sliceattain.h"
#include "cxgcode/gcodehelper.h"
#include "crslice/gcode/gcodedata.h"
#include "data/kernelmacro.h"
#include "crslice/gcode/parasegcode.h"
#include "crslice2/gcodeprocessor.h"

#include "interface/machineinterface.h"
#include "interface/appsettinginterface.h"

#include "qtusercore/string/resourcesfinder.h"
#include "qtusercore/module/systemutil.h"
#include "qtuser3d/trimesh2/conv.h"
// #include "qtuser3d/geometry/geometrycreatehelper.h"
// #include "qtuser3d/geometry/bufferhelper.h"
// #include "qtuser3d/refactor/xentity.h"
#include "cxkernel/utils/utils.h"

#include <QtCore/QUuid>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QThread>
#include <external/interface/spaceinterface.h>
#include "boost/nowide/cstdio.hpp"

#include <QFile>
#include <QIODevice>

#include "external/slice/gcodepreviewnode.h"

using namespace qtuser_3d;
namespace creative_kernel
{
	SliceAttain::SliceAttain(SliceResultPointer result, const QString& sliceName, SliceAttainType type, QObject* parent)
		:QObject(parent)
		, m_cache(nullptr)
		, m_sliceName(sliceName)
		, m_type(type)
		, m_result(result)
	{
		QString path = QString("%1/%2").arg(TEMPGCODE_PATH).arg(QUuid::createUuid().toString(QUuid::WithoutBraces));
		mkMutiDir(path);

		QString name = m_sliceName;
		m_tempGCodeFileName = QString("%1/%2").arg(path).arg(name);
		int maxPath = qtuser_core::getSystemMaxPath() - 7;
		if (m_tempGCodeFileName.length() > maxPath)
		{
			m_tempGCodeFileName = m_tempGCodeFileName.left(maxPath);
		}
		m_tempGCodeImageFileName = QString("%1_image").arg(m_tempGCodeFileName);
		if (m_tempGCodeImageFileName.length() > maxPath)
		{
			m_tempGCodeImageFileName = QString("%1_image").arg(m_tempGCodeImageFileName.left(maxPath - 6));
		}
		m_tempGCodeFileName += ".gcode";
		m_tempGCodeImageFileName += ".gcode";
		m_sourceGCodeFile = m_tempGCodeFileName;

		static int id = 0;
		m_id = id++;
	}

	SliceAttain::~SliceAttain()
	{
		QFileInfo info(m_tempGCodeFileName);
		QString path = info.absolutePath();

		clearPath(path);
		QDir dir;
		dir.setPath(path);
		dir.removeRecursively();

		if (m_previewNode)
			m_previewNode->deleteLater();
	}

	int SliceAttain::moveId(int layer, int step)
	{
		if (layer > m_gcodePath.layers.size())
			return m_gcodePath.layers.back().last;

		crslice2::GCodeLayerInfo& _layer = m_gcodePath.layers[layer - 1];
		if (step >= 0)
		{
			int id = _layer.first + step - 1;
			if (id > _layer.last)
				id = _layer.last;

			return id < 0 ? _layer.first : id;
		}
		else
		{
			return _layer.last;
		}
	}

	int SliceAttain::firstMoveId(int layer)
	{
		if (layer > m_gcodePath.layers.size())
			return m_gcodePath.layers.back().last;

		crslice2::GCodeLayerInfo& _layer = m_gcodePath.layers[layer - 1];		
		return _layer.first;
	}

	bool SliceAttain::isLastStep(int layer, int step)
	{
		auto _layer = m_gcodePath.layers[layer - 1];
		int stepSize = _layer.stepCount;
		return stepSize == step;
	}

	int SliceAttain::id()
	{
		return m_id;
	}

	void SliceAttain::setId(int id)
	{
		m_id = id;
	}

	void SliceAttain::build(ccglobal::Tracer* tracer)
	{
		builder.build(m_result, tracer);
	}

	void SliceAttain::loadGCodePath()
	{
		crslice2::process_file(cxkernel::qString2String(m_sourceGCodeFile), m_gcodePath);
	}

	void SliceAttain::getDataFromOrca()
	{
		//getTime
		std::vector<std::vector<std::pair<int, float>>> times;
		FILE* fp = boost::nowide::fopen(cxkernel::qString2String(m_sourceGCodeFile).c_str(), "r");
		if (fp == nullptr)
			return;
		if (fp) fclose(fp);

		crslice2::process_file(cxkernel::qString2String(m_sourceGCodeFile), times);
		if (times.size() > 1)
		{
			gcode::GCodeParseInfo& info = builder.getGCodeStruct().getParam();
			info.roles_time = times[0];

			if (times.size() > 2)
				if (!times[2].empty())
					info.printTime = times[2][0].second;

			//merge cura time
			{
				if (info.timeParts.OuterWall > 0.0f)
					info.roles_time.push_back(std::pair<int, float>(1, info.timeParts.OuterWall));
				if (info.timeParts.InnerWall > 0.0f)
					info.roles_time.push_back(std::pair<int, float>(2, info.timeParts.InnerWall));
				if (info.timeParts.Skin > 0.0f)
					info.roles_time.push_back(std::pair<int, float>(3, info.timeParts.Skin));
				if (info.timeParts.Support > 0.0f)
					info.roles_time.push_back(std::pair<int, float>(4, info.timeParts.Support));
				if (info.timeParts.SkirtBrim > 0.0f)
					info.roles_time.push_back(std::pair<int, float>(5, info.timeParts.SkirtBrim));
				if (info.timeParts.Infill > 0.0f)
					info.roles_time.push_back(std::pair<int, float>(6, info.timeParts.Infill));
				if (info.timeParts.SupportInfill > 0.0f)
					info.roles_time.push_back(std::pair<int, float>(7, info.timeParts.SupportInfill));
				if (info.timeParts.MoveCombing > 0.0f)
					info.roles_time.push_back(std::pair<int, float>(8, info.timeParts.MoveCombing));
				if (info.timeParts.MoveRetraction > 0.0f)
					info.roles_time.push_back(std::pair<int, float>(9, info.timeParts.MoveRetraction));
				if (info.timeParts.PrimeTower > 0.0f)
					info.roles_time.push_back(std::pair<int, float>(11, info.timeParts.PrimeTower));
			}
			builder.getGCodeStruct().setParam(info);
			//builder.parseInfo.layers_time = times[1];
			for (auto& time : times[1])
			{
				if (time.first >= -1)
				{
					auto iter = builder.getGCodeStruct().m_layerTimes.find(time.first+1);
					if (iter != builder.getGCodeStruct().m_layerTimes.end())
						iter->second = time.second;
					else
						builder.getGCodeStruct().m_layerTimes.insert(std::pair<int, float>(time.first+1, time.second));
				}
			}
		}
	}

	void SliceAttain::build_paraseGcode(ccglobal::Tracer* tracer)
	{
		gcode::paraseGcodeAndPreview(cxkernel::qString2String(m_sourceGCodeFile), this, tracer);

		gcode::GCodeParseInfo& info = builder.getGCodeStruct().getParam();
		if (info.producer == gcode::GProducer::OrcaSlicer && !info.have_roles_time)
		{
			getDataFromOrca();
		}

		builder.build_with_image(m_result, tracer);
	}

	void SliceAttain::preparePreviewData(ccglobal::Tracer* tracer)
	{
		builder.build();
		//提前预览阶段使用 gvt_structure 类型
		// prepareVisualTypeData(gcode::GCodeVisualType::gvt_structure);
	}


	void SliceAttain::setSourceGCodeFile(const QString& fileName)
	{
		m_sourceGCodeFile = fileName;
	}

	QString SliceAttain::sliceName()
	{
		
		return m_sliceName;
	}

	QString SliceAttain::sourceFileName()
	{
		return m_sourceGCodeFile;
	}

	bool SliceAttain::isFromGcode()
	{
		return m_sourceGCodeFile != m_tempGCodeFileName;
	}

	bool SliceAttain::isFromFile()
	{
		if (getEngineType() == EngineType::ET_ORCA)
		{
			return false;
		}
		return m_type == SliceAttainType::sat_file;
	}

	QString SliceAttain::nozzleDiameter()
	{
		return QString::number(m_gcodePath.nozzleDiameter, 'f', 1);
	}

	QString SliceAttain::printerName()
	{
		return QString(m_gcodePath.printerModel.data());
	}

	int SliceAttain::printTime()
	{
		return m_gcodePath.printTime;
	}

	QString SliceAttain::material_weight()
	{
		return QString::number(m_gcodePath.materialWeight, 'f', 2);
		}

	QString SliceAttain::printing_time()
	{
		int printTime = m_gcodePath.printTime;
		QString str = QString("%1h%2m%3s").arg(printTime / 3600)
			.arg(printTime / 60 % 60)
			.arg(printTime % 60);
		return str;
	}

	QString SliceAttain::material_money()
	{
		return QString::number(m_gcodePath.materialCost, 'f', 2);
		}

	QString SliceAttain::material_length()
	{
		return QString::number(m_gcodePath.materialLength);
	}

	trimesh::box3 SliceAttain::box()
	{
		return m_result->inputBox;
	}

	int SliceAttain::layers()
	{
		return m_gcodePath.layers.size();
	}

	int SliceAttain::steps(int layer)
	{
		auto& _layer = m_gcodePath.layers[layer - 1];
		return _layer.stepCount;
	}

	int SliceAttain::totalSteps()
	{
		return m_gcodePath.layers.back().last;
	}

	int SliceAttain::nozzle()
	{
		return builder.baseInfo.nNozzle;
	}

	float SliceAttain::minSpeed()
	{
		return m_gcodePath.speedRange.min;
	}

	float SliceAttain::maxSpeed()
	{
		return m_gcodePath.speedRange.max;
	}

	float SliceAttain::minTimeOfLayer()
	{
		return m_gcodePath.layerDurationRange.min;
	}

	float SliceAttain::maxTimeOfLayer()
	{
		return m_gcodePath.layerDurationRange.max;
	}

	float SliceAttain::minFlowOfStep()
	{
		return m_gcodePath.volumetricRateRange.min;
	}

	float SliceAttain::maxFlowOfStep()
	{
		return m_gcodePath.volumetricRateRange.max;
	}

	float SliceAttain::minLineWidth()
	{
		return m_gcodePath.widthRange.min;
	}

	float SliceAttain::maxLineWidth()
	{
		return m_gcodePath.widthRange.max;
	}

	float SliceAttain::minLayerHeight()
	{
		return m_gcodePath.heightRange.min;
	}

	float SliceAttain::maxLayerHeight()
	{
		return m_gcodePath.heightRange.max;
	}

	float SliceAttain::minTemperature()
	{
		return m_gcodePath.temperatureRange.min;
	}

	float SliceAttain::maxTemperature()
	{
		return m_gcodePath.temperatureRange.max;
	}

	float SliceAttain::minAcc()
	{
		return m_gcodePath.accRange.min;
	}

	float SliceAttain::maxAcc()
	{
		return m_gcodePath.accRange.max;
	}

	float SliceAttain::layerHeight(int layer)
	{
		trimesh::vec3 pos = traitPosition(layer, 1);
		float height = builder.layerHeight(layer);
		if (height < 0)
			return pos.z;
		else
			return /*pos.z + */height;
	}

	float SliceAttain::layerHeight()
	{
		return builder.parseInfo.layerHeight;
	}

	float SliceAttain::lineWidth()
	{
		return builder.parseInfo.lineWidth;
	}

	std::vector<std::pair<int, float>> SliceAttain::getTimeParts_orca()
	{
		return m_gcodePath.timeParts;
	}

	bool SliceAttain::isCuraProducer()
	{
		return false;
	}

	QImage* SliceAttain::getImageFromGcode()
	{
		if (m_result)
		{
			if (!m_result->previewsData.empty())
			{
				QImage* image=new QImage();
				image->loadFromData(&m_result->previewsData.back()[0], m_result->previewsData.back().size());
				return image;
			}
		}
		return nullptr;
	}

	void SliceAttain::getFilamentsList(std::vector<std::pair<int, double>>& volumes_per_extruder, std::vector<std::pair<int, double>>& flush_per_filament, std::vector<std::pair<int, double>>& volumes_per_tower)
	{
		volumes_per_extruder.clear();
		for (const auto& pair : m_gcodePath.perFilamentUsedLength)
		{
			int extruder = pair.first;
			volumes_per_extruder.push_back(std::pair<int, double>(extruder, m_gcodePath.perFilamentUsedLength[extruder]));
			volumes_per_extruder.push_back(std::pair<int, double>(extruder, m_gcodePath.perFilamentUsedWeight[extruder]));
		}

		flush_per_filament.clear();
		for (const auto& pair : m_gcodePath.flushPerFilamentUsedLength)
		{
			int extruder = pair.first;
			flush_per_filament.push_back(std::pair<int, double>(extruder, m_gcodePath.flushPerFilamentUsedLength[extruder]));
			flush_per_filament.push_back(std::pair<int, double>(extruder, m_gcodePath.flushPerFilamentUsedWeight[extruder]));
		}

		volumes_per_tower.clear();
		for (const auto& pair : m_gcodePath.towerPerFilamentUsedLength)
		{
			int extruder = pair.first;
			volumes_per_tower.push_back(std::pair<int, double>(extruder, m_gcodePath.towerPerFilamentUsedLength[extruder]));
			volumes_per_tower.push_back(std::pair<int, double>(extruder, m_gcodePath.towerPerFilamentUsedWeight[extruder]));
		}

	}

	const crslice2::StepInfo& SliceAttain::findStep(int layer, int step, bool *ok)
	{
		int id = moveId(layer, step);
		const crslice2::StepInfo& stepInfo = m_gcodePath.getStepInfo(id);
		*ok = stepInfo.moveId >= 0;
		return stepInfo;
	}

	float SliceAttain::traitSpeed(int layer, int step)
	{
		bool ok;
		const crslice2::StepInfo& stepInfo = findStep(layer, step, &ok);
		if (ok)
			return stepInfo.speed;
		else
			return 0;
	}

	trimesh::vec3 SliceAttain::traitPosition(int layer, int step)
	{
		bool ok = false;
		trimesh::vec3 pos(0, 0, 0);
		while (!ok && !isLastStep(layer, step))
		{
			const crslice2::StepInfo& stepInfo = findStep(layer, step, &ok);
			pos = trimesh::vec3(stepInfo.position);
			step++;
		}
		return pos;
	}

	float SliceAttain::traitDuration(int layer, int step)
	{
		bool ok;
		const crslice2::StepInfo& stepInfo = findStep(layer, step, &ok);
		if (ok)
			return stepInfo.layerDuration;
		else
			return 0;
	}
	float SliceAttain::traitLayerHeight(int layer, int step)
	{
		bool ok;
		const crslice2::StepInfo& stepInfo = findStep(layer, step, &ok);
		if (ok)
			return stepInfo.height;
		else
			return 0;
	}
	float SliceAttain::traitAcc(int layer, int step)
	{
		bool ok;
		const crslice2::StepInfo& stepInfo = findStep(layer, step, &ok);
		if (ok)
			return stepInfo.acc;
		else
			return 0;
	}
	float SliceAttain::traitLineWidth(int layer, int step)
	{
		bool ok;
		const crslice2::StepInfo& stepInfo = findStep(layer, step, &ok);
		if (ok)
			return stepInfo.width;
		else
			return 0;
	}
	float SliceAttain::traitFlow(int layer, int step)
	{
		bool ok;
		const crslice2::StepInfo& stepInfo = findStep(layer, step, &ok);
		if (ok)
			return stepInfo.volumetricRate;
		else
			return 0;
	}
	float SliceAttain::traitLayerTime(int layer, int step)
	{
		bool ok;
		const crslice2::StepInfo& stepInfo = findStep(layer, step, &ok);
		if (ok)
			return stepInfo.layerDuration;
		else
			return 0;
	}
	float SliceAttain::traitFanSpeed(int layer, int step)
	{
		bool ok;
		const crslice2::StepInfo& stepInfo = findStep(layer, step, &ok);
		if (ok)
			return stepInfo.fanSpeed;
		else
			return 0;
	}
	float SliceAttain::traitTemperature(int layer, int step)
	{
		bool ok;
		const crslice2::StepInfo& stepInfo = findStep(layer, step, &ok);
		if (ok)
			return stepInfo.temperature;
		else
			return 0;
	}

	trimesh::fxform SliceAttain::modelMatrix()
	{
		return builder.parseInfo.xf4;
	}

	int SliceAttain::findViewIndexFromStep(int layer, int nStep)
	{
		int stepId = moveId(layer, nStep);
		int layerId = firstMoveId(layer);
		return m_gcodePath.gcodeMap[stepId] - m_gcodePath.gcodeMap[layerId];
		}

	int SliceAttain::findStepFromViewIndex(int layer, int viewIndex)
	{
		int layerId = firstMoveId(layer);
		int layerIndex = m_gcodePath.gcodeMap[layerId];
		viewIndex += layerIndex;

		int result = -1;
		int lineCount = m_gcodePath.gcodeMap.size();
		for (int i = 0, count = lineCount - 1; i < count; ++i)
		{
			int curr = m_gcodePath.gcodeMap[i];
			int last = m_gcodePath.gcodeMap[i + 1];
			if (curr == viewIndex || last > viewIndex)
			{
				result = i;
				break;
					}
				}
		if (result == -1)
		{
			result = m_gcodePath.gcodeMap[lineCount - 1];
			}

		result -= layerId;
		return result;
		}

	void SliceAttain::requestLayerGcode(int layer)
	{
		if (m_readLayer == layer)
			return;

		m_layerCodes.clear();
		QFile file(m_sourceGCodeFile);
		if (!file.open(QIODevice::ReadOnly))
			return;

		int firstLineId = firstMoveId(layer);
		int lastLineId = moveId(layer, -1);
		int firstLine = m_gcodePath.gcodeMap[firstLineId];
		int lastLine = m_gcodePath.gcodeMap[lastLineId];

		for (uint64_t id = firstLine; id <= lastLine; ++id)
		{
			int _id = id - 2;
			const size_t start = id == 1 ? 0 : m_gcodePath.gcodeLinesEnd[_id < 0 ? 0 : _id];
			//const size_t original_len = m_gcodePath.gcodeLinesEnd[id - 1] - start;
			//const size_t len = std::min(original_len, (size_t)55);
			file.reset();
			file.seek(start);
			auto line = file.readLine();
			m_layerCodes << line;
		}
		file.close();

		m_readLayer = layer;
	}

	QStringList SliceAttain::layerGcode()
	{
		return m_layerCodes;
	}

    float SliceAttain::getMachineHeight()
    {
		return m_gcodePath.printableHeight;
    }

    float SliceAttain::getMachineWidth()
    {
		return m_gcodePath.printableWidth;
    }

    float SliceAttain::getMachineDepth()
    {
		return m_gcodePath.printableLength;
    }

	void SliceAttain::saveGCode(const QString& fileName, QImage* previewImage)
	{
		QString imageStr;
		if (previewImage && !isFromGcode())
		{
			float layerHeight = builder.parseInfo.layerHeight;
			QString screenSize = QString(QString::fromLocal8Bit(builder.parseInfo.screenSize.c_str()));
            QString exportFormat = QString(QString::fromLocal8Bit(builder.parseInfo.exportFormat.c_str()));

			previewImage = cxsw::resizeModule(previewImage);
			if (exportFormat == "bmp")
			{
				cxsw::image2String(*previewImage, 64, 64, true, imageStr);
				cxsw::image2String(*previewImage, 400, 400, false, imageStr);
			}
			else
			{
				QImage minPreImg;
				QImage maxPreImg;
				if (screenSize == "CR-10 Inspire")
				{
					minPreImg = previewImage->scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);
					maxPreImg = previewImage->scaled(600, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				}
				else if (screenSize == "CR-200B Pro")
				{
					minPreImg = previewImage->scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
					maxPreImg = previewImage->scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				}
				else if (screenSize == "Ender-3 S1")
				{
					maxPreImg = previewImage->scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				}
				else if (screenSize == "Ender-5 S1")
				{
					maxPreImg = previewImage->scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				}
				else//Sermoon D3
				{
					minPreImg = previewImage->scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);
					maxPreImg = previewImage->scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				}

				if (!minPreImg.isNull())
				{
					cxsw::getImageStr(imageStr, &minPreImg, builder.baseInfo.layers, exportFormat, layerHeight, false, SLICE_PATH);
				}
				cxsw::getImageStr(imageStr, &maxPreImg, builder.baseInfo.layers, exportFormat, layerHeight, exportFormat == "png", SLICE_PATH);
			}
			if (previewImage)
				delete previewImage;
		}
		else if (isFromGcode())
		{
			//float layerHeight = builder.parseInfo.layerHeight;
			QString screenSize = QString(QString::fromLocal8Bit(builder.parseInfo.screenSize.c_str()));
			QString exportFormat = QString(QString::fromLocal8Bit(builder.parseInfo.exportFormat.c_str()));

			QString imgSavePath = QString("%1/imgPreview.%2").arg(SLICE_PATH).arg(exportFormat);
			QImage* image = getImageFromGcode();
			if(image)
				image->save(imgSavePath);
			else
			{
				QFile::remove(imgSavePath);
			}
			//cxsw::getImageStr(imageStr, getImageFromGcode(), builder.baseInfo.layers, exportFormat, layerHeight, false, SLICE_PATH);
		}

		if (!isFromGcode())
		{
			QString srcName = m_sourceGCodeFile;
			if (imageStr.isEmpty())
			{
				if (!QFile(fileName).exists())
				{
					qtuser_core::copyFile(srcName, fileName);
				}
			}
			else
			{
				QFile srcfile(srcName);
				QFile file(fileName);
				if (srcfile.open(QIODevice::ReadOnly) && file.open(QIODevice::WriteOnly))
				{
					file.write(imageStr.toLocal8Bit(), imageStr.size());
					file.write("\r\n", 2);
					file.write(srcfile.readAll());
				}
			}
		}
	}

	void SliceAttain::saveTempGCode()
	{
		if(!isFromGcode())
			saveGCode(m_tempGCodeFileName, nullptr);
	}

	void SliceAttain::saveTempGCodeWithImage(QImage& image)
	{
		saveGCode(m_tempGCodeImageFileName, &image);
	}

	QString SliceAttain::tempGCodeFileName()
	{
        return m_tempGCodeFileName;
    }

    QString SliceAttain::tempGcodeThumbnail()
    {
		QString exportFormat = QString(QString::fromLocal8Bit(builder.parseInfo.exportFormat.c_str()));
        return QString("%1/imgPreview.%2").arg(SLICE_PATH).arg(exportFormat);
    }

	QString SliceAttain::tempGCodeImageFileName()
	{
		if (m_tempGCodeImageFileName.isEmpty())
			return m_tempGCodeFileName;
		return m_tempGCodeImageFileName;
	}

	void SliceAttain::saveGCodeImage()
	{
		int maxIndex = -1;
		int maxSize = -1;
		for (int i = 0, count = m_gcodePath.imageData.size(); i < count; ++i)
		{
			auto& imageData = m_gcodePath.imageData[i];
			if (imageData.first > maxSize)
			{
				maxSize = imageData.first;
				maxIndex = i;
			}
		}
		if (maxIndex < 0)
			return;

		QString imgSavePath = QString("%1/imgPreview.%2").arg(SLICE_PATH).arg("png");
		QFile file(imgSavePath);
		if (file.open(QIODevice::WriteOnly))
		{
			file.write((const char*)m_gcodePath.imageData[maxIndex].second.data(), m_gcodePath.imageData[maxIndex].first);
			file.close();
		}
	}

	void SliceAttain::getPathData(const trimesh::vec3 point, float e, int type, bool isOrca, bool isseam)
	{
		if (m_type == SliceAttainType::sat_file)
			builder.getGCodeStruct().getPathData(point, e, type, true, isOrca,isseam);
		else 
			builder.getGCodeStruct().getPathData(point,e, type, false,isOrca,isseam);
	}
	void SliceAttain::getPathDataG2G3(const trimesh::vec3 point, float i, float j, float e, int type,int p, bool isG2, bool isOrca, bool isseam)
	{
		if (m_type == SliceAttainType::sat_file)
			builder.getGCodeStruct().getPathDataG2G3(point, i, j, e, type, p, isG2, true,isOrca,isseam);
		else
			builder.getGCodeStruct().getPathDataG2G3(point, i, j, e, type, p, isG2, false,isOrca, isseam);
	}
	void SliceAttain::setParam(gcode::GCodeParseInfo& pathParam)
	{
		builder.getGCodeStruct().setParam(pathParam);
	}
	void SliceAttain::setLayer(int layer)
	{
		builder.getGCodeStruct().setLayer(layer);
		return;
	}

	void SliceAttain::setLayers(int layer)
	{
		if (layer >= 0)
			builder.baseInfo.layers = layer;
	}

	void SliceAttain::setSpeed(float s)
	{
		builder.getGCodeStruct().setSpeed(s);
	}
	void SliceAttain::setAcc(float acc)
	{
		builder.getGCodeStruct().setAcc(acc);
	}
	void SliceAttain::setTEMP(float temp)
	{
		builder.getGCodeStruct().setTEMP(temp);
	}
	void SliceAttain::setExtruder(int nr)
	{
		builder.getGCodeStruct().setExtruder(nr);
	}
	void SliceAttain::setFan(float fan)
	{
		builder.getGCodeStruct().setFan(fan);
	}
	void SliceAttain::setZ(float z, float h)
	{
		if (m_type == SliceAttainType::sat_file)
			builder.getGCodeStruct().setZ_from_gcode(z);
		else
			builder.getGCodeStruct().setZ(z,h);
	}
	void SliceAttain::setE(float e)
	{
		builder.getGCodeStruct().setE(e);
	}


	void SliceAttain::setWidth(float width)
	{
		builder.getGCodeStruct().setWidth(width);
	}

	void SliceAttain::setLayerHeight(float height)
	{
		builder.getGCodeStruct().setLayerHeight(height);
	}

	void SliceAttain::setLayerPause(int pause)
	{
		builder.getGCodeStruct().setLayerPause(pause);
	}

	void SliceAttain::setTime(float time)
	{
		builder.getGCodeStruct().setTime(time);
	}

	void SliceAttain::getNotPath()
	{
		builder.getGCodeStruct().getNotPath();
	}

	void SliceAttain::set_data_gcodelayer(int layer, const std::string& gcodelayer)//set a layer data
	{
		if (!m_result)
		{
			return ;
		}

		if (layer == -1)
			m_result->set_gcodeprefix(gcodelayer);
		else
			m_result->set_data_gcodelayer(layer, gcodelayer);
	}

	void SliceAttain::setNozzleColorList(std::string& colorList, std::string& defaultColorList, std::string& defaultType)
	{
		builder.getGCodeStruct().setNozzleColorList(colorList, defaultColorList, defaultType);
	}
	void SliceAttain::getNozzleColorList(std::vector<std::string>& colorList, std::vector<std::string>& defaultColorList, std::vector<std::string>& defaultType)
	{
		colorList = m_gcodePath.fliamentColors;
		defaultColorList = m_gcodePath.defaultFliamentColorsWithUnused;
		defaultType = m_gcodePath.fliamentTypes;
	}

	void SliceAttain::onSupports(int layerIdx, float z, float thickness, const std::vector<std::vector<trimesh::vec3>>& paths)
	{
	}

	void SliceAttain::setSceneBox(const trimesh::box3& box)
	{
	}

	void SliceAttain::writeImages(const std::vector<std::pair<trimesh::ivec2, std::vector<unsigned char>>>& images)
	{
		if (images.empty())
		{
			return;
		}
		QString imgSavePath = QString("%1/imgPreview.%2").arg(SLICE_PATH).arg("png");
		int max_size=0;
		int max_index=0;
		for(int i=0; i<images.size(); i++)
		{
			std::pair<trimesh::ivec2, std::vector<unsigned char> > image = images[i];
			if(image.second.size()>=max_size)
			{
				max_size = image.second.size();
				max_index = i;
			}
			
		}
		QFile file(imgSavePath);
		if (file.open(QIODevice::WriteOnly))
		{
			file.write((const char*)images[max_index].second.data(), images[max_index].second.size());
			file.close();
		}
	}

	qtuser_3d::Box3D SliceAttain::getGCodePathBoundingBox()
	{
		return qtuser_3d::triBox2Box3D(m_gcodePath.boundingBox);
	}

	QMap<QString, QPair<QString, int64_t> > SliceAttain::getSliceWarningDetails()
	{
		auto itrWarn = m_sliceWarningDetails.begin();
		for (; itrWarn != m_sliceWarningDetails.end(); itrWarn++)
		{
			if (QString("Path_Conflict") == itrWarn.key())
			{
				QString strDetails = itrWarn.value().first;

				int layerWordPos = strDetails.indexOf("#");
				int heightWordPos = strDetails.lastIndexOf("#");
				int heightValuePos = strDetails.indexOf("$");
				int lastPartPos = strDetails.lastIndexOf("$");

				//text = (boost::format(_u8L("Conflicts of gcode paths have been found at layer# %d, #height$ %.2f mm.$ Please separate the conflicted objects further@%s")) % layer % height % objName1).str();
				QString heightValuePart = strDetails.mid(heightValuePos + 1, lastPartPos - heightValuePos - 1-4);
				int layerIdx = builder.layerIndex(heightValuePart.toFloat());
				float previewHeight = builder.layerHeight(layerIdx);
				itrWarn.value().first.replace(layerWordPos+2, heightWordPos - layerWordPos - 2, QString::number(layerIdx) + ", ");

				heightValuePos = itrWarn.value().first.indexOf("$");
				lastPartPos = itrWarn.value().first.lastIndexOf("$");
				itrWarn.value().first.replace(heightValuePos + 2, lastPartPos - heightValuePos - 2, QString::number(previewHeight) + " mm.");
			}
		}

		return m_sliceWarningDetails;
	}

	void SliceAttain::setSliceWarningDetails(const QMap<QString, QPair<QString, int64_t> >& sliceWarningDetails)
	{
		m_sliceWarningDetails.clear();
		m_sliceWarningDetails = sliceWarningDetails;
	}

	void SliceAttain::setGCodePath(crslice2::GCodePath gcodePath)
	{
		m_gcodePath = gcodePath;
}

	crslice2::GCodePath SliceAttain::gcodePath()
	{
		return m_gcodePath;
	}
}

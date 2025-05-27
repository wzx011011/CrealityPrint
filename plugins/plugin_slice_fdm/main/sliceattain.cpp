#include "sliceattain.h"

#include "qtuser3d/geometry/linecreatehelper.h"
#include "qtuser3d/geometry/trianglescreatehelper.h"
#include "qtuser3d/geometry/gridcreatehelper.h"
#include <QPainter>

#include "interface/spaceinterface.h"

#include "report/slicereport.h"
#include "us/slicermanager.h"
//#include "core/dlpdatahelper.h"
#include "core/gcodehelper.h"
#include "render/fdmlayerbuilder.h"

#include "utils/trimesh2qgeometryrenderer.h"
#include "trimesh2/TriMesh_algo.h"
#include"thumbnail/thumbnail.h"

#include <regex>
#include <QStandardPaths>

using namespace qtuser_3d;
using namespace creative_kernel;

AttainInfo::AttainInfo()
	:attainType(AttainType::at_fdm)
	, layers(0)
	, nNozzle(1)
	, volume(0.0)
	, printTime(0)
	, materialLenth(0.0f)
	, builder(nullptr)
{

}

AttainInfo::~AttainInfo()
{
	if (builder)
	{
		delete builder;
		builder = nullptr;
	}

	layerCode.clear();
	prefixCode.clear();
}

SliceAttain::SliceAttain(QObject* parent)
	:QObject(parent)
	, m_report(nullptr)
	, m_cache(nullptr)
{	
}

SliceAttain::~SliceAttain()
{
}

CXSLiceReport* SliceAttain::getReport()
{
	if (!m_report)
	{
		ReportInfo reportinfo;
		reportinfo.layers = m_info.layers;
		reportinfo.materialLenth = m_info.materialLenth;
		reportinfo.printTime = m_info.printTime;
		reportinfo.volume = m_info.volume;
		m_report = new CXSLiceReport(&reportinfo);

	}
	return m_report;
}

qtuser_3d::Box3D SliceAttain::printerBox()
{
	return m_info.printerBox;
}

qtuser_3d::Box3D SliceAttain::box()
{
	return m_info.box;
}

int SliceAttain::layers()
{
	return m_info.layers;
}

float SliceAttain::volume()
{
	return m_info.volume;
}

int SliceAttain::steps(int layer)
{
	if (layer < 0 || layer >= (int)m_info.steps.size())
		return 0;
	return m_info.steps.at(layer);
}

int SliceAttain::nozzle()
{
	return m_info.nNozzle;
}

time_t SliceAttain::printingTime()
{
	return m_info.printTime;
}

void SliceAttain::fromGlobalSetting(us::GlobalSetting* globalSettings)
{
	m_info.printerBox = baseBoundingBox();
}

void SliceAttain::build(std::vector<std::string>& layers
	, std::string& prefix, std::string& tailfix ,us::GlobalSetting* globalSettings)
{
	qDebug() << "slicedata build";
	fromGlobalSetting(globalSettings);

	m_info.prefixCode.swap(prefix);
	m_info.layerCode.swap(layers);
	m_info.tailCode.swap(tailfix);
	m_info.attainType = AttainType::at_fdm;

	float lineWidth = globalSettings->value("line_width", "1.0").toFloat();
	m_info.builder = FDMLayerBuilder::buildFromGCode(m_info.prefixCode, m_info.layerCode, lineWidth, &m_info.steps);

	if (m_info.builder)
	{
		m_info.box = m_info.builder->totalBox();
		m_info.nNozzle = m_info.builder->totalNozzleNr();
		m_info.layers = m_info.builder->totalLayers();
	}
	m_info.volume = 0.0;
	calculatePrintTimeAndMatial(m_info.prefixCode);
}

std::string SliceAttain::layerGcode(int layer)
{
	if (layer < 0 || layer >= (int)m_info.layerCode.size())
		return "";
	return m_info.layerCode.at(layer);
}

void SliceAttain::take(Qt3DRender::QGeometry** geometry, std::unordered_map<int, trimesh::vec4>& indicator)
{
	if (m_info.builder && !m_cache)
	{
		//m_info.builder->buildGeometry(geometry, indicator);
		//m_info.builder->buildBillboardGeometry(geometry, indicator);
		m_info.builder->buildBillboardNoTessGeometry(&m_cache, m_orderCache);
		delete m_info.builder;
		m_info.builder = nullptr;
	}

	if (m_cache)
	{
		*geometry = m_cache;
		indicator = m_orderCache;
	}
}

QImage* createImageWithOverlay(const QImage& baseImage, const QImage& overlayImage)
{
	QImage* imageWithOverlay = new QImage(baseImage.size(), baseImage.format());
	QImage mask = imageWithOverlay->createMaskFromColor(QRgb(0), Qt::MaskInColor);
	imageWithOverlay->setAlphaChannel(mask);
	QPainter painter(imageWithOverlay);

	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(imageWithOverlay->rect(), Qt::transparent);

	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawImage(0, 0, baseImage);

	int offsetX = (baseImage.width() - overlayImage.width()) / 2;
	int offsetY = (baseImage.height() - overlayImage.height()) / 2;
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawImage(offsetX, offsetY, overlayImage);

	painter.end();

	return imageWithOverlay;
}

QRect getModuleRect(QImage* previewImage)
{
	int imageW = previewImage->width();
	int imageH = previewImage->height();
	int dataLength = imageW * imageH * 4;
	std::vector<unsigned char> data(dataLength);
	memcpy(data.data(), previewImage->bits(), dataLength);
	int lineDataLength = imageW * 4;
	int minX = imageW, maxX = -1, minY = -1, maxY = -1;
	for (int i = 3; i < dataLength; i += 4)
	{
		if (data[i] != 0)
		{
			int x = (i % lineDataLength) / 4;
			int y = i / lineDataLength;
			if (minY == -1) minY = y;
			maxY = y;
			if (minX > x)
			{
				minX = x;
			}
			if (maxX < x)
			{
				maxX = x;
			}
		}
	}
	QRect moduleRect = QRect(minX, minY, maxX - minX, maxY - minY);
	return moduleRect;
}

QImage* resizeModule(QImage* previewImage)
{
	int imageW = previewImage->width();
	int imageH = previewImage->height();
	double WH_ratio = imageW / (double)imageH;

	QRect moduleRect = getModuleRect(previewImage);
	QImage moduleImg = previewImage->copy(moduleRect);

	int bigImageW = imageW - 20;
	int bigImageH = imageH - 20;
	QImage bigImg = moduleImg.scaled(bigImageW, bigImageH, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QImage baseImageDst = QImage(previewImage->size(), previewImage->format());
	QImage baseImageDst_mask = baseImageDst.createMaskFromColor(QRgb(0), Qt::MaskInColor);
	baseImageDst.setAlphaChannel(baseImageDst_mask);
	QImage* dstImg = createImageWithOverlay(baseImageDst, bigImg);
	return dstImg;
}

int getLineStart(QImage* Image)
{
	int LineStart = -1;
	int imageW = Image->width();
	int imageH = Image->height();
	QImage imageC = Image->alphaChannel();
	unsigned char* imgCData = imageC.bits();

	for (int i = 0; i < imageH; i++)
	{
		for (int j = 0; j < imageW; j++)
		{
			if (imgCData[i * imageW + j] > 0)
			{
				LineStart = i;
				break;
			}
		}
		if (LineStart > -1) break;
	}

	return LineStart;
}

int getLineEnd(QImage* Image)
{
	int LineEnd = -1;
	int imageW = Image->width();
	int imageH = Image->height();
	QImage imageC = Image->alphaChannel();
	unsigned char* imgCData = imageC.bits();

	for (int i = imageH - 1; i >= 0; i--)
	{
		for (int j = 0; j < imageW; j++)
		{
			if (imgCData[i * imageW + j] > 0)
			{
				LineEnd = i;
				break;
			}
		}
		if (LineEnd > -1) break;
	}

	return LineEnd;
}

void getImageStr(std::string& imageStr, QImage* Image, int layers, QString sPreImgFormat, float layerHeight = 0.)
{
    QString imgSavePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/imgPreview." + sPreImgFormat;
	Image->save(imgSavePath);

	QByteArray cdata = imgSavePath.toLocal8Bit();
	std::string imgSaveStdPath(cdata);

	std::fstream ios(imgSaveStdPath, std::ios::binary | std::ios::in);
	std::string s;
	std::vector<unsigned char> data;
	while (std::getline(ios, s))
	{
		s += "\n";
		int src_size = data.size();
		data.resize(src_size + s.size());
		copy(s.begin(), s.end(), data.begin() + src_size);
	}

	data.pop_back();
	std::vector<std::string> outStr;
	QString imgSize;
	imgSize.sprintf("%d*%d", Image->width(), Image->height());
	QString imgStartEndLine;
	imgStartEndLine.sprintf("%d %d", getLineStart(Image), getLineEnd(Image));
	thumbnail_to_gcode(data, imgSize.toStdString(), sPreImgFormat.toStdString(), imgStartEndLine.toStdString(), layers, outStr, layerHeight);
	for (auto& line : outStr)
	{
		imageStr += line;
		imageStr += "\n";
	}
}

void SliceAttain::saveGCode(const QString& fileName, QImage* previewImage)
{
	std::string imageStr;
	if (previewImage)
	{
		previewImage = resizeModule(previewImage);
		QString sPreImgFormat = GLOBALPARAM("preview_img_type", "jpg");
		if (sPreImgFormat == "bmp")
		{
			cxsw::image2String(*previewImage, 64, 64, true, imageStr);
			cxsw::image2String(*previewImage, 400, 400, false, imageStr);
			//cxsw::writeImageInfo(fileName, *previewImage, 48, 48, sPreImgFormat, m_info.layers);//48
			//cxsw::writeImageInfo(fileName, *previewImage, 300, 300, sPreImgFormat, m_info.layers);//300
		}
		else
		{
			us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();
			QString screen_size = globalsetting->settings().value("screen_size")->value().toString();
			float layer_height = globalsetting->settings().value("layer_height")->value().toFloat();

			if (screen_size == "Sermoon D3")
			{
				QImage minPreImg = previewImage->scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				QImage maxreImg = previewImage->scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				getImageStr(imageStr, &minPreImg, m_info.layers, sPreImgFormat, layer_height);
				getImageStr(imageStr, &maxreImg, m_info.layers, sPreImgFormat, layer_height);
			}
			else if (screen_size == "CR-10 Inspire")
			{
				QImage minPreImg = previewImage->scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				QImage maxreImg = previewImage->scaled(600, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				getImageStr(imageStr, &minPreImg, m_info.layers, sPreImgFormat, layer_height);
				getImageStr(imageStr, &maxreImg, m_info.layers, sPreImgFormat, layer_height);
			}
			else
			{
				QImage minPreImg = previewImage->scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				QImage maxreImg = previewImage->scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				getImageStr(imageStr, &minPreImg, m_info.layers, sPreImgFormat, layer_height);
				getImageStr(imageStr, &maxreImg, m_info.layers, sPreImgFormat, layer_height);
			}

//#if 1
//			/*QImage minPreImg = previewImage->scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//			getImageStr(imageStr, &minPreImg, m_info.layers, sPreImgFormat);
//			getImageStr(imageStr, previewImage, m_info.layers, sPreImgFormat);*/
//#else
//			cxsw::writeImageInfo(fileName, *previewImage, 96, 96, sPreImgFormat, m_info.layers);//48
//			cxsw::writeImageInfo(fileName, *previewImage, 200, 200, sPreImgFormat, m_info.layers);//300
//			imageStr = "";
//#endif
		}
	}

	if (m_info.attainType == AttainType::at_fdm)
		cxsw::cxSaveGCode(fileName, imageStr, m_info.layerCode, m_info.prefixCode, m_info.tailCode, previewImage);
}

void SliceAttain::calculatePrintTimeAndMatial(const std::string& prefixCode)
{
	qDebug() << "cal time and matial";
	std::string gcodeStr = prefixCode;

	std::replace(gcodeStr.begin(), gcodeStr.end(), '\n', ' ');
	std::replace(gcodeStr.begin(), gcodeStr.end(), '\r', ' ');

	std::smatch sm;
	if (std::regex_match(gcodeStr, sm, std::regex(".*TIME:([0-9]{0,8}).*"))) ////get print time
	{
		std::string tStr = sm[1];
		int tmp = atoi(tStr.c_str());
		m_info.printTime = tmp;
	}
	if (std::regex_match(gcodeStr, sm, std::regex(".*Filament used:([0-9]{0,8}\\.[0-9]{0,8}).*"))) ////get print time
	{
		std::string tStr = sm[1];
		float tmp = atof(tStr.c_str());
		m_info.materialLenth = tmp;
	}
}



#include "photo2mesh.h"

#include "data/meshiocenter.h"

#include "enchase/imagematrix.h"
#include "enchase/enchaser.h"
#include "enchase/mapper.h"
#include "enchase/imagematrixfsource.h"
#include <QtGui/QImage>

MatrixF* loadFromImage(QImage* image, int segment)
{
	uint width = (uint)image->width();
	uint height = (uint)image->height();
	if (width == 0 || height == 0)
	{
		printf("load image error\n");
		return nullptr;
	}
	MatrixF* imgMtx = new MatrixF(width, height, 1);
	float* imgData = imgMtx->ptr(0);
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			QRgb qrgb = image->pixel(w, h);
			{
				//imgData[w] = (0.212655 * qRed(qrgb) + 0.715158 * qGreen(qrgb) + 0.072187 * qBlue(qrgb)) / 255.0;
			}
			unsigned char gray = qGray(qrgb);
			if (segment > 0)
			{
				gray = (gray / segment) * segment;
			}

			imgData[w] = (float)gray / 255.0f;
		}
		imgData = imgMtx->ptr(h);
	}
	return imgMtx;
}

MatrixF* loadImage(QString file, int segment)
{
	QImage image(file);
	return loadFromImage(&image, segment);
}

TextureSurface::TextureSurface()
{
}
TextureSurface::~TextureSurface()
{
}
void TextureSurface::setTextureFile(QString& file)
{
	m_file = file;
}
MatrixF* TextureSurface::produce()
{
	return loadImage(m_file, 0);
}

Photo2Mesh::Photo2Mesh() 
	: m_blur(9)
	, m_colorSeg(-1)
	, m_edgeType(0)
	, m_convertType(0)
	, m_baseHeight(0.35f)
	, m_maxHeight(2.2f)
	, m_index(0)
	, m_invert(true)
	, m_meshXWidth(140)
	, m_pointXNum(1000)
	, m_pointYNum(1000)
{

}

Photo2Mesh::~Photo2Mesh()
{
}

int Photo2Mesh::setBlur(int blur)
{
	if (blur < 0)
	{
		blur = 0;
	}
	if (blur > 18)
	{
		blur = 18;
	}
	m_blur = blur;

	return 0;
}

//int Photo2Mesh::setColorSeg(int colorSeg)
//{
//	if (colorSeg > 256)
//	{
//		colorSeg = 256;
//	}
//	if (colorSeg <= 0)
//	{
//		colorSeg = -1;
//	}
//	m_colorSeg = colorSeg;
//	return 0;
//}

void Photo2Mesh::useNormalConvert()
{
	m_convertType = 0;
}

void Photo2Mesh::useTransparencyConvert()
{
//	m_convertType = 1;
}

int Photo2Mesh::setBaseHeight(float baseHeight)
{
	if (baseHeight > 0)
	{
		m_baseHeight = baseHeight;
		return 0;
	}
	return -1;
}

int Photo2Mesh::setMaxHeight(float maxHeight)
{
	if (maxHeight > 0)
	{
		m_maxHeight = maxHeight;
		return 0;
	}
	return -1;
}

void Photo2Mesh::lighterIsHigh()
{
	m_invert = false;
}

void Photo2Mesh::darkerIsHigh()
{
	m_invert = true;
}

void Photo2Mesh::setMeshWidth(float meshX)
{
	m_meshXWidth = meshX;
}

trimesh::TriMesh* Photo2Mesh::generate(QString filename, qtuser_core::Progressor* progressor)
{
	Enchaser enchaser;
	Mapper mapper;
	progressor->progress(0);

	m_planeSurface.setTextureFile(filename);

	m_planeSurface.useBlur = m_blur;
	m_planeSurface.convertType = m_convertType;
	m_planeSurface.baseHeight = m_baseHeight;
	m_planeSurface.maxHeight = m_maxHeight;
	m_planeSurface.invert = m_invert;
	m_planeSurface.edgeType = enchase::EdgeType(m_edgeType);
	m_planeSurface.useIndex = m_index;
	m_planeSurface.colorSegment = m_colorSeg;

	MatrixF* matrix = m_planeSurface.matrix(nullptr);
	progressor->progress(0.9f);

	if (matrix)
	{
		int w = matrix->width();
		int h = matrix->height();
		if (w <= 0 || h <= 0)
		{
			return nullptr;
		}

		float pixel2space = m_meshXWidth / m_pointXNum;

		m_pointYNum = (int)((float)m_pointXNum * (float)h / (float)w);
		enchaser.setSource(enchase::defaultPlane(m_pointXNum, m_pointYNum, pixel2space, m_baseHeight));
		enchase::defaultCoord(m_pointXNum, m_pointYNum, mapper.allTextureGroup());

		progressor->progress(0.95f);

		MatrixFSource* source = new MatrixFSource(matrix);
		mapper.setSource(source);
		enchaser.enchaseCache(&mapper, m_planeSurface.index());

		progressor->progress(1.0f);

		return enchaser.takeCurrent();
	}
	progressor->progress(1.0f);
	return nullptr;
}


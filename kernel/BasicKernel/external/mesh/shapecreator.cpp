#include "shapecreator.h"
#include <QtCore/qmath.h>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QtGui/QQuaternion>
#include <QtGui/QMatrix4x4>
#include <QtGui/QVector2D>
#include <QStandardPaths>

#include "qtuser3d/geometry/boxgeometrydata.h"
#include "fmesh/font/fontoutlinecenter.h"
#include "fmesh/generate/generator.h"
#include "fmesh/contour/path.h"

ShapeCreator::ShapeCreator(QObject* parent)
	:QObject(parent)
{
}
ShapeCreator::~ShapeCreator()
{
}
trimesh::TriMesh* ShapeCreator::createCuboidMesh(QVector3D size)
{
    trimesh::TriMesh* mesh = new trimesh::TriMesh();
    //QFile file(":/support/mesh/cube.stl");
    //if(!file.open(QFile::ReadOnly))
    //{
    //    return NULL;
    //}
    //file.seek(0);
    //char header[80];
    //file.read(header, 80);
	//
    //uint32_t faceCount(0);
    //file.read((char*)&faceCount, sizeof(uint32_t));
	//
    //mesh->faces.reserve(faceCount);
    //mesh->vertices.reserve(3 * faceCount);
    //for (int i = 0; i < faceCount; i++)
    //{
    //    float fbuf[12];
    //    file.read((char*)fbuf, 48);
    //    int v = mesh->vertices.size();
    //    mesh->vertices.push_back(trimesh::point(fbuf[3], fbuf[4], fbuf[5]));
    //    mesh->vertices.push_back(trimesh::point(fbuf[6], fbuf[7], fbuf[8]));
    //    mesh->vertices.push_back(trimesh::point(fbuf[9], fbuf[10], fbuf[11]));
    //    mesh->faces.push_back(trimesh::TriMesh::Face(v, v + 1, v + 2));
    //    unsigned char att[2];
    //    file.read((char*)att, 2);
    //}
	//
    //mesh->need_bbox();
	//
    //file.close();
	mesh->vertices.resize(8);
	mesh->faces.resize(12);
	memcpy(&mesh->vertices.at(0), static_box_position, 24 * sizeof(float));
	memcpy(&mesh->faces.at(0), static_box_triangles_indices, 36 * sizeof(unsigned));

	mesh->need_normals();
	mesh->need_bbox();

    return mesh;

}

trimesh::TriMesh* ShapeCreator::createCuboidMesh(QVector3D bottom, QVector3D top, int r, float radius, float startTheta)
{
	trimesh::TriMesh* mesh = new trimesh::TriMesh();
	QVector3D start = bottom;
	QVector3D end = top;

	QVector3D dir = end - start;
	dir.normalize();
	QQuaternion q = QQuaternion::fromDirection(dir, QVector3D(0.0f, 0.0f, 1.0f));

	float deltaTheta = 2.0f * 3.1415926f / (float)r;
	std::vector<float> cosValue;
	std::vector<float> sinValue;
	for (int i = 0; i < r; ++i)
	{
		cosValue.push_back(qCos(deltaTheta * (float)i + startTheta));
		sinValue.push_back(qSin(deltaTheta * (float)i + startTheta));
	}

	std::vector<QVector3D> baseNormals;
	for (int i = 0; i < r; ++i)
	{
		baseNormals.push_back(QVector3D(cosValue[i], sinValue[i], 0.0f));
	}

	unsigned vertexNum = 2 * r;
	mesh->vertices.resize(vertexNum);
	unsigned triangleNum = 2 * r;
	mesh->faces.resize(triangleNum + 2 * (r - 2));

	unsigned vertexIndex = 0;
	unsigned triangleIndex = 0;
	for (int i = 0; i < r; ++i)
	{
		QVector3D n = q * baseNormals[i];
		QVector3D s = start + n * radius;
		mesh->vertices.at(vertexIndex) = trimesh::vec3(s.x(), s.y(), s.z());
		++vertexIndex;
		QVector3D e = end + n * radius;
		mesh->vertices.at(vertexIndex) = trimesh::vec3(e.x(), e.y(), e.z());
		++vertexIndex;
	}

	for (int i = 0; i < r; ++i)
	{
		trimesh::TriMesh::Face f1(2 * i, ((2 * i + 3) % (2 * r)), 2 * i + 1);
		mesh->faces.at(triangleIndex) = f1;
		++triangleIndex;

		trimesh::TriMesh::Face f2(2 * i, ((2 * i + 2) % (2 * r)), ((2 * i + 3) % (2 * r)));
		mesh->faces.at(triangleIndex) = f2;
		++triangleIndex;
	}

	for (int i = 1; i < r - 1; ++i)
	{
		trimesh::TriMesh::Face f1(0, 2 * (i + 1), 2 * i);
		mesh->faces.at(triangleIndex) = f1;
		++triangleIndex;
	}

	for (int i = 1; i < r - 1; ++i)
	{
		trimesh::TriMesh::Face f1(1, 2 * i + 1, 2 * (i + 1) + 1);
		mesh->faces.at(triangleIndex) = f1;
		++triangleIndex;
	}

	return mesh;
}

trimesh::TriMesh* ShapeCreator::createCuboidMesh(std::vector<mmesh::VerticalSeg>& segments, std::vector<int>& chunks, QMatrix4x4& matrix, int nn, float radius, float startTheta)
{
	trimesh::TriMesh* mesh = new trimesh::TriMesh();

	QMatrix4x4 imatrix = matrix.inverted();

	unsigned r = nn;
	float deltaTheta = 2.0f * 3.1415926f / (float)r;
	std::vector<float> cosValue;
	std::vector<float> sinValue;
	for (unsigned i = 0; i < r; ++i)
	{
		cosValue.push_back(qCos(deltaTheta * (float)i + startTheta));
		sinValue.push_back(qSin(deltaTheta * (float)i + startTheta));
	}

	std::vector<QVector3D> baseNormals;
	for (unsigned i = 0; i < r; ++i)
	{
		baseNormals.push_back(QVector3D(cosValue[i], sinValue[i], 0.0f));
	}

	int count = (int)segments.size();
	unsigned vertexNum = count * 2 * r;
	mesh->vertices.resize(vertexNum);
	int chunkSize = (2 * r + 2 * (r - 2));
	unsigned triangleNum = count * chunkSize;
	mesh->faces.resize(triangleNum);
	chunks.resize(count);
	for (int i = 0; i < count; ++i)
	{
		chunks.at(i) = chunkSize * i;
	}

	unsigned vertexIndex = 0;
	unsigned triangleIndex = 0;

	for (mmesh::VerticalSeg& verticalSeg : segments)
	{
		unsigned startVertexIndex = vertexIndex;
		unsigned startTriangleIndex = triangleIndex;

		QVector3D start = imatrix * QVector3D(verticalSeg.b.x, verticalSeg.b.y, verticalSeg.b.z);
		QVector3D end = imatrix * QVector3D(verticalSeg.t.x, verticalSeg.t.y, verticalSeg.t.z);

		QVector3D dir = end - start;
		dir.normalize();
		QQuaternion q = QQuaternion::fromDirection(dir, QVector3D(0.0f, 0.0f, 1.0f));
		for (unsigned i = 0; i < r; ++i)
		{
			QVector3D n = q * baseNormals[i];
			QVector3D s = start + n * radius;

			trimesh::vec3& v1 = mesh->vertices.at(startVertexIndex);
			v1[0] = s.x();
			v1[1] = s.y();
			v1[2] = s.z();

			++startVertexIndex;
			QVector3D e = end + n * radius;

			trimesh::vec3& v2 = mesh->vertices.at(startVertexIndex);
			v2[0] = e.x();
			v2[1] = e.y();
			v2[2] = e.z();

			++startVertexIndex;
		}
		unsigned triIndex = startTriangleIndex;
		for (unsigned i = 0; i < r; ++i)
		{
			trimesh::TriMesh::Face& f1 = mesh->faces.at(triIndex++);
			f1.at(0) = vertexIndex + 2 * i;
			f1.at(2) = vertexIndex + 2 * i + 1;
			f1.at(1) = vertexIndex + ((2 * i + 3) % (2 * r));
			trimesh::TriMesh::Face& f2 = mesh->faces.at(triIndex++);
			f2.at(0) = vertexIndex + 2 * i;
			f2.at(2) = vertexIndex + ((2 * i + 3) % (2 * r));
			f2.at(1) = vertexIndex + ((2 * i + 2) % (2 * r));
		}
		for (int i = 1; i < r - 1; ++i)
		{
			trimesh::TriMesh::Face f1(vertexIndex, vertexIndex + 2 * (i + 1), vertexIndex + 2 * i);
			mesh->faces.at(triIndex) = f1;
			++triIndex;
		}

		for (int i = 1; i < r - 1; ++i)
		{
			trimesh::TriMesh::Face f1(vertexIndex + 1, vertexIndex + 2 * i + 1, vertexIndex + 2 * (i + 1) + 1);
			mesh->faces.at(triIndex) = f1;
			++triIndex;
		}

		vertexIndex += 2 * r;
		triangleIndex += chunkSize;
	}

	return mesh;
}

trimesh::TriMesh* ShapeCreator::createCylinderMesh(QVector3D top, QVector3D bottom, float radius, int num, float theta)
{
	trimesh::TriMesh* mesh = new trimesh::TriMesh();

	int hPart = num;

	QVector3D start = bottom;
	QVector3D end = top;

	QVector3D dir = end - start;
	dir.normalize();
	QQuaternion q = QQuaternion::fromDirection(dir, QVector3D(0.0f, 0.0f, 1.0f));

	theta *= M_PIf / 180.0f;
	float deltaTheta = M_PIf * 2.0f / (float)(hPart);
	std::vector<float> cosValue;
	std::vector<float> sinValue;
	for (int i = 0; i < hPart; ++i)
	{
		cosValue.push_back(qCos(deltaTheta * (float)i + theta));
		sinValue.push_back(qSin(deltaTheta * (float)i + theta));
	}

	std::vector<QVector3D> baseNormals;
	for (int i = 0; i < hPart; ++i)
	{
		baseNormals.push_back(QVector3D(cosValue[i], sinValue[i], 0.0f));
	}

	int vertexNum = 2 * hPart;
	std::vector<trimesh::vec3> points(vertexNum);
	int faceNum = 4 * hPart - 4;
	mesh->faces.resize(faceNum);

	int vertexIndex = 0;
	for (int i = 0; i < hPart; ++i)
	{
		QVector3D n = q * baseNormals[i];
		QVector3D s = start + n * radius;
		points.at(vertexIndex) = trimesh::vec3(s.x(), s.y(), s.z());
		++vertexIndex;
		QVector3D e = end + n * radius;
		points.at(vertexIndex) = trimesh::vec3(e.x(), e.y(), e.z());
		++vertexIndex;
	}
	mesh->vertices.swap(points);

	auto fvindex = [&hPart](int layer, int index)->int {
		return layer + 2 * index;
	};

	int faceIndex = 0;
	for (int i = 0; i < hPart; ++i)
	{
		int v1 = fvindex(0, i);
		int v2 = fvindex(1, i);
		int v3 = fvindex(0, (i + 1) % hPart);
		int v4 = fvindex(1, (i + 1) % hPart);

		trimesh::TriMesh::Face& f1 = mesh->faces.at(faceIndex);
		f1[0] = v1;
		f1[1] = v3;
		f1[2] = v2;
		++faceIndex;
		trimesh::TriMesh::Face& f2 = mesh->faces.at(faceIndex);
		f2[0] = v2;
		f2[1] = v3;
		f2[2] = v4;
		++faceIndex;
	}

	for (int i = 1; i < hPart - 1; ++i)
	{
		trimesh::TriMesh::Face& f1 = mesh->faces.at(faceIndex);
		f1[0] = 0;
		f1[1] = fvindex(0, i + 1);
		f1[2] = fvindex(0, i);
		++faceIndex;
	}

	for (int i = 1; i < hPart - 1; ++i)
	{
		trimesh::TriMesh::Face& f1 = mesh->faces.at(faceIndex);
		f1[0] = 1;
		f1[1] = fvindex(1, i);
		f1[2] = fvindex(1, i + 1);
		++faceIndex;
	}

	return mesh;
}

trimesh::TriMesh* ShapeCreator::createPyramidMesh(int nbBottomVertex, float bottomRadius, float height)
{
	if (nbBottomVertex < 3 || bottomRadius < 0.0f || height < 0.0f)
	{
		return nullptr;
	}

	trimesh::TriMesh* pyramid = new trimesh::TriMesh();

	pyramid->vertices.emplace_back(0.0f, 0.0f, 0.0f);
	float theta = 0.0f;
	for (int i = 0; i < nbBottomVertex; i++)
	{
		pyramid->vertices.emplace_back(cosf(theta) * bottomRadius, sinf(theta) * bottomRadius, 0.0f);
		theta += M_2PI / nbBottomVertex;
	}
	pyramid->vertices.emplace_back(0.0f, 0.0f, height);

	int next = 0;
	for (int i = 1; i <= nbBottomVertex; i++)
	{
		next = i + 1;
		if (next > nbBottomVertex)
		{
			next = next % nbBottomVertex;
		}
		pyramid->faces.emplace_back(0, next, i);
		pyramid->faces.emplace_back(nbBottomVertex + 1, i, next);
	}

	return pyramid;
}

void ShapeCreator::createTextMesh(QString text, QString font, float height, float thickness,
	std::vector<float>& widths, std::vector<trimesh::TriMesh*>& meshs)
{
	fmesh::FontOutlineCenter outline;

	QByteArray cdata = QCoreApplication::applicationDirPath().toLocal8Bit();
	std::string applicationDirPath(cdata);
	outline.addSearchPath(applicationDirPath);

	cdata = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toLocal8Bit();
	std::string appDataLocationPaht(cdata);
	outline.addSearchPath(appDataLocationPaht);


	int index = QCoreApplication::applicationDirPath().lastIndexOf("/");
	cdata = QCoreApplication::applicationDirPath().left(index).toLocal8Bit();
	std::string appResourcePaht(cdata);
	outline.addSearchPath(appResourcePaht +"/Resources");

	fmesh::GeneratorProxy proxy;
	fmesh::ADParam param;
	ClipperLibXYZ::IntPoint min, max;

	// text mesh parameters
	param.extend_width = height * 0.01;
	param.total_height = thickness;
	param.bottom_offset = 0.0f;

	param.top_type = fmesh::ADTopType::adtt_close;
	param.top_height = thickness * 0.25;
	param.top_extend_width = height * 0.01;

	param.bottom_type = fmesh::ADBottomType::adbt_close;
	param.bottom_height = thickness * 0.25;
	param.bottom_extend_width = height * 0.01;

	param.shape_type = fmesh::ADShapeType::adst_none;
	param.shape_bottom_height = 0.0f;
	param.shape_top_height = 0.0f;
	param.shape_middle_width = 0.0f;
	param.shape_angle = 0.0f;
	
	for (QChar chr : text.trimmed())
	{
		if (iswspace(chr.unicode()))
		{
			continue;
		}

		ClipperLibXYZ::Paths* cur_paths = outline.getPath(/*font.toStdString()*/std::string((const char*)font.toLocal8Bit()), chr.unicode(), height);

		if (cur_paths)
		{
			trimesh::TriMesh* cur_mesh = proxy.build(param, cur_paths, nullptr, nullptr, nullptr, true);
			if (cur_mesh)
			{
				fmesh::calculatePathBox(cur_paths, min, max);
				widths.push_back((max.X - min.X) / 1000.0f);
				meshs.push_back(cur_mesh);
			}
		}
	}
}

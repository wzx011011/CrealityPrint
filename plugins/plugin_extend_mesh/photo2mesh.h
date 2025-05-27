#ifndef _NULLSPACE_PHOTO2MESH_1589849922902_H
#define _NULLSPACE_PHOTO2MESH_1589849922902_H

#include "enchase/default.h"
#include "enchase/surface.h"
#include "qtusercore/module/progressor.h"

using namespace enchase;

class TextureSurface : public Surface
{
public:
	TextureSurface();
	virtual ~TextureSurface();
	void setTextureFile(QString& file);
protected:
	MatrixF* produce() override;
protected:
	QString m_file;
};

class Photo2Mesh
{
public:
	Photo2Mesh();
	~Photo2Mesh();

	int setBlur(int blur);
//	int setColorSeg(int colorSeg);

	void useNormalConvert();
	void useTransparencyConvert();

	int setBaseHeight(float baseHeight);
	int setMaxHeight(float maxHeight);

	void lighterIsHigh();
	void darkerIsHigh();

	void setMeshWidth(float meshX);

	trimesh::TriMesh* generate(QString filename, qtuser_core::Progressor* progressor);

protected:
	int m_blur;
	int m_colorSeg;
	int m_edgeType;
	int m_convertType;
	
	float m_baseHeight;
	float m_maxHeight;

	int m_index;

	bool m_invert;

	float m_meshXWidth;

protected:
	int m_pointXNum;
	int m_pointYNum;

protected:
	TextureSurface m_planeSurface;


};
#endif // _NULLSPACE_PHOTO2MESH_1589849922902_H

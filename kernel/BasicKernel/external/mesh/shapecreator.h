#ifndef _NULLSPACE_CYLINDERCREATOR_1590476543843_H
#define _NULLSPACE_CYLINDERCREATOR_1590476543843_H
#include "basickernelexport.h"
#include <QtCore/QObject>
#include <QtGui/QVector3D>
#include "trimesh2/TriMesh.h"
#include "mmesh/trimesh/box2dgrid.h"

class BASIC_KERNEL_API ShapeCreator: public QObject
{
public:
    ShapeCreator(QObject* parent = nullptr);
    virtual ~ShapeCreator();

    static trimesh::TriMesh* createCylinderMesh(QVector3D top, QVector3D bottom, float radius, int num = 20, float theta = 0.0f);
    static trimesh::TriMesh* createCuboidMesh(QVector3D size);
    static trimesh::TriMesh* createCuboidMesh(QVector3D bottom, QVector3D top, int n, float radius, float startTheta = 0.7853981634f);

    static trimesh::TriMesh* createCuboidMesh(std::vector<mmesh::VerticalSeg>& segments, std::vector<int>& chunks, QMatrix4x4& matrix, int n, float radius, float startTheta = 0.7853981634f);

    /* create a pyramid */
    static trimesh::TriMesh* createPyramidMesh(int nbBottomVertex, float bottomRadius, float height);

    /* coordinates of the center: (0, 0, 0) */
    static void createTextMesh(QString text, QString font, float height, float thickness,
        std::vector<float>& widths, std::vector<trimesh::TriMesh*>& meshs);

protected:
};
#endif // _NULLSPACE_CYLINDERCREATOR_1590476543843_H

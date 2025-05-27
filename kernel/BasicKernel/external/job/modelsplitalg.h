#ifndef _NULLSPACE_CModelSplitAlg_1589765331924_H
#define _NULLSPACE_CModelSplitAlg_1589765331924_H
#include "basickernelexport.h"
#include <QtGui/QVector3D>
#include <QtGui/QVector2D>
#include <QtCore/QVector>
#include <QRect>
#include <vector> 
#include "qtusercore/module/progressor.h"
#include "trimesh2/TriMesh.h"
using namespace std;

//one struct that save the point
struct S3DPointF
{
	float fX;
	float fY;
	float fZ;
	S3DPointF()
	{
		fX = 0;
		fY = 0;
		fZ = 0;
	}
	~S3DPointF()
	{
		fX = 0;
		fY = 0;
		fZ = 0;
	}
};


//one struct that save the triangle and the information of connecting with other triangle
struct SFaceInfor
{
	SFaceInfor()
	{
		iMark = 0;
	}
	~SFaceInfor()
	{
		iMark = 0;
	}
	S3DPointF pt[3];
	//0000 0001: it means that the first side connects with another face side, the first side is from the first point to the second point
	//0000 0010: it means that the second side connects with another face side, the second side is from the second point to the third point
	//0000 0100: it means that the third side connects with another face side, the third side is from the third point to the first point
	//this variable value is 0 and this means that this triangle doesn't have any side connect with another face. this value is 7(0000 0111), it means that 
	int iMark;
};


struct SSubModelMesh
{
	//this vector that save all the triangle that its three sides connect with other triangle
	vector<SFaceInfor> vt3SidesConn;
	//this vector that save all the triangle and not all its three sides connect with other triangle.
	vector<SFaceInfor> vtNot3SidesConn;
};


class BASIC_KERNEL_API CModelSplitAlg
{
public:
	CModelSplitAlg();
	virtual ~CModelSplitAlg();

public:
	/*******************************************************************
describe:
for spliting one model, at the beginning, we need to pick the first triangle mesh face. because there are about one million triangle mesh faces in one model. if I put the first triangle mesh face into the cycle, every loop in this cycle, it need to check the triangle face is the first one or not. it will waste the time. depands on this, I put the first triangle mesh face on the upon the cycle.
in the cycle, we need check this triangle mesh is valid or not. if there are two vertexes position are the same in one triangle, we thought this triangle face is invalid and I will give it up. after this, I will get one triangle face in the list that I put all the triangle mesh after they are checked. when I get one triangle mesh, I use the source triangle mesh to check with this destination tirangle mesh.
if the destination triangle mesh side not connect with other triangle, I will use these sides to compare with the source triangle mesh sides. when there is one one side connect between these two triangle mesh, I think these two triangle mesh connect and I will not check other sides of these two triangles. because these two triangles can't have two sides connect with each other. you can tink it more deeply. if there are two sides to connect, it means these three vertexes are the same position and i can prove that these two triangles are the same.
after this, we will go to check next the triangle mesh


parameter:
meshCreality: the model that will be split
vtSplitModel: list that save all the sub model triangle mesh

return:


time: monday, september second day 2020
******************************************************************/
	int SplitModel(trimesh::TriMesh& meshCreality, vector<SSubModelMesh>& vtSplitModel, qtuser_core::Progressor* progressor);
	

	/*******************************************************************
describe:
translate vtSplitModel triangle mesh data format to the creality software triangle mesh data format.


parameter:
meshCreality: the model that will be split
vtSplitModel: list that save one sub model triangle mesh

return:


time: monday, september second day 2020
******************************************************************/
	bool TranslateMeshToCreality(trimesh::TriMesh& meshCreality, SSubModelMesh& vtSplitModel);

	/*******************************************************************
describe:
get the triangle mesh data from srcmesh and merge them into the dstMesh. in this function, it calculate the new box of this new model.


parameter:
dstMesh: dstMesh and SrcMesh triangle mesh data will be saved into this store.
srcMesh: this triangle mesh data will be saved into dstMesh store.
globalMatrix: the global matrix of srcMesh. this matrix include scale, move and rotate.

return:


time: monday, september second day 2020
******************************************************************/
	bool ConnectSubmodel(trimesh::TriMesh& dstMesh, trimesh::TriMesh& srcMesh, const QMatrix4x4& globalMatrix);


	/*******************************************************************
describe:
the triangle mesh data will be moved to the original point of this coordination system and the center point of this model box will be the original point


parameter:
dstMesh: the triangle mesh data will be moved to the original point of this coordination system and the center point of this model box will be the original point
localPos: the vector that move the triangle to the original point

return:


time: monday, september third day 2020
******************************************************************/
	static bool MoveToOriginal(trimesh::TriMesh* dstMesh, QVector3D& localPos);


	bool TranslateToGlobalPos(trimesh::TriMesh& dstMesh, trimesh::TriMesh& srcMesh, const QMatrix4x4& globalMatrix);


private:
	//this function explain in cpp source code file
	bool CombineToCurrent(SSubModelMesh& CursubModel, SSubModelMesh& PresubModel);

	//this function explain in cpp source code file
	int IsConnect(SFaceInfor& dst, SFaceInfor& src);

	//this function explain in cpp source code file
	bool IsSamePoint(const S3DPointF& dst, const S3DPointF& src);

	//this function explain in cpp source code file
	bool IsSamePointFloat(const S3DPointF& dst, const S3DPointF& src);

	//this function explain in cpp source code file
	bool IsSameSegmentLine(const S3DPointF& dstStart, const S3DPointF& dstEnd, const S3DPointF& srcStart, const S3DPointF& srcEnd);

	//this function explain in cpp source code file
	bool AnalyzeTriangleConnectionInfor(int* piConnectIndex, vector<SSubModelMesh>& vtSplitModel, SFaceInfor& fSrcInfor);

	//this function explain in cpp source code file
	bool IsValidTriangle(const SFaceInfor& fInfor);
};

#endif // _NULLSPACE_CLayoutAlg_1589765331924_H

#include "modelsplitalg.h"
#include "qtuser3d/math/space3d.h"
#include "math.h"
#include <QTime>

using namespace std;
#define GAPS_VALUE 1
#define FLOAT_PRECISION  0.01


CModelSplitAlg::CModelSplitAlg()
{

}

CModelSplitAlg::~CModelSplitAlg()
{
}



bool CModelSplitAlg::TranslateMeshToCreality(trimesh::TriMesh& meshCreality, SSubModelMesh& vtSplitModel)
{
	//ReadMeshVCGFromFile("E:/project/test/meshLab_below", meshVCG);

	int faceCount = vtSplitModel.vt3SidesConn.size() + vtSplitModel.vtNot3SidesConn.size();


	meshCreality.faces.clear();
	meshCreality.vertices.clear();
	meshCreality.faces.reserve(faceCount);
	meshCreality.vertices.reserve(3 * faceCount);
	for (int i = 0; i < vtSplitModel.vt3SidesConn.size(); i++)
	{
		int v = meshCreality.vertices.size();
		for (int k = 0; k < 3; k++)
		{
			meshCreality.vertices.push_back(trimesh::point(vtSplitModel.vt3SidesConn[i].pt[k].fX, vtSplitModel.vt3SidesConn[i].pt[k].fY, vtSplitModel.vt3SidesConn[i].pt[k].fZ));
		}
		meshCreality.faces.push_back(trimesh::TriMesh::Face(v, v + 1, v + 2));
	}

	for (int i = 0; i < vtSplitModel.vtNot3SidesConn.size(); i++)
	{
		int v = meshCreality.vertices.size();
		for (int k = 0; k < 3; k++)
		{
			meshCreality.vertices.push_back(trimesh::point(vtSplitModel.vtNot3SidesConn[i].pt[k].fX, vtSplitModel.vtNot3SidesConn[i].pt[k].fY, vtSplitModel.vtNot3SidesConn[i].pt[k].fZ));
		}
		meshCreality.faces.push_back(trimesh::TriMesh::Face(v, v + 1, v + 2));
	}

	return true;
}


/*******************************************************************
describe:
check these two triangles mesh is valid or not. the algorithm is that we check there are two vertexes' point is the same or not. if there are two vertexes' position is the same, it means this triangle is invalid.


parameter:
fInfor: check this triangle is valid or not
return:


time: monday, september third day 2020
******************************************************************/
bool CModelSplitAlg::IsValidTriangle(const SFaceInfor& fInfor)
{
	if (IsSamePointFloat(fInfor.pt[0], fInfor.pt[1]))
	{
		return false;
	}
	else if (IsSamePointFloat(fInfor.pt[2], fInfor.pt[1]))
	{
		return false;
	}
	else if (IsSamePointFloat(fInfor.pt[0], fInfor.pt[2]))
	{
		return false;
	}

	return true;
}



bool CModelSplitAlg::TranslateToGlobalPos(trimesh::TriMesh& dstMesh, trimesh::TriMesh& srcMesh, const QMatrix4x4& globalMatrix)
{
	int iVertexes = srcMesh.vertices.size();

	for (int i = 0; i < iVertexes; i++)
	{
		QVector4D qPoint(srcMesh.vertices[i][0], srcMesh.vertices[i][1], srcMesh.vertices[i][2], 1);

		qPoint = globalMatrix * qPoint;

		srcMesh.vertices[i][0] = qPoint.x();
		srcMesh.vertices[i][1] = qPoint.y();
		srcMesh.vertices[i][2] = qPoint.z();


		if (dstMesh.bbox.min.x > qPoint.x()) dstMesh.bbox.min.x = qPoint.x();
		if (dstMesh.bbox.min.y > qPoint.y()) dstMesh.bbox.min.y = qPoint.y();
		if (dstMesh.bbox.min.z > qPoint.z()) dstMesh.bbox.min.z = qPoint.z();

		if (dstMesh.bbox.max.x < qPoint.x()) dstMesh.bbox.max.x = qPoint.x();
		if (dstMesh.bbox.max.y < qPoint.y()) dstMesh.bbox.max.y = qPoint.y();
		if (dstMesh.bbox.max.z < qPoint.z()) dstMesh.bbox.max.z = qPoint.z();
	}


	return true;
}


bool CModelSplitAlg::ConnectSubmodel(trimesh::TriMesh& dstMesh, trimesh::TriMesh& srcMesh, const QMatrix4x4& globalMatrix)
{
	int iFacenum = srcMesh.faces.size();

	for (int i = 0; i < iFacenum; i++)
	{
		int v = dstMesh.vertices.size();
		for (int k = 0; k < 3; k++)
		{
			QVector4D qPoint(srcMesh.vertices[srcMesh.faces[i][k]][0], srcMesh.vertices[srcMesh.faces[i][k]][1], srcMesh.vertices[srcMesh.faces[i][k]][2], 1);

			qPoint = globalMatrix * qPoint;

			dstMesh.vertices.push_back(trimesh::point(qPoint.x(), qPoint.y(), qPoint.z()));



			if (dstMesh.bbox.min.x > qPoint.x()) dstMesh.bbox.min.x = qPoint.x();
			if (dstMesh.bbox.min.y > qPoint.y()) dstMesh.bbox.min.y = qPoint.y();
			if (dstMesh.bbox.min.z > qPoint.z()) dstMesh.bbox.min.z = qPoint.z();

			if (dstMesh.bbox.max.x < qPoint.x()) dstMesh.bbox.max.x = qPoint.x();
			if (dstMesh.bbox.max.y < qPoint.y()) dstMesh.bbox.max.y = qPoint.y();
			if (dstMesh.bbox.max.z < qPoint.z()) dstMesh.bbox.max.z = qPoint.z();
		}


		dstMesh.faces.push_back(trimesh::TriMesh::Face(v, v + 1, v + 2));
	}


	return true;
}


/*******************************************************************
describe:
the triangle mesh data will be moved to the original point of this coordination system and the center point of this model box will be the original point


parameter:
dstMesh: the triangle mesh data will be moved to the original point of this coordination system and the center point of this model box will be the original point
localPos: the vector that move the triangle to the original point

return:


time: monday, september third day 2020
******************************************************************/
bool CModelSplitAlg::MoveToOriginal(trimesh::TriMesh* dstMesh, QVector3D& localPos)
{
	int iVertexnum = dstMesh->vertices.size();

	localPos.setX((dstMesh->bbox.max.x + dstMesh->bbox.min.x) / 2);
	localPos.setY((dstMesh->bbox.max.y + dstMesh->bbox.min.y) / 2);
	localPos.setZ(dstMesh->bbox.min.z / 2);

	for (int i = 0; i < iVertexnum; i++)
	{
		dstMesh->vertices[i][0] -= localPos.x();
		dstMesh->vertices[i][1] -= localPos.y();
		dstMesh->vertices[i][2] -= localPos.z();
	}
	dstMesh->clear_bbox();
	dstMesh->need_bbox();
	return true;
}


int CModelSplitAlg::SplitModel(trimesh::TriMesh& meshCreality, vector<SSubModelMesh>& vtSplitModel, qtuser_core::Progressor* progressor)
{
	int iFacenum = meshCreality.faces.size();

	//for the first triangle face, I just need to record it. If I put this first triangle face recording into the cycle and it will
	int iFirstIndex = 0;
	for (; iFirstIndex < iFacenum; ++iFirstIndex)
	{
		SFaceInfor fInfor;
		for (int k = 0; k < 3; ++k)
		{
			fInfor.pt[k].fX = meshCreality.vertices[meshCreality.faces[iFirstIndex][k]][0];
			fInfor.pt[k].fY = meshCreality.vertices[meshCreality.faces[iFirstIndex][k]][1];
			fInfor.pt[k].fZ = meshCreality.vertices[meshCreality.faces[iFirstIndex][k]][2];
		}

		if (IsValidTriangle(fInfor))
		{
			SSubModelMesh vtFaceInfor;
			vtFaceInfor.vtNot3SidesConn.push_back(fInfor);
			vtSplitModel.push_back(vtFaceInfor);

			break;
		}
	}


	// For each triangle read the normal, the three coords and a short set to zero
	for (int iF = iFirstIndex; iF < iFacenum; ++iF)
	{
		int iSubModel = vtSplitModel.size();

		SFaceInfor fSrcInfor;
		for (int k = 0; k < 3; ++k)
		{
			fSrcInfor.pt[k].fX = meshCreality.vertices[meshCreality.faces[iF][k]][0];
			fSrcInfor.pt[k].fY = meshCreality.vertices[meshCreality.faces[iF][k]][1];
			fSrcInfor.pt[k].fZ = meshCreality.vertices[meshCreality.faces[iF][k]][2];
		}

		if (!IsValidTriangle(fSrcInfor))
		{
			continue;
		}

		int iConnectIndex[3];
		iConnectIndex[0] = -1;	//the first segment line of the triangle that connect with sub modal
		iConnectIndex[1] = -1;	//the second segment line of the triangle that connect with sub modal
		iConnectIndex[2] = -1;	//the third segment line of the triangle that connect with sub modal
		for (size_t i = 0; i < iSubModel; i++)
		{
			SSubModelMesh& subModel = vtSplitModel[i];
			for (int j = 0; j < subModel.vtNot3SidesConn.size(); j++)
			{
				SFaceInfor& fDstInfor = subModel.vtNot3SidesConn[j];

				int iIndexConnect = IsConnect(fDstInfor, fSrcInfor);
				if (iIndexConnect >= 0 && iIndexConnect < 3)
				{
					iConnectIndex[iIndexConnect] = i;

					//for the destination triangle mesh, we need to check its iMask sign value. if this value is 7(0000 0111), it means that its three sides connect with triangle mesh and this destination triangle mesh doesn't need to check again. I will put it into another list. this list doesn't need to compare with the source triangle.
					//use this mechanism, it will reduce a lot of the time to compare the source triangle mesh with the invalid destination triangle mesh. for 1 million triangle meshs model, when it is collecting, it will have a lot of the triangle mesh not to compare after its three sides connected with other triangle mesh.
					//yaoyaping
					if (7 == fDstInfor.iMark)
					{
						subModel.vt3SidesConn.push_back(subModel.vtNot3SidesConn[j]);

						vector<SFaceInfor>::iterator it = subModel.vtNot3SidesConn.begin();
						subModel.vtNot3SidesConn.erase(it + j);
						j--;
					}
				}
			}
		}

		AnalyzeTriangleConnectionInfor(iConnectIndex, vtSplitModel, fSrcInfor);

		if (progressor)
		{
			float fPro = ((float)iF) / iFacenum;
			progressor->progress(fPro);

			if (progressor->interrupt())
			{
				return 0;
			}
		}
	}


	return 1;
}


/*******************************************************************
describe:
for analyzing the result of the triangle connection information, I divide this into two parts. the first part analyze the first and second segment line connection information and the second part analyze the second and the third segment line connection. in the first part, I check the second segment line has the connection or not. if it doens't connect with any other triangle mesh, I will check the first segment line. if the first segment line has the connection. I will assign the connection triangle mesh number into the second segment line. it will help to me to analyze this value with the third segment line in the second cycle.
if the second line segment connect with another triangle mesh, we need to check the first line segment connect with the triangle mesh or not. if the first line segment connect with the another triangle, we need to check this triangle is the same with the triangle that connect with the second line segment. if them are different, we can combine them together. in the conbining, we need to check combine which one into another which one. for example, if combine A into B traignle mesh list, we need to remember the B triangle mesh list number. and we need to delete the A triangle mesh list. however, we delete one item in the vector and it will affect the index of the vector.
if the B index is large than A, the B index needs to minus one. because A is delete now and we will lose one item at the front of the B. at last, if B is the first line segment, we need to assign the B index into the second segment line variable, because we will use this varaible value in the next cicle. after we finsih this first cicle, we will get into the second cicle. in this algorithm, the first cicle and the second cicle logic is the same.

parameter:
piConnectIndex: it has three items and they save all the index of the sub model
vtSplitModel: the list that save all the sub modal triangle mesh data
fSrcInfor: this triangle is checking to connect with another triangle or not.

return:


time: monday, September, third day 2020
******************************************************************/
bool CModelSplitAlg::AnalyzeTriangleConnectionInfor(int* piConnectIndex, vector<SSubModelMesh>& vtSplitModel, SFaceInfor& fSrcInfor)
{
	//this varialbe will be used to record which submodel index that the new triangle will be inserted. this triangle is used to compare with every sub model mesh.
	int iIndexCombine = -1;
	for (int i = 1; i < 3; i++)
	{
		if (piConnectIndex[i] >= 0)
		{
			if (piConnectIndex[i - 1] >= 0)
			{
				if (piConnectIndex[i] != piConnectIndex[i - 1])
				{
					SSubModelMesh& curSubModel = vtSplitModel[piConnectIndex[i]];
					SSubModelMesh& preSubModel = vtSplitModel[piConnectIndex[i - 1]];
					if (CombineToCurrent(curSubModel, preSubModel))
					{
						//the previous sub model triangle mesh data is copied into the current sub model triangle mesh data
						vector<SSubModelMesh>::iterator it = vtSplitModel.begin();

						vtSplitModel.erase(it + piConnectIndex[i - 1]);

						//yaoyaping: because it erase the previous item, so we need to check this item is front of the current item.
						if (piConnectIndex[i - 1] < piConnectIndex[i])
						{
							piConnectIndex[i] -= 1;
						}

						iIndexCombine = piConnectIndex[i];

						if (i < 2)
						{
							if (piConnectIndex[i + 1] > piConnectIndex[i - 1])
							{
								piConnectIndex[i + 1] -= 1;
							}
						}
					}
					else
					{
						//the previous sub model triangle mesh data is copied into the current sub model triangle mesh data
						vector<SSubModelMesh>::iterator it = vtSplitModel.begin();

						vtSplitModel.erase(it + piConnectIndex[i]);

						if (piConnectIndex[i - 1] > piConnectIndex[i])
						{
							piConnectIndex[i - 1] -= 1;
						}

						iIndexCombine = piConnectIndex[i - 1];
						piConnectIndex[i] = piConnectIndex[i - 1];


						if (i < 2)
						{
							if (piConnectIndex[i + 1] > piConnectIndex[i])
							{
								piConnectIndex[i + 1] -= 1;
							}
						}
					}


					if (piConnectIndex[i + 1] == piConnectIndex[i - 1] || piConnectIndex[i + 1] == piConnectIndex[i])
					{
						break;
					}
				}
				else
				{
					iIndexCombine = piConnectIndex[i];
				}
			}
			else
			{
				iIndexCombine = piConnectIndex[i];
			}
		}
		else
		{
			if (piConnectIndex[i - 1] >= 0)
			{
				piConnectIndex[i] = piConnectIndex[i - 1];
				iIndexCombine = piConnectIndex[i];
			}
		}
	}

	if (iIndexCombine >= 0)
	{
		SSubModelMesh& curSubModel = vtSplitModel[iIndexCombine];

		if (7 == fSrcInfor.iMark)
		{
			curSubModel.vt3SidesConn.push_back(fSrcInfor);
		}
		else
		{
			curSubModel.vtNot3SidesConn.push_back(fSrcInfor);
		}
	}
	else
	{
		SSubModelMesh vtFaceInfor;
		vtFaceInfor.vtNot3SidesConn.push_back(fSrcInfor);
		vtSplitModel.push_back(vtFaceInfor);
	}

	return true;
}


/*******************************************************************
describe:
at the beginning, we compare the count of these two sub model triangle mesh. which count is large, we will copy the small count one into the large one and it will reduce the CPU time.


parameter:
CursubModel: the current sub model triangle mesh data
PresubModel: the previous sub model triangle mesh data

return:


time: monday, september third day 2020
******************************************************************/
bool CModelSplitAlg::CombineToCurrent(SSubModelMesh& CursubModel, SSubModelMesh& PresubModel)
{
	//check which sub model mesh count is more and then copy the less mesh into the large mesh count sub model list, it will reduce the CPU time
	int iPreItemCount = PresubModel.vt3SidesConn.size() + PresubModel.vtNot3SidesConn.size();
	int iCurItemCount = CursubModel.vt3SidesConn.size() + CursubModel.vtNot3SidesConn.size();
	if (iPreItemCount > iCurItemCount)
	{
		for (int i = 0; i < CursubModel.vt3SidesConn.size(); i++)
		{
			PresubModel.vt3SidesConn.push_back(CursubModel.vt3SidesConn[i]);
		}
		for (int i = 0; i < CursubModel.vtNot3SidesConn.size(); i++)
		{
			PresubModel.vtNot3SidesConn.push_back(CursubModel.vtNot3SidesConn[i]);
		}

		return false;
	}
	else
	{
		for (int i = 0; i < PresubModel.vt3SidesConn.size(); i++)
		{
			CursubModel.vt3SidesConn.push_back(PresubModel.vt3SidesConn[i]);
		}
		for (int i = 0; i < PresubModel.vtNot3SidesConn.size(); i++)
		{
			CursubModel.vtNot3SidesConn.push_back(PresubModel.vtNot3SidesConn[i]);
		}

		return true;
	}

	return true;
}

/*******************************************************************
describe:
check there two point is the same point or not

parameter:
dst: the dstination point
src: the source point

return:


time: monday, september third day 2020
******************************************************************/
bool CModelSplitAlg::IsSamePoint(const S3DPointF& dst, const S3DPointF& src)
{
	if (dst.fX == src.fX && dst.fY == src.fY && dst.fZ == src.fZ)
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*******************************************************************
describe:
check there two point is the same point or not, in this function, the float type has the precision, so we use the fabs function to check two float value is the same or not.

parameter:
dst: the dstination point
src: the source point

return:


time: monday, september third day 2020
******************************************************************/
bool CModelSplitAlg::IsSamePointFloat(const S3DPointF& dst, const S3DPointF& src)
{
	if (fabs(dst.fX - src.fX) < FLOAT_PRECISION && fabs(dst.fY - src.fY) < FLOAT_PRECISION && fabs(dst.fZ - src.fZ) < FLOAT_PRECISION)
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*******************************************************************
describe:
check there two line segment is the same or not. because one line segment has two ends, we need to compare them two times

parameter:
dst: the dstination point
src: the source point

return:


time: monday, september third day 2020
******************************************************************/
bool CModelSplitAlg::IsSameSegmentLine(const S3DPointF& dstStart, const S3DPointF& dstEnd, const S3DPointF& srcStart, const S3DPointF& srcEnd)
{
	if (IsSamePointFloat(dstStart, srcStart) && IsSamePointFloat(dstEnd, srcEnd))
	{
		return true;
	}
	if (IsSamePointFloat(dstStart, srcEnd) && IsSamePointFloat(dstEnd, srcStart))
	{
		return true;
	}

	return false;
}

/*******************************************************************
describe:
check these two triangle mesh connect or not. because one of these two triangle was taken to compare with another triangle, so one of this triangle mesh side may be connected with another triangle. for this kind side, we can't take it to check it connect with another triangle or not.
so we just take the sides that not connect with another triangle and check them to connect with the source triangle or not. when this side connect with another triangle, we will mark it into the member variable "iMark".


parameter:
dst: the dstination triangle mesh
src: the source triangle mesh

return:


time: monday, september third day 2020
******************************************************************/
int CModelSplitAlg::IsConnect(SFaceInfor& dst, SFaceInfor& src)
{
	for (int i = 0; i < 3; i++)
	{
		if (!((1 << i) & (src.iMark)))
		{
			for (int j = 0; j < 3; j++)
			{
				if (!((1 << j) & (dst.iMark)))
				{
					//yaoyaping:if two triangle connect, it means that it just have one side to connect. it doesn't have this kind case that two sides connect each other. because two side connect in two triangle, it must be the same triangle for these two triangles.
					if (IsSameSegmentLine(dst.pt[j], dst.pt[(j + 1) % 3], src.pt[i], src.pt[(i + 1) % 3]))
					{
						dst.iMark = dst.iMark | (1 << j);
						src.iMark = src.iMark | (1 << i);
						return i;
					}
				}
			}
		}
	}

	return -1;
}



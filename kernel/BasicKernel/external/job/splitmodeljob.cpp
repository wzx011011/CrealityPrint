#include "splitmodeljob.h"

#include <QSettings>

#include "data/modeln.h"

#include "interface/modelinterface.h"
#include "interface/selectorinterface.h"

#include "utils/modelpositioninitializer.h"
#include "utils/convexhullcalculator.h"

#include <QtCore/QDebug>

#include "interface/spaceinterface.h"

#include "modelsplitalg.h"
#include "interface/cacheinterface.h"
#include "data/modeln.h"
#include "data/modelgroup.h"

#include "mmesh/trimesh/meshtopo.h"
#include "mmesh/trimesh/trimeshutil.h"

#include "external/interface/visualsceneinterface.h"

#include "utils/matrixutil.h"
using namespace creative_kernel;


SplitModelJob::SplitModelJob(QObject* parent)
	:Job(parent)
	, m_bSplit(false)
	, m_pnewMesh(NULL)
{
}

SplitModelJob::~SplitModelJob()
{
}

void SplitModelJob::EnableSplit(bool bSplit)
{
	m_bSplit = bSplit;
}

QString SplitModelJob::name()
{
	return "Generate Geometry";
}

QString SplitModelJob::description()
{
	return "Generating Geometry.";
}

void SplitModelJob::failed()
{
}

void SplitModelJob::successed(qtuser_core::Progressor* progressor)
{
	if (m_bSplit)
	{
		AddSplitModel(progressor);
	}
	else
	{
		if (m_pnewMesh)
		{
			if (m_models.size() > 0)
			{
				ModelSpace* space = getModelSpace();
				ModelGroup* group = space->currentGroup();
				QList<ModelN*> nlist;
				modifyGroup(m_models, nlist, group, true);
			}

			TriMeshPtr mesh(m_pnewMesh);
			creative_kernel::ModelN* pNewModel = new creative_kernel::ModelN();
			if (pNewModel)
			{
				pNewModel->setTrimesh(mesh);
				pNewModel->setLocalPosition(m_SplitModelInfor.localPos);
				pNewModel->SetInitPosition(pNewModel->center());

				QString name = QString("%1#%2").arg(m_SplitModelInfor.objectName).arg(0) + ".stl";
				pNewModel->setObjectName(name);

				addModel(pNewModel, true);
			}
		}

	}



}

void SplitModelJob::work(qtuser_core::Progressor* progressor)
{
	if (m_bSplit)
	{
		SplitModel(progressor);
	}
	else
	{
		MergeModel(progressor);
	}
}


void SplitModelJob::MergeModel(qtuser_core::Progressor* progressor)
{
	m_pnewMesh = NULL;
	m_models.clear();
	m_models = selectionms();
	if (m_models.size() < 2)
	{
		qDebug() << "merge invalid num.";
		return;
	}

	//for making the name of this model
	ModelN* mFirst = m_models.at(0);

	if (mFirst)
	{
		m_SplitModelInfor.objectName = mFirst->objectName();
	}
	m_SplitModelInfor.objectName.chop(4);

	//create the new triangle mesh data and it will store all the submodels triangle mesh data into this triangle mesh data
	m_pnewMesh = new trimesh::TriMesh();

	if (m_pnewMesh)
	{
		CModelSplitAlg splitAlg;
		std::vector<trimesh::TriMesh*> vtTriMesh;
		//merging all the sub models triangle mesh data together.
		for (size_t i = 0; i < m_models.size(); i++)
		{
			ModelN* pCurModel = m_models.at(i);

			if (pCurModel && m_pnewMesh)
			{
				QMatrix4x4 globalMatrix = pCurModel->globalMatrix();

				trimesh::fxform gxf = qMatrix2Xform(globalMatrix);
				if (pCurModel->mesh())
				{
					//splitAlg.TranslateToGlobalPos(*m_pnewMesh, *pCurModel->mesh(), globalMatrix);

					vtTriMesh.push_back(pCurModel->mesh());

					mmesh::mergeTriMesh(m_pnewMesh, vtTriMesh, gxf, pCurModel->isFanZhuan());
					vtTriMesh.clear();
				}
			}
		}
		m_pnewMesh->need_bbox();
		ConvexHullCalculator::calculate(m_pnewMesh, nullptr);
		//mergeTriMesh(m_pnewMesh, vtTriMesh);
		//splitAlg.MoveToOriginal(*m_pnewMesh, m_SplitModelInfor.localPos);
	}
}


void SplitModelJob::AddSplitModel(qtuser_core::Progressor* progressor)
{
	for (int iSubModels = 0; iSubModels < m_meshes.size(); iSubModels++)
	{
		ModelN* pModel = m_models[iSubModels];
		if (NULL == pModel)
		{
			continue;
		}

		creative_kernel::ModelGroup* group = qobject_cast<creative_kernel::ModelGroup*>(pModel->parent());
		if (m_meshes.size() > 0 && group)
		{
			QList<ModelN*> newModels;
			QString name = pModel->objectName();
			int id = 0;

			std::vector<trimesh::TriMesh*> subMeshs = m_meshes[iSubModels];
			for (trimesh::TriMesh* mesh : subMeshs)
			{
				if (mesh->vertices.size() == 0 || mesh->faces.size() == 0)
				{
					delete mesh;
					continue;
				}
				++id;

				creative_kernel::ModelN* m = new creative_kernel::ModelN();
				QString subName = QString("%1-split-parts-%2").arg(name).arg(id);
				m->setObjectName(subName);

				trimesh::fxform xf = qMatrix2Xform(pModel->globalMatrix());
				size_t vertexCount = mesh->vertices.size();
				for (size_t i = 0; i < vertexCount; ++i)
				{
					trimesh::vec3 v = mesh->vertices.at(i);
					mesh->vertices.at(i) = xf * v;
				}

				mesh->need_bbox();
				QVector3D localPos;
				CModelSplitAlg::MoveToOriginal(mesh, localPos);

				TriMeshPtr ptr(mesh);
				creative_kernel::cache(ptr);
				m->setTrimesh(ptr);
				m->setLocalPosition(localPos);
				m->SetInitPosition(localPos);
				newModels.push_back(m);
			}
			m_meshes.clear();

			if (newModels.size() > 0)
			{
				QList<ModelN*> removes;
				removes << pModel;

				modifyGroup(removes, newModels, group, true);
			}

			requestVisUpdate(true);
		}
	}

}

void SplitModelJob::SplitModel(qtuser_core::Progressor* progressor)
{
	for (int iSubModel = 0; iSubModel < m_models.size(); iSubModel++)
	{
		ModelN* pModel = m_models.at(iSubModel);
		if (!pModel)
		{
			continue;
		}
		trimesh::TriMesh* mesh = pModel->mesh();
		mmesh::dumplicateMesh(mesh, nullptr);;

		if (!mesh || mesh->faces.size() <= 0)
		{
			continue;
		}

		if (progressor)
		{
			progressor->progress(0.1f);
		}

		mmesh::MeshTopo topo;
		topo.build(mesh);
		if (progressor)
		{
			progressor->progress(0.3f);

			if (progressor->interrupt())
			{
				return;
			}
		}

		int faceNum = (int)mesh->faces.size();
		std::vector<bool> visitFlags(faceNum, false);

		std::vector<int> visitStack;
		std::vector<int> nextStack;

		std::vector<std::vector<int>> parts;
		parts.reserve(100);
		for (int faceID = 0; faceID < faceNum; ++faceID)
		{
			if (visitFlags.at(faceID) == false)
			{
				visitFlags.at(faceID) = true;
				visitStack.push_back(faceID);

				std::vector<int> facesChunk;
				facesChunk.push_back(faceID);
				while (!visitStack.empty())
				{
					int seedSize = (int)visitStack.size();
					for (int seedIndex = 0; seedIndex < seedSize; ++seedIndex)
					{
						int cFaceID = visitStack.at(seedIndex);
						trimesh::ivec3& oppoHalfs = topo.m_oppositeHalfEdges.at(cFaceID);
						for (int halfID = 0; halfID < 3; ++halfID)
						{
							int oppoHalf = oppoHalfs.at(halfID);
							if (oppoHalf >= 0)
							{
								int oppoFaceID = topo.faceid(oppoHalf);
								if (visitFlags.at(oppoFaceID) == false)
								{
									nextStack.push_back(oppoFaceID);
									facesChunk.push_back(oppoFaceID);
									visitFlags.at(oppoFaceID) = true;
								}
							}
						}
					}

					visitStack.swap(nextStack);
					nextStack.clear();
				}

				parts.push_back(std::vector<int>());
				parts.back().swap(facesChunk);
			}
			else
			{
				visitFlags.at(faceID) = true;
			}

			if ((faceID + 1) % 100 == 0)
			{
				if (progressor->interrupt())
				{
					return;
				}
			}
			
		}

		std::vector<trimesh::TriMesh*> meshes;
		size_t partSize = parts.size();
		for (size_t i = 0; i < partSize; ++i)
		{
			if (parts.at(i).size() > 10)
			{
				trimesh::TriMesh* outMesh = mmesh::partMesh(parts.at(i), mesh);
				if (outMesh) meshes.push_back(outMesh);
			}
		}

		//merge small ones
		int tSize = (int)meshes.size();
		int maxCount = 0;
		for (int i = 0; i < tSize; ++i)
		{
			if (maxCount < (int)meshes.at(i)->vertices.size())
				maxCount = (int)meshes.at(i)->vertices.size();
		}

		int smallCount = (int)((float)maxCount * 0.05f);
		std::vector<trimesh::TriMesh*> allInOne;
		std::vector<trimesh::TriMesh*> validMeshes;
		for (int i = 0; i < tSize; ++i)
		{
			if ((int)meshes.at(i)->vertices.size() < smallCount)
				allInOne.push_back(meshes.at(i));
			else
				validMeshes.push_back(meshes.at(i));
		}

		if (allInOne.size() > 0)
		{
			trimesh::TriMesh* newMesh = new trimesh::TriMesh();
			mmesh::mergeTriMesh(newMesh, allInOne);
			validMeshes.push_back(newMesh);

			for (trimesh::TriMesh* m : allInOne)
				delete m;
			allInOne.clear();
		}
		for (trimesh::TriMesh* m : validMeshes)
			ConvexHullCalculator::calculate(m, nullptr);
		m_meshes.push_back(validMeshes);
		if (progressor)
		{
			progressor->progress(1);
		}
	}

}

void SplitModelJob::setModel(QList<creative_kernel::ModelN*> model)
{
	m_models = model;
}


#include "simplesupportop.h"
#include "fdmsupporthandler.h"

#include "qtuser3d/math/angles.h"
#include "qtuser3d/math/space3d.h"
#include "utils/matrixutil.h"

#include "data/modeln.h"
#include "data/gridcache.h"
#include "mmesh/trimesh/box2dgrid.h"

#include <QtCore/QDebug>

#include "mesh/shapecreator.h"
#include "qtuser3d/entity/alonepointentity.h"
#include "qtuser3d/entity/lineentity.h"

#include "interface/visualsceneinterface.h"
#include "interface/camerainterface.h"
#include "interface/modelinterface.h"
#include "interface/selectorinterface.h"
#include "interface/spaceinterface.h"
#include "interface/cacheinterface.h"
#include "interface/eventinterface.h"
#include "interface/machineinterface.h"

#include "data/fdmsupportgroup.h"
#include "data/fdmsupport.h"

#include "qtuser3d/effect/effectmanager.h"
#include "utils/trimesh2qgeometryrenderer.h"
#include<cfloat>
#include "kernel/abstractkernel.h"
using namespace creative_kernel;
SimpleSupportOp::SimpleSupportOp(QObject* parent)
	:SceneOperateMode(parent)
	, m_supportMode(SupportMode::esm_delete/*SupportMode::esm_add*/)//by TCJ
	, m_supportAngle(75.0f)
	, m_supportSize(2.0f)
{
	m_cache = new GridCache(this);
	m_debugEntity = new qtuser_3d::AlonePointEntity();
	//m_lineEntity = new qtuser_3d::LineEntity();
	//
	m_cylinderEntity = new BasicEntity();
	m_cylinderEntity->setEffect(qtuser_3d::getEffectManager()->create("finephong", m_cylinderEntity));
	QVector4D color = QVector4D(1.0f, 0.0f, 0.0f, 0.5f);
	m_cylinderEntity->createParameter("color", color);

	m_handler = new FDMSupportHandler(this);

	addMachineTracer(this);
}

SimpleSupportOp::~SimpleSupportOp()
{
	removeMachineTracer(this);

	delete m_debugEntity;
}

void SimpleSupportOp::onAttach()
{
	qDebug() << "support onAttach";
	visShow(m_debugEntity);
	//visShow(m_lineEntity);
	//m_debugEntity->setEnabled(false);
	//m_lineEntity->setEnabled(false);
	visShow(m_cylinderEntity);
	visHide(m_cylinderEntity);

	changeShaders("modelSupport");
	QList<ModelN*> models = modelns();
	m_cache->build(models);

	for (ModelN* model : models)
	{
		model->enterSupportStatus();
	}

	addLeftMouseEventHandler(this);
	addHoverEventHandler(this);

	addSelectTracer(this);
    enableSupportMode(true);
	int baseFace = 40000000;
	for (ModelN* model : models)
	{
		model->fdmSupport()->setFaceBase(baseFace);
		baseFace += 10000000;
	}

	m_handler->setPickSource(visPickSource());

	requestVisUpdate(true);
	m_bShowPop = true;
}

void SimpleSupportOp::onDettach()
{
	qDebug() << "support onDettach";
	visHide(m_debugEntity);
	//visHide(m_lineEntity);
	visHide(m_cylinderEntity);

	changeShaders("model");

	QList<ModelN*> models = modelns();
	for (ModelN* model : models)
	{
		model->leaveSupportStatus();
	}

	m_cache->clear();

	removeSelectorTracer(this);
    enableSupportMode(false);
	removeLeftMouseEventHandler(this);
	removeHoverEventHandler(this);

	m_handler->clear();
	requestVisUpdate(true);
	m_bShowPop = false;
}


void SimpleSupportOp::onSelectionsChanged()
{
//    QList<creative_kernel::ModelN*> selections = selectionms();
//    bool flag = (selections.size() == 0) ? false : true;
//    AbstractKernelUI::getSelf()->switchShowPop(flag);
}

void SimpleSupportOp::selectChanged(qtuser_3d::Pickable* pickable)
{
//    updateHelperEntity();
}

void SimpleSupportOp::changeShaders(const QString& name)
{
	//QList<ModelN*> models = modelns();
	//for (ModelN* m : models)
	//	m->effect()->setViewPass(name);
}

qtuser_3d::Pickable* SimpleSupportOp::surfaceXy(const QPoint& pos, int* primitiveID)
{
	Pickable* pickable = checkPickable(pos, primitiveID);
	return pickable;
}

void SimpleSupportOp::onLeftMouseButtonClick(QMouseEvent* event)
{
	qDebug() << "support onLeftMouseButtonClick";

	if (m_supportSize > -0.0001f && m_supportSize < 0.0001f)
	{
		//֧�Ŵ�СΪ0ʱ���޷��ֶ�����֧��
		return;
	}

	auto checkCollide = [&event](unsigned primitiveID, TrimeshEntity* entity)->QVector3D {
		QVector3D collide;
		entity->rayCheck(primitiveID, visRay(event->pos()), collide, nullptr);
		return collide;
	};

	int primitiveID = 0;
	Pickable* pickable = surfaceXy(event->pos(), &primitiveID);

	QVector3D c;
	if (pickable && m_supportMode == SupportMode::esm_add)
	{
		std::vector<mmesh::VerticalSeg> tcollides;
		ModelN* m = qobject_cast<ModelN*>(pickable);
		mmesh::Box2DGrid* tree = nullptr;

		float supportSize = m_supportSize;
		//qtuser_3d::Box3D box = m->globalSpaceBox();
		//QVector3D boxSize = box.size();
		//float dmax = std::max(boxSize.x(), boxSize.y()) * 0.01f;
		//if (supportSize < dmax)
		//{
		//	supportSize = dmax;
		//}
			
		if (m)
		{
			c = checkCollide(primitiveID, m);
			tree = m_cache->grid(m);

			if (!tree->checkFace(primitiveID))
			{
				tree = nullptr;
			}
			m->setSupportLiftHeight(m->globalSpaceBox().min.z());
		}
		else
		{
			qtuser_3d::Ray ray = visRay(event->pos());
			QVector3D pc(0.0f, 0.0f, 0.0f);
			QVector3D pn(0.0f, 0.0f, 1.0f);
			QVector3D xyz;
			if (lineCollidePlane(pc, pn, ray, xyz))
			{
				m = m_cache->check(trimesh::vec2(xyz.x(), xyz.y()));
				tree = m_cache->grid(m);
				c = xyz;
			}
		}

		if (tree)
		{
			trimesh::vec3 tc(c.x(), c.y(), c.z());
			tree->check(tcollides, tc, primitiveID);
		}

		if (tcollides.size() == 1 && tree)
		{
			trimesh::fxform xf = qMatrix2Xform(m->globalMatrix());
			trimesh::fxform invXf = trimesh::inv(xf);

			mmesh::VerticalSeg& seg = tcollides.at(0);
			FDMSupportParam param;
			param.bottom = invXf * seg.b;
			param.top = invXf * seg.t;
			QVector3D s = m->localScale();
			s = QVector3D(supportSize, supportSize, supportSize) / s;
			param.radius = std::min(s.x(), s.y()) * sqrtf(2.0f) * 0.9f / 2.0f;

			FDMSupportGroup* supportGroup = m->fdmSupport();
			supportGroup->createSupport(param);
		}
	}
	else if (m_supportMode == SupportMode::esm_delete)
	{
		QVector3D debugPosition;
		
		QList<ModelN*> selections = selectionms();
		FDMSupport* support = m_handler->check(event->pos(), debugPosition);
		if (support)
		{
			//FDMSupportGroup* supportGroup = qobject_cast<FDMSupportGroup*>(support->parent());
			for (int i = 0; i < selections.size(); i++)
			{
				FDMSupportGroup* supportGroup = selections.at(i)->fdmSupport();

				QList<FDMSupport*> listSuport = supportGroup->FDMSupports();

				if (listSuport.contains(support))
				{
					supportGroup->removeSupport(support);
				}
			}

			m_handler->reset();
		}
	}

	requestVisUpdate(true);
}

void SimpleSupportOp::onHoverMove(QHoverEvent* event)
{
	//-------by TCJ ------
	visHide(m_cylinderEntity);
	if (m_supportSize > -0.0001f && m_supportSize < 0.0001f)
	{
		return;
	}
	auto checkCollide = [&event](unsigned primitiveID, TrimeshEntity* entity)->QVector3D {
		QVector3D collide;
		entity->rayCheck(primitiveID, visRay(event->pos()), collide, nullptr);
		return collide;
	};

	int primitiveID = 0;
	Pickable* pickable = surfaceXy(event->pos(), &primitiveID);

	QVector3D c;
	if (pickable && m_supportMode == SupportMode::esm_add)
	{
		std::vector<mmesh::VerticalSeg> tcollides;
		ModelN* m = qobject_cast<ModelN*>(pickable);
		mmesh::Box2DGrid* tree = nullptr;

		float supportSize = m_supportSize;
		
		if (m)
		{
			c = checkCollide(primitiveID, m);
			tree = m_cache->grid(m);

			if (!tree->checkFace(primitiveID))
			{
				tree = nullptr;
			}
		}
		else
		{
			qtuser_3d::Ray ray = visRay(event->pos());
			QVector3D pc(0.0f, 0.0f, 0.0f);
			QVector3D pn(0.0f, 0.0f, 1.0f);
			QVector3D xyz;
			if (lineCollidePlane(pc, pn, ray, xyz))
			{
				m = m_cache->check(trimesh::vec2(xyz.x(), xyz.y()));
				tree = m_cache->grid(m);
				c = xyz;
			}
		}

		if (tree)
		{
			trimesh::vec3 tc(c.x(), c.y(), c.z());
			tree->check(tcollides, tc, primitiveID);
		}

		if (tcollides.size() == 1 && tree)
		{
			trimesh::fxform xf = qMatrix2Xform(m->globalMatrix());
			trimesh::fxform invXf = trimesh::inv(xf);

			mmesh::VerticalSeg& seg = tcollides.at(0);			
			QVector3D s = m->localScale();
			//s = QVector3D(supportSize, supportSize, supportSize) / s;
			s = QVector3D(supportSize, supportSize, supportSize);
			float cylinderRadius = std::min(s.x(), s.y()) * sqrtf(2.0f) * 0.9f / 2.0f;

			visShow(m_cylinderEntity);
			if (m_cylinderEntity->isEnabled())
			{
				m_cylinderMesh.reset(ShapeCreator::createCylinderMesh(QVector3D(seg.t.x, seg.t.y, seg.t.z),
					QVector3D(seg.b.x, seg.b.y, seg.b.z),
					cylinderRadius, 4/*30*/, 45));
				Qt3DRender::QGeometry* geometry = trimesh2GeometryFace(m_cylinderMesh.get(), nullptr);
				m_cylinderEntity->setGeometry(geometry);
			}
		}
	}
	else if (m_supportMode == SupportMode::esm_delete)
	{
		QVector3D debugPosition;
		FDMSupport* support = m_handler->hover(event->pos(), debugPosition);
		if (support)
		{
			visShow(m_debugEntity);
			m_debugEntity->setColor(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		}
		m_debugEntity->updateGlobal(debugPosition);
	}

	requestVisUpdate(true);
	//-----end--------
#if 0
	if (m_cache->size() == 0)
	{
		return;
	}
	visHide(m_debugEntity);
	visHide(m_lineEntity);
	//m_debugEntity->setEnabled(false);
	//m_lineEntity->setEnabled(false);

	QVector3D debugPosition;
	if (m_supportMode == SupportMode::esm_add)
	{
		QVector3D position, normal;
		int primitiveID = -1;
		ModelN* hoverModel = checkPickModel(event->pos(), position, normal, &primitiveID);
		mmesh::Box2DGrid* hoverTree = nullptr;
		if (hoverModel)
		{
			hoverTree = m_cache->grid(hoverModel);
			debugPosition = position;
			normal.normalize();
			if (qAbs(QVector3D::dotProduct(normal, QVector3D(0.0f, 0.0f, 1.0f))) < 0.001)
			{
				hoverTree = nullptr;
			}
		}
		else
		{
			qtuser_3d::Ray ray = visRay(event->pos());
			QVector3D pc(0.0f, 0.0f, 0.0f);
			QVector3D pn(0.0f, 0.0f, 1.0f);
			QVector3D xyz;
			if (lineCollidePlane(pc, pn, ray, xyz))
			{
				hoverModel = m_cache->check(trimesh::vec2(xyz.x(), xyz.y()));
				hoverTree = m_cache->grid(hoverModel);
				debugPosition = xyz;
			}
		}

		std::vector<mmesh::VerticalSeg> segments;
		if (hoverTree)
		{
			trimesh::vec3 tc(debugPosition.x(), debugPosition.y(), debugPosition.z());
			hoverTree->check(segments, tc, primitiveID);
		}

		if (segments.size() > 0)
		{
			std::vector<trimesh::vec3> points;
			for (size_t i = 0; i < segments.size(); ++i)
			{
				mmesh::VerticalSeg& seg = segments.at(i);
				points.push_back(seg.b);
				points.push_back(seg.t);
			}

			m_lineEntity->updateGeometry((int)points.size(), (float*)&points.at(0));
			visShow(m_lineEntity);
			//m_lineEntity->setEnabled(true);
			m_lineEntity->setColor(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));

			visShow(m_debugEntity);
			//m_debugEntity->setEnabled(true);
			m_debugEntity->setColor(QVector4D(0.0f, 1.0f, 0.0f, 1.0f));
		}
		else
		{
			visShow(m_debugEntity);
			//m_debugEntity->setEnabled(true);
			m_debugEntity->setColor(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}
	else if (m_supportMode == SupportMode::esm_delete)
	{
		FDMSupport* support = m_handler->hover(event->pos(), debugPosition);
		if (support)
		{
			visShow(m_debugEntity);
			//m_debugEntity->setEnabled(true);
			m_debugEntity->setColor(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	m_debugEntity->updateGlobal(debugPosition);

	requestVisUpdate();
#endif
}

bool SimpleSupportOp::hasSupport()
{
	QList<creative_kernel::ModelN*> selections = selectionms();
	foreach(creative_kernel::ModelN * model, selections)
	{
		if (model->hasFDMSupport())
		{
			FDMSupportGroup* p_support = model->fdmSupport();
			if (p_support->fdmSupportNum())
			{
				return true;
			}
		}
		if (model->hasAttach())
		{
			return true;
		}
	}
	return false;
}

void SimpleSupportOp::setAddMode()
{
	m_supportMode = SupportMode::esm_add;
}

void SimpleSupportOp::setDeleteMode()
{
	m_supportMode = SupportMode::esm_delete;
}

void SimpleSupportOp::deleteAllSupport()
{
	QList<ModelN*> selections = selectionms();
//	if (selections.size() > 0)
	for(int i = 0; i < selections.size(); i++)
	{
		FDMSupportGroup* supportGroup = selections.at(i)->fdmSupport();
		supportGroup->clearSupports();
		m_handler->reset();
	}

	requestVisUpdate(true);
}

void SimpleSupportOp::onMachineChanged(QString oldmachine, QString newmachine)
{
	//
}

void SimpleSupportOp::onMachineTypeChanged(int oldmachinetype, int newmachinetype)
{
	QList<ModelN*> selections = modelns();
	for (int i = 0; i < selections.size(); i++)
	{
		FDMSupportGroup* supportGroup = selections.at(i)->fdmSupport();
		supportGroup->clearSupports();
		m_handler->reset();
	}

	requestVisUpdate(true);
}

void SimpleSupportOp::setSupportAngle(float angle)
{
	m_supportAngle = angle;

	// 有乘除法或者三角函数计算时最好采用双精度，尽量不要用单精度去存储中间结果，以避免精度丢失，进而对临界值的计算造成影响
    //float minOverhangCos = cosf(M_PIf * (90.0f - angle) / 180.0f);
	float minOverhangCos = cos(M_PIl * (90.0 - angle) / 180.0);

	QList<ModelN*> models = selectionms();
    for(ModelN * model : models)
	{
		model->setSupportCos(minOverhangCos);
	}

	requestVisUpdate(true);
}

void SimpleSupportOp::setSupportSize(float size)

{
#ifdef __APPLE__
	if (1 == isnan(size))
#else
	if (1 == std::isnan(size))
#endif
	{
		size = 0.0f;
	}
	m_supportSize = size;
	if (m_supportSize > -0.0001f && m_supportSize < 0.0001f) return;
	if (m_supportSize < 0.1f) m_supportSize = 2.0f;
	if (m_supportSize > 20.0f) m_supportSize = 20.0f;
}

void SimpleSupportOp::autoSupport(bool platform)
{
	QList<creative_kernel::ModelN*> models = selectionms();
	if (models.size() == 0) return;

	for (int i = 0; i < models.size(); i++)
	{
		ModelN* m = models.at(i);

		float supportSize = m_supportSize;

		std::vector<mmesh::VerticalSeg> supports;
		mmesh::Box2DGrid* grid = m_cache->grid(m);
		if (grid)
		{
			grid->buildGlobalProperties();
			grid->autoSupport(supports, supportSize, m_supportAngle, platform);
			size_t FaceSupportsize = supports.size();
			grid->autoSupportOfVecAndSeg(supports, supportSize, platform, true);

			size_t size = supports.size();
			qDebug() << "Auto add supports " << size;

			for (int i = 0; i < size; ++i)
			{
				trimesh::fxform xf = qMatrix2Xform(m->globalMatrix());
				trimesh::fxform invXf = trimesh::inv(xf);

				mmesh::VerticalSeg& seg = supports.at(i);
				FDMSupportParam param;
				param.bottom = invXf * seg.b;
				param.top = invXf * seg.t;

				QVector3D s = m->localScale();
				if (i < FaceSupportsize)
					s = QVector3D(supportSize, supportSize, supportSize) / s;
				else
					s = QVector3D(1.5 * supportSize, 1.5 * supportSize, 1.5 * supportSize) / s;
				param.radius = std::min(s.x(), s.y()) * sqrtf(2.0f) * 0.9f / 2.0f;

				FDMSupportGroup* supportGroup = m->fdmSupport();
				supportGroup->createSupport(param);
			}
		}

		m->setSupportLiftHeight(m->globalSpaceBox().min.z());
	}

	requestVisUpdate(true);
}

bool SimpleSupportOp::getShowPop()
{
	return m_bShowPop;
}

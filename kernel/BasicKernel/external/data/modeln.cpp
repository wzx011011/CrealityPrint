#include "modeln.h"

#include "qtuser3d/entity/modelnentity.h"
#include "qtuser3d/entity/purecolorentity.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/effect/texturecreator.h"

#include "interface/reuseableinterface.h"
#include "interface/spaceinterface.h"
#include "interface/cacheinterface.h"
#include "interface/selectorinterface.h"

#include "data/fdmsupportgroup.h"

#include "us/modelsetting.h"

#include "external/utils/trimesh2qgeometryrenderer.h"
#include "external/utils/trimeshselect.h"
#include "external/data/modeleffect.h"

#include "mmesh/trimesh/trimeshutil.h"
//#include "mmesh/trimesh/meshrepair.h"
//#include "repair/mesh/meshrepair.h"

#include "cmesh/mesh/repair.h"

#include "utils/matrixutil.h"

#include <QColor>
#include "imageproc/imageloader.h"

namespace creative_kernel
{
	ModelN::ModelN(QObject* parent)
		:TrimeshEntity(parent)
		, m_fdmSupportGroup(nullptr)
		, m_nozzle(0)
		, m_attachUpdating(true)
		, m_meshWithAttachCache(nullptr)
		, m_errorEdges(0)
		, m_errorNormals(0)
        , m_errorHoles(0)
        , m_errorIntersects(0)
		, m_localAngleStack({QVector3D()})
		, m_currentLocalDispalyAngle(0)
	{
		qtuser_3d::Box3D box3d = baseBoundingBox();

		m_entity = new ModelNEntity();
		m_entity->setEffect(getCachedModelEffect(&box3d.min, &box3d.max));

		m_setting = new us::ModelSetting(this);

		m_visibility = true;
		m_showType = 1;
		m_detectAdd = true;
		m_supportLiftHeight = 10.5;
	}
	
	ModelN::~ModelN()
	{
	}

	void ModelN::setModelName(const QString& name)
	{
		setObjectName(name);
		m_entity->setObjectName(name + ".ModelNEntity");
		m_entity->setConvexName(m_entity->objectName() + ".Convex");
	}

	int ModelN::haveAuxiliary(int pos) const
	{
		if (pos < 0 || pos >= m_auxiliaryMeshs.size())
		{
			return -1;
		}
		if (m_auxiliaryMeshs[0].get() == nullptr)
		{
			return 0;
		}
		return 1;
	}

	int ModelN::prepareHollow()
	{
		if (m_auxiliaryMeshs[1])
		{
			m_auxiliaryMeshs[1].reset();
		}
		if (m_auxiliaryMeshs[2])
		{
			m_auxiliaryMeshs[2].reset();
		}
		if (m_auxiliaryMeshs[0].get() == nullptr)
		{
			m_auxiliaryMeshs[0].reset(new trimesh::TriMesh);
			*m_auxiliaryMeshs[0] = *m_mesh;
			return 0;
		}
		else
		{
			*m_mesh = *m_auxiliaryMeshs[0];
			return 1;
		}
	}

	int ModelN::setAuxiliaryMesh(int pos, const trimesh::TriMesh& mesh, const trimesh::xform& xf)
	{
		if (pos < 0 || pos >= m_auxiliaryMeshs.size())
		{
			return -1;
		}
		if (m_auxiliaryMeshs[pos].get() == nullptr)
		{
			m_auxiliaryMeshs[pos].reset(new trimesh::TriMesh);
		}
		*m_auxiliaryMeshs[pos] = mesh;
		trimesh::apply_xform(m_auxiliaryMeshs[pos].get(), xf);
		return 0;
	}

	trimesh::TriMesh* ModelN::getAuxiliaryMesh(int pos)
	{
		if (pos < 0 || pos >= m_auxiliaryMeshs.size())
		{
			return nullptr;
		}
		if (m_auxiliaryMeshs[pos])
		{
			return m_auxiliaryMeshs[pos].get();
		}
		return nullptr;
	}

	TriMeshPtr ModelN::getAuxiliaryMeshPtr(int pos)
	{
		if (pos < 0 || pos >= m_auxiliaryMeshs.size())
		{
			return TriMeshPtr();
		}
		if (m_auxiliaryMeshs[pos])
		{
			return m_auxiliaryMeshs[pos];
		}
		return nullptr;
	}

	int ModelN::numAuxiliaryMesh() const
	{
		return m_auxiliaryMeshs.size();
	}

	void ModelN::onGlobalMatrixChanged(const QMatrix4x4& globalMatrix)
	{
		m_entity->setPose(globalMatrix);
		qtuser_3d::Box3D box = globalSpaceBox();

		QMatrix4x4 matrix = globalMatrix;
		m_entity->updateBoxLocal(box, matrix);

		//m_lineEntity->setPose(globalMatrix);

		setModelEffectBottomZ(box.min.z());

		//convex xy
		QMatrix4x4 invMatrix = matrix.inverted();
		qtuser_3d::ConvexPolygon& polygon = convexPolygon();
		m_entity->updateConvex(polygon.m_polygons, invMatrix, true);
        using namespace creative_kernel;
        creative_kernel::onModelSceneChanged();

		// 更新附属模型的矩阵变换
		for (auto itr = m_qmapofAttachModel.begin(); itr != m_qmapofAttachModel.end(); itr++)
		{
			itr.value()->setParent2Global(globalMatrix);
		} 
    }

	void ModelN::onStateChanged(ControlState state)
	{
		m_entity->setState((float)state);
		m_entity->setBoxVisibility(selected() ? true : false);
	}


	QVector3D ModelN::localRotateAngle()
	{
		return m_localAngleStack[m_currentLocalDispalyAngle];
	}

	QQuaternion ModelN::rotateByStandardAngle(QVector3D axis, float angle)
	{
		QVector3D currentLDR = m_localAngleStack[m_currentLocalDispalyAngle];
		QVector3D v = axis * angle;
		currentLDR += v;

		m_localAngleStack.erase(m_localAngleStack.begin() + m_currentLocalDispalyAngle + 1, m_localAngleStack.end());
		m_localAngleStack.push_back(currentLDR);
		m_currentLocalDispalyAngle++;

		setLocalQuaternion(QQuaternion::fromAxisAndAngle(axis, angle) * m_localRotate);
		return m_localRotate;
	}

	void ModelN::updateDisplayRotation(bool redoFlag, int step)
	{
		if (redoFlag)
			m_currentLocalDispalyAngle += step;
		else
			m_currentLocalDispalyAngle -= step;

		m_currentLocalDispalyAngle = std::min(m_localAngleStack.size() - 1, std::max(0, m_currentLocalDispalyAngle));
	}

	void ModelN::resetRotate()
	{
		QVector3D currentLDR = m_localAngleStack[m_currentLocalDispalyAngle];
		currentLDR = QVector3D();

		m_localAngleStack.erase(m_localAngleStack.begin() + m_currentLocalDispalyAngle + 1, m_localAngleStack.end());
		m_localAngleStack.push_back(currentLDR);
		m_currentLocalDispalyAngle++;

		QQuaternion q = QQuaternion();
		setLocalQuaternion(q);
	}

	void ModelN::meshChanged(qtuser_core::Progressor* progressor)
	{
		TrimeshCacheField* field = getCache(m_mesh);

		Qt3DRender::QGeometry* geometry = field->getGeometry(0, progressor);
		m_entity->setGeometry(geometry);
    }

	void ModelN::meshFromGeometryData(GeometryData* faceGeoData, GeometryData* lineGeoData, GeometryData* edgeGeoData)
	{
		TrimeshCacheField* field = getCache(m_mesh);
		setTexture();
		Qt3DRender::QGeometry* geometry = field->getGeometryByData(0, faceGeoData);
		m_entity->setGeometry(geometry);
	}

	void ModelN::faceBaseChanged(int faceBase)
	{
		QPoint vertexBase(0, 0);
		vertexBase.setX(faceBase * 3);
		m_entity->setVertexBase(vertexBase);
	}

	void ModelN::setSupportsVisibility(bool visibility)
	{
		if (m_fdmSupportGroup)
			m_fdmSupportGroup->setVisibility(visibility);
	}

	us::ModelSetting* ModelN::setting()
	{
		return m_setting;
	}

	void ModelN::setsetting(us::ModelSetting* modelsetting)
	{
		m_setting = modelsetting;
	}

	void ModelN::showModelFace()
	{
		spaceShow(m_entity);
		setSupportsVisibility(true);
	}

	void ModelN::hideModelFace()
	{
		spaceHide(m_entity);

		setSupportsVisibility(false);
	}

	/*void ModelN::showModelLine()
	{
		spaceShow(m_lineEntity);
		m_edgeEntity->setParent((Qt3DCore::QEntity*)nullptr);
	}

	void ModelN::hideModelLine()
	{
		spaceHide(m_lineEntity);
	}*/

	void ModelN::setShowType(int showtype)
	{
		m_showType = showtype;

		m_entity->setRenderMode(showtype);
	}

	void ModelN::setVisibility(bool visibility)
	{
		m_visibility = visibility;
		if (m_showType & 1)
		{
			visibility ? spaceShow(m_entity) : spaceHide(m_entity);
			setSupportsVisibility(visibility);
		}
		if (m_showType & 2)
		{
			visibility ? spaceShow(m_entity) : spaceHide(m_entity);
		}
	}

	bool ModelN::isVisible()
	{
		return m_visibility;
	}

	qtuser_3d::ModelNEntity* ModelN::getModelEntity()
	{
		return m_entity;
	}

	void ModelN::setCustomColor(QColor color)
	{
		m_entity->setCustomColor(color);
	}

	QColor ModelN::getCustomColor()
	{
		return m_entity->getCustomColor();
	}

	void ModelN::mirror(const QMatrix4x4& matrix, bool apply)
	{
		Node3D::mirror(matrix, apply);
		bool fanzhuan = isFanZhuan();
		m_entity->setFanZhuan((int)fanzhuan);
	}

	void ModelN::setTransparency(float alpha)
	{	
		alpha = std::min(1.0f, std::max(0.0f, alpha));

		if (m_entity->getTransparency() >=1.0 && alpha < 1.0)
		{
			m_entity->setEffect(EFFECTCREATE("model.alpha", m_entity));

			Qt3DRender::QBlendEquationArguments* blend = m_entity->blendArguments();
			if (blend)
			{
				blend->setSourceRgba(Qt3DRender::QBlendEquationArguments::SourceAlpha);
				blend->setDestinationRgba(Qt3DRender::QBlendEquationArguments::OneMinusSourceAlpha);
			}
		}
		else if (m_entity->getTransparency() < 1.0 && alpha >= 1.0)
		{
			qtuser_3d::Box3D box3d = baseBoundingBox();
			m_entity->setEffect(getCachedModelEffect(&box3d.min, &box3d.max));
		}

		m_entity->setTransparency(alpha);
	}

	float ModelN::getTransparency()
	{
		return m_entity->getTransparency();
	}

	void ModelN::setLightingEnable(bool enable)
	{
		m_entity->setLightingEnable(enable);
	}

	bool ModelN::getLightingEnable()
	{
		return m_entity->getLightingEnable();
	}

	void ModelN::setNozzle(int nozzle)
	{
		m_nozzle = nozzle;
		if (m_entity != nullptr)
		{
			m_entity->setNozzle(nozzle);
			const QHash<QString, us::USetting*>& MS = m_setting->settings();
			if (MS.contains("extruder_nr"))
			{
				MS["extruder_nr"]->setValue(nozzle);
			}
			if (MS.contains("support_bottom_extruder_nr"))
			{
				MS["support_bottom_extruder_nr"]->setValue(nozzle);
			}
		}
	}

	int ModelN::nozzle()
	{
		return m_nozzle;
	}

	QVector3D& ModelN::GetInitPosition()
	{
		return m_initPosition;
	}

	void ModelN::SetInitPosition(QVector3D QPosition)
	{
		m_initPosition = QPosition;
	}

	bool ModelN::hasFDMSupport()
	{
		return m_fdmSupportGroup && (m_fdmSupportGroup->fdmSupportNum()>0);
	}

	void ModelN::setFDMSup(FDMSupportGroup* fdmSup)
	{
		m_fdmSupportGroup = fdmSup;
		m_fdmSupportGroup->setParent(this);
		m_fdmSupportGroup->setParent2Global(globalMatrix());
		m_fdmSupportGroup->setVisibility(true);
	}


	void ModelN::setState(float state)
	{
		m_entity->setState(state);
	}

	void ModelN::setErrorState(bool error)
	{
		m_entity->setErrorState(error);
	}

	void ModelN::setBoxState(int state)
	{
		if(state == 0)
			m_entity->setBoxVisibility(false);
		else
			m_entity->setBoxVisibility(selected() ? true : false);
	}

    FDMSupportGroup* ModelN::fdmSupport()
	{
        if (!m_fdmSupportGroup)
            buildFDMSupport();
		return m_fdmSupportGroup;
	}

	void ModelN::setFDMSuooprt(bool detectAdd)
	{
		m_detectAdd = detectAdd;
	}

	bool ModelN::getFDMSuooprt()
	{
		return m_detectAdd;
	}

	void ModelN::enterSupportStatus()
	{
		m_entity->enterSupportStatus();
	}

	void ModelN::leaveSupportStatus()
	{
		m_entity->leaveSupportStatus();
	}

	void ModelN::setSupportCos(float cos)
	{
		m_entity->setSupportCos(cos);
	}

	void ModelN::setWaterPoint(QVector3D water, float zcha)
	{
		m_entity->setWaterPoint(water, zcha);
	}

	void ModelN::setNeedCheckScope(int checkscope)
	{
		m_entity->setNeedCheckScope(checkscope);
	}

	void ModelN::unSetWaterPoint()
	{
		m_entity->unSetWaterPoint();
	}

	qtuser_3d::Box3D ModelN::boxWithSup()
	{
		qtuser_3d::Box3D box = Node3D::globalSpaceBox();
		return box;
	}

    void ModelN::buildFDMSupport()
	{
        //bool bHasFdm = true;
		if (m_fdmSupportGroup) return;
		m_fdmSupportGroup = new FDMSupportGroup(this);
            m_fdmSupportGroup->setParent2Global(globalMatrix());
            m_fdmSupportGroup->setVisibility(true);


	}

	void ModelN::setSupportLiftHeight(float value)
	{
		m_supportLiftHeight = value;
	}
	float ModelN::getSupportLiftHeight()
	{
		return m_supportLiftHeight;
	}

	
	void ModelN::SetModelViewClrState(ControlState statevalue,bool boxshow)
	{
		//m_entity->setState((float)ControlState::selected);
		m_entity->setState((float)statevalue);
		m_entity->setBoxVisibility(boxshow);
	}
	void ModelN::insertAttachModel(QString uid, ModelN* model)
	{
		if (model)
		{
			model->setParent2Global(globalMatrix());
			model->setVisibility(true);

			m_qmapofAttachModel.insert(uid, model);

			m_attachUpdating = true;
		}
	}

	void ModelN::removeAttachModel(QString uid)
	{
		ModelN* removed = m_qmapofAttachModel.take(uid);

		if (removed)
		{
			removed->setVisibility(false);
			removed->deleteLater();
			removed = nullptr;

			m_attachUpdating = true;
		}
	}

	void ModelN::clearAttachModel()
	{
		if (m_qmapofAttachModel.size() > 0)
		{
			for (auto itr = m_qmapofAttachModel.begin(); itr != m_qmapofAttachModel.end(); itr++)
			{
				ModelN* removed = itr.value();
				removed->setVisibility(false);
				removed->deleteLater();
				removed = nullptr;
			}

			m_qmapofAttachModel.clear();

			m_attachUpdating = true;
		}
	}

	ModelN* ModelN::getAttachModel(QString uid)
	{
		return m_qmapofAttachModel.value(uid);
	}

	QList<QString> ModelN::getAttachModelIDList()
	{
		return m_qmapofAttachModel.keys();
	}

	bool ModelN::hasAttach()
	{
		return !m_qmapofAttachModel.isEmpty();
	}

	trimesh::TriMesh* ModelN::mesh(bool withAttach, bool attachUpdating)
	{
		TriMeshPtr resultPtr = meshptr(withAttach, attachUpdating);
		return resultPtr  ? &*resultPtr : nullptr;
	}

	TriMeshPtr ModelN::meshptr(bool withAttach, bool attachUpdating)
	{
		TriMeshPtr resultMesh = nullptr;
		if (withAttach)
		{
			if (m_attachUpdating || attachUpdating)
			{
				std::vector<trimesh::TriMesh*> inMeshes;
				inMeshes.push_back(TrimeshEntity::mesh());

				for (auto itr = m_qmapofAttachModel.begin(); itr != m_qmapofAttachModel.end(); itr++)
				{
					trimesh::TriMesh* mesh = itr.value()->mesh(withAttach, attachUpdating);

					trimesh::apply_xform(mesh, trimesh::xform(qMatrix2Xform(itr.value()->m_local2Parent)));

					inMeshes.push_back(mesh);
				}

				trimesh::TriMesh* outMesh = new trimesh::TriMesh();
				mmesh::mergeTriMesh(outMesh, inMeshes);
				m_meshWithAttachCache = TriMeshPtr(outMesh);

				m_attachUpdating = false;
			}

			resultMesh = m_meshWithAttachCache;
		}
		else
		{
			resultMesh = TrimeshEntity::meshptr();
		}

		return resultMesh;
	}

	int ModelN::getErrorEdges()
	{
		return  m_errorEdges;
	}
	int ModelN::getErrorNormals()
	{
		return m_errorNormals;
	}

	int ModelN::getErrorHoles()
	{
		return m_errorHoles;
	}
	int ModelN::getErrorIntersects()
	{
		return m_errorIntersects;
	}

    void ModelN::setErrorEdges(int value)
    {
        m_errorEdges = value;
    }
    void ModelN::setErrorNormals(int value)
    {
        m_errorNormals = value;
    }
    void ModelN::setErrorHoles(int value)
    {
        m_errorHoles = value;
    }
    void ModelN::setErrorIntersects(int value)
    {
        m_errorIntersects = value;
    }

	void ModelN::needDetectError()
	{
		//repair::getErrorEdges(mesh(), m_errorEdges, m_errorNormals);
		cmesh::ErrorInfo  errorInfo;
		cmesh::getErrorInfo(mesh(), errorInfo);
		m_errorEdges = errorInfo.edgeNum;      //缺陷边
		m_errorNormals = errorInfo.normalNum;    //缺陷的法线
		m_errorHoles = errorInfo.holeNum;      //孔洞个数
		m_errorIntersects = errorInfo.intersectNum; //非流面
	}

	void ModelN::setTexture()
	{
		int maptype = trimesh::Material::MapType::DIFFUSE;
		int bufferSize = m_mesh->map_bufferSize[maptype];
		unsigned char* imagedata = m_mesh->map_buffers[maptype];
		if (imagedata)
		{
			QImage aimage;
			aimage.loadFromData(imagedata, bufferSize);
			aimage = aimage.rgbSwapped();
			aimage = aimage.mirrored(false,true);
			Qt3DRender::QTexture2D* textureDiffuse = createFromImage(aimage);
			m_entity->setTDiffuse(textureDiffuse);
		}

		maptype = trimesh::Material::MapType::SPECULAR;
		bufferSize = m_mesh->map_bufferSize[maptype];
		imagedata = m_mesh->map_buffers[maptype];
		if (imagedata)
		{
			QImage aimage;
			aimage.loadFromData(imagedata, bufferSize);
			Qt3DRender::QTexture2D* textureSpecular = createFromImage(aimage);
			m_entity->setTAmbient(textureSpecular);
		}

		maptype = trimesh::Material::MapType::AMBIENT;
		bufferSize = m_mesh->map_bufferSize[maptype];
		imagedata = m_mesh->map_buffers[maptype];
		if (imagedata)
		{
			QImage aimage;
			aimage.loadFromData(imagedata, bufferSize);
			Qt3DRender::QTexture2D* textureAmbient = createFromImage(aimage);
			m_entity->setTSpecular(textureAmbient);
		}

		maptype = trimesh::Material::MapType::NORMAL;
		bufferSize = m_mesh->map_bufferSize[maptype];
		imagedata = m_mesh->map_buffers[maptype];
		if (imagedata)
		{
			QImage aimage;
			aimage.loadFromData(imagedata, bufferSize);
			Qt3DRender::QTexture2D* textureNormal = createFromImage(aimage);
			m_entity->setTNormal(textureNormal);
		}
	}

}

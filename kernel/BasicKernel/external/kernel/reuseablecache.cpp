#include "reuseablecache.h"
#include "qtuser3d/entity/printerentity.h"
#include "qtuser3d/camera/screencamera.h"
#include "qtuser3d/effect/ueffect.h"

#include "qtuser3d/utils/shaderprogrammanager.h"
#include "qtuser3d/utils/primitiveshapecache.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/renderpass/renderpassmanager.h"

#include "external/data/modeleffect.h"
#include "external/data/effectparametersetter.h"
#include "external/interface/renderinterface.h"

#include <Qt3DRender/QCullFace>

namespace creative_kernel
{
	ReuseableCache::ReuseableCache(QObject* parent)
		:QObject(parent)
		, m_printerEntity(nullptr)
	{
		m_mainCamera = new qtuser_3d::ScreenCamera(this);

		m_rootNode = new Qt3DCore::QNode();   //used for qnode
		m_cacheNode = new Qt3DCore::QNode(m_rootNode);

        m_modelEffect = qobject_cast<qtuser_3d::UEffect*>(EFFECTCREATE("modelwireframe.view.rt_pickFace.pick_zproject.alpha", m_rootNode));
		m_zProjectParameter = m_modelEffect->createParameter("pcolor", QVector4D(0.2275f, 0.2275f, 0.2353f, 0.5f));
		Qt3DRender::QRenderPass* pass = m_modelEffect->traitRenderPass("zproject.alpha");
		if (pass)
		{
			Qt3DRender::QBlendEquationArguments* blend = new Qt3DRender::QBlendEquationArguments(pass);
			blend->setSourceRgba(Qt3DRender::QBlendEquationArguments::SourceAlpha);
			blend->setDestinationRgba(Qt3DRender::QBlendEquationArguments::OneMinusSourceAlpha);
			pass->addRenderState(blend);

			Qt3DRender::QCullFace* cullFace = new Qt3DRender::QCullFace(pass);
			cullFace->setMode(Qt3DRender::QCullFace::Back);
			pass->addRenderState(cullFace);
		}

		m_modelEffectProxy = new ModelEffect(this);
		m_modelEffectProxy->setEffect(m_modelEffect);
		m_supportEffect = qobject_cast<qtuser_3d::UEffect*>(EFFECTCREATE("support_pickFaceFlag.pick", m_rootNode));
		m_modelEffectProxy->setSupportEffect(m_supportEffect);

		m_lineEffect = qobject_cast<qtuser_3d::UEffect*>(EFFECTCREATE("modelline", m_rootNode));
		m_lineEffectProxy = new ModelEffect(this);
		m_lineEffectProxy->setEffect(m_lineEffect);

		m_mainCamera->camera()->setParent(m_rootNode);

		m_effectParameterSetter = new EffectParameterSetter(this);
	}
	
	ReuseableCache::~ReuseableCache()
	{
		delete m_rootNode;
	}

	qtuser_3d::PrinterEntity* ReuseableCache::getCachedPrinterEntity()
	{
		return m_printerEntity;
	}

	void ReuseableCache::setPrinterEntity(qtuser_3d::PrinterEntity* printer)
	{
		m_printerEntity = printer;
		m_printerEntity->setParent(m_rootNode);
	}

	ModelEffect* ReuseableCache::modelEffectProxy()
	{
		return m_modelEffectProxy;
	}

	qtuser_3d::ScreenCamera* ReuseableCache::mainScreenCamera()
	{
		return m_mainCamera;
	}

	Qt3DRender::QEffect* ReuseableCache::getCachedModelEffect(QVector3D *minspace, QVector3D *maxspace)
	{
		if (minspace != nullptr && maxspace != nullptr)
		{
			m_modelEffectProxy->setSpaceBox(*minspace, *maxspace);
		}
		return m_modelEffect;
	}

	Qt3DRender::QEffect* ReuseableCache::getCachedSupportEffect()
	{
		return m_supportEffect;
	}

	Qt3DRender::QEffect* ReuseableCache::getCachedLineEffect()
	{
		return m_lineEffect;
	}

	void ReuseableCache::registerCache()
	{
		m_effectParameterSetter->setSupportEffect(m_supportEffect);

		PRIMITIVEROOT->setParent(m_rootNode);
		SHADERROOT->setParent(m_rootNode);
		RENDERPASSROOT->setParent(m_rootNode);
		EFFECTROOT->setParent(m_rootNode);
		registerResidentNode(m_rootNode);
	}

	void ReuseableCache::blockRelation()
	{
		EFFECTROOT->setParent((Qt3DCore::QNode*)nullptr);
		RENDERPASSROOT->setParent((Qt3DCore::QNode*)nullptr);
		PRIMITIVEROOT->setParent((Qt3DCore::QNode*)nullptr);
		SHADERROOT->setParent((Qt3DCore::QNode*)nullptr);
		m_mainCamera->camera()->setParent((Qt3DCore::QNode*)nullptr);
		unRegisterResidentNode(m_rootNode);
	}

	Qt3DCore::QNode* ReuseableCache::cacheNode()
	{
		return m_cacheNode;
	}

	void ReuseableCache::setZProjectColor(const QVector4D& color)
	{
		m_zProjectParameter->setValue(color);
	}
}

#ifndef CREATIVE_KERNEL_REUSEABLECACHE_1594457868780_H
#define CREATIVE_KERNEL_REUSEABLECACHE_1594457868780_H
#include "basickernelexport.h"
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QEffect>

namespace qtuser_3d
{
	class PrinterEntity;
	class ScreenCamera;
	class UEffect;
}

namespace creative_kernel
{
	class ModelEffect;
	class EffectParameterSetter;
	class BASIC_KERNEL_API ReuseableCache : public QObject
	{
		Q_OBJECT
	public:
		ReuseableCache(QObject* parent = nullptr);
		virtual ~ReuseableCache();

		qtuser_3d::PrinterEntity* getCachedPrinterEntity();
		void setPrinterEntity(qtuser_3d::PrinterEntity* printer);

		qtuser_3d::ScreenCamera* mainScreenCamera();
		Qt3DRender::QEffect* getCachedModelEffect(QVector3D *minspace = nullptr, QVector3D *maxspace = nullptr);
		Qt3DRender::QEffect* getCachedLineEffect();
		Qt3DRender::QEffect* getCachedSupportEffect();

		ModelEffect* modelEffectProxy();
		Qt3DCore::QNode* cacheNode();

		void setZProjectColor(const QVector4D& color);

		void registerCache();
		void blockRelation();
	protected:
		qtuser_3d::PrinterEntity* m_printerEntity;
		qtuser_3d::ScreenCamera* m_mainCamera;

		qtuser_3d::UEffect* m_modelEffect;
		Qt3DRender::QParameter* m_zProjectParameter;

		ModelEffect* m_modelEffectProxy;

		qtuser_3d::UEffect* m_lineEffect;
		ModelEffect* m_lineEffectProxy;

		Qt3DCore::QNode* m_rootNode;
		Qt3DCore::QNode* m_cacheNode;

		qtuser_3d::UEffect* m_supportEffect;
		EffectParameterSetter* m_effectParameterSetter;
	};
}
#endif // CREATIVE_KERNEL_REUSEABLECACHE_1594457868780_H
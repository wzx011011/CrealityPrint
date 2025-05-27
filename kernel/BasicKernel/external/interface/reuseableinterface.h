#ifndef CREATIVE_KERNEL_REUSEABLEINTERFACE_1594457885812_H
#define CREATIVE_KERNEL_REUSEABLEINTERFACE_1594457885812_H
#include "basickernelexport.h"
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QEffect>

namespace qtuser_3d
{
	class PrinterEntity;
	class ModelEffect;
}

namespace creative_kernel
{
	BASIC_KERNEL_API qtuser_3d::PrinterEntity* getCachedPrinterEntity();
	BASIC_KERNEL_API Qt3DRender::QCamera* getCachedCameraEntity();
	BASIC_KERNEL_API Qt3DRender::QEffect* getCachedModelEffect(QVector3D *minspace = nullptr, QVector3D *maxspace = nullptr);
	BASIC_KERNEL_API Qt3DRender::QEffect* getCachedLineEffect();
	BASIC_KERNEL_API Qt3DRender::QEffect* getCachedSupportEffect();


    BASIC_KERNEL_API void setModelEffectClipBottomZ(float bottomZ = 0.0f);
	BASIC_KERNEL_API void setModelEffectBottomZ(float bottomZ = 0.0f);

	BASIC_KERNEL_API void setModelEffectClipMaxZ(float z = 100000.0f);
	BASIC_KERNEL_API void setModelEffectBox(QVector3D& dmin, QVector3D& dmax);

	BASIC_KERNEL_API void cacheNode(Qt3DCore::QNode* node);
	BASIC_KERNEL_API void unCacheNode(Qt3DCore::QNode* node);

	BASIC_KERNEL_API void setZProjectColor(const QVector4D& color);
}
#endif // CREATIVE_KERNEL_REUSEABLEINTERFACE_1594457885812_H

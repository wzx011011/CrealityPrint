#include "reuseableinterface.h"
#include "external/kernel/kernel.h"
#include "external/kernel/reuseablecache.h"
#include "qtuser3d/camera/screencamera.h"
#include "external/data/modeleffect.h"
#include "qtuser3d/camera/screencamera.h"

namespace creative_kernel
{
	qtuser_3d::PrinterEntity* getCachedPrinterEntity()
	{
		return getKernel()->reuseableCache()->getCachedPrinterEntity();
	}

	Qt3DRender::QCamera* getCachedCameraEntity()
	{
		return getKernel()->reuseableCache()->mainScreenCamera()->camera();
	}

	Qt3DRender::QEffect* getCachedModelEffect(QVector3D *minspace, QVector3D *maxspace)
	{
		return getKernel()->reuseableCache()->getCachedModelEffect(minspace, maxspace);
	}

	Qt3DRender::QEffect* getCachedLineEffect()
	{
		return getKernel()->reuseableCache()->getCachedLineEffect();
	}

	Qt3DRender::QEffect* getCachedSupportEffect()
	{
		return getKernel()->reuseableCache()->getCachedSupportEffect();
	}
    //lisugui 2020-10-27
    void setModelEffectClipBottomZ(float bottomZ)
    {
        ModelEffect* effectProxy = getKernel()->reuseableCache()->modelEffectProxy();
        effectProxy->setVisibleBottomHeight(bottomZ);
    }

	void setModelEffectBottomZ(float bottomZ)
	{
		ModelEffect* effectProxy = getKernel()->reuseableCache()->modelEffectProxy();
		effectProxy->setBottom(bottomZ);
	}

	void setModelEffectClipMaxZ(float z)
	{
		ModelEffect* effectProxy = getKernel()->reuseableCache()->modelEffectProxy();
		effectProxy->setVisibleTopHeight(z);
	}
	
	void setModelEffectBox(QVector3D& dmin, QVector3D& dmax)
	{
		ModelEffect* effectProxy = getKernel()->reuseableCache()->modelEffectProxy();
		effectProxy->setSpaceBox(dmin, dmax);
	}

	void cacheNode(Qt3DCore::QNode* node)
	{
		if (node)
		{
			Qt3DCore::QNode* cacheNode = getKernel()->reuseableCache()->cacheNode();
			node->setParent(cacheNode);
		}
	}

	void unCacheNode(Qt3DCore::QNode* node)
	{
		if (node)
		{
			node->setParent((Qt3DCore::QNode*)nullptr);
		}
	}

	void setZProjectColor(const QVector4D& color)
	{
		getKernel()->reuseableCache()->setZProjectColor(color);
	}
}

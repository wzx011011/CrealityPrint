#ifndef CREATIVE_KERNEL_MODELEFFECT_1595134798238_H
#define CREATIVE_KERNEL_MODELEFFECT_1595134798238_H
#include "basickernelexport.h"
#include <Qt3DRender/QParameter>
#include <QtGui/QVector3D>

namespace qtuser_3d
{
	class UEffect;
}
namespace creative_kernel
{
	class BASIC_KERNEL_API ModelEffect : public QObject
	{
		Q_OBJECT
	public:
		ModelEffect(QObject* parent = nullptr);
		virtual ~ModelEffect();

		void setEffect(qtuser_3d::UEffect* effect);
		void setSupportEffect(qtuser_3d::UEffect* effect);

		void setSpaceBox(QVector3D minspace, QVector3D maxspace);
		void setBottom(float bottom);

		void setVisibleBottomHeight(float bottomHeight);
		void setVisibleTopHeight(float topHeight);

	protected:
		qtuser_3d::UEffect* m_effect;
		qtuser_3d::UEffect* m_supportEffect;

		Qt3DRender::QParameter* m_minSpaceParameter;
		Qt3DRender::QParameter* m_maxSpaceParameter;

		Qt3DRender::QParameter* m_bottomParameter;

		Qt3DRender::QParameter* m_visibleBottomHeightParameter;
		Qt3DRender::QParameter* m_visibleTopHeightParameter;
		Qt3DRender::QParameter* m_modelStateColors;

		Qt3DRender::QParameter* m_ambientColors;
		Qt3DRender::QParameter* m_diffuseColors;
		Qt3DRender::QParameter* m_specularColors;

		Qt3DRender::QParameter* m_supportminSpaceParameter;
		Qt3DRender::QParameter* m_supportmaxSpaceParameter;
								 
		Qt3DRender::QParameter* m_supportbottomParameter;
								  
		Qt3DRender::QParameter* m_supportvisibleBottomHeightParameter;
		Qt3DRender::QParameter* m_supportvisibleTopHeightParameter;
	};
}
#endif // CREATIVE_KERNEL_MODELEFFECT_1595134798238_H
#ifndef CREATIVE_KERNEL_EFFECTPARAMETERSETTER_1595558352007_H
#define CREATIVE_KERNEL_EFFECTPARAMETERSETTER_1595558352007_H
#include "basickernelexport.h"
#include <Qt3DRender/QParameter>

namespace qtuser_3d
{
	class UEffect;
}

namespace creative_kernel
{
	class BASIC_KERNEL_API EffectParameterSetter : public QObject
	{
		Q_OBJECT
	public:
		EffectParameterSetter(QObject* parent = nullptr);
		virtual ~EffectParameterSetter();

		void setSupportEffect(qtuser_3d::UEffect* effect);
	protected:
		qtuser_3d::UEffect* m_supportEffect;
	};
}
#endif // CREATIVE_KERNEL_EFFECTPARAMETERSETTER_1595558352007_H
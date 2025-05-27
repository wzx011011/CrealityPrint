#include "effectparametersetter.h"
#include "qtuser3d/effect/ueffect.h"

#include <QtGui/QVector4D>

namespace creative_kernel
{
	EffectParameterSetter::EffectParameterSetter(QObject* parent)
		:QObject(parent)
		, m_supportEffect(nullptr)
	{
	}
	
	EffectParameterSetter::~EffectParameterSetter()
	{
	}

	void EffectParameterSetter::setSupportEffect(qtuser_3d::UEffect* effect)
	{
		m_supportEffect = effect;

		if (m_supportEffect)
		{
			QVariantList values = QVariantList();
			values << QVector4D(0.0f, 0.3f, 0.0f, 0.0f)
				<< QVector4D(0.561f, 0.861f, 0.561f, 1.0f)
				<< QVector4D(0.462f, 0.963f, 0.976f, 1.0f)
				<< QVector4D(0.03f, 0.763f, 0.784f, 1.0f);
			m_supportEffect->createParameter("stateColors[0]", values);
		}
	}
}
#include "modeleffect.h"
#include "qtuser3d/effect/ueffect.h"
#include <QtGui/QVector4D>
#include "data/modelspace.h"
#include "external/interface/spaceinterface.h"
#include "interface/appsettinginterface.h"

namespace creative_kernel
{
	ModelEffect::ModelEffect(QObject* parent)
		:QObject(parent)
		, m_effect(nullptr)
		, m_minSpaceParameter(nullptr)
		, m_maxSpaceParameter(nullptr)
		, m_bottomParameter(nullptr)
		, m_visibleBottomHeightParameter(nullptr)
		, m_visibleTopHeightParameter(nullptr)
		, m_modelStateColors(nullptr)
	{
	}
	
	ModelEffect::~ModelEffect()
	{
	}

	void ModelEffect::setEffect(qtuser_3d::UEffect* effect)
	{
		m_effect = effect;

		if (m_effect)
		{
			m_minSpaceParameter = m_effect->createParameter("minSpace", QVector3D(0.0f, 0.0f, 0.0f));
			m_maxSpaceParameter = m_effect->createParameter("maxSpace", QVector3D(0.0f, 0.0f, 0.0f));
			m_bottomParameter = m_effect->createParameter("bottom", -100);

			m_visibleBottomHeightParameter = m_effect->createParameter("bottomVisibleHeight", -1.0f);
			m_visibleTopHeightParameter = m_effect->createParameter("topVisibleHeight", 1000.0f);

			QVariantList values = getShaderSetting("modeleffect_statecolors");
			m_modelStateColors = m_effect->createParameter("stateColors[0]", values);

			m_ambientColors = m_effect->createParameter("ambient", getSettingVector4D("modeleffect_ambient"));
			m_diffuseColors = m_effect->createParameter("diffuse", getSettingVector4D("modeleffect_diffuse"));
			m_specularColors = m_effect->createParameter("specular", getSettingVector4D("modeleffect_specular"));
		}
	}

	void ModelEffect::setSupportEffect(qtuser_3d::UEffect* effect)
	{
		m_supportEffect = effect;

		if (m_supportEffect)
		{
			m_supportminSpaceParameter = m_supportEffect->createParameter("minSpace", QVector3D(0.0f, 0.0f, 0.0f));
			m_supportmaxSpaceParameter = m_supportEffect->createParameter("maxSpace", QVector3D(0.0f, 0.0f, 0.0f));
			m_supportbottomParameter = m_supportEffect->createParameter("bottom", -100);

			m_supportvisibleBottomHeightParameter = m_supportEffect->createParameter("bottomVisibleHeight", -1.0f);
			m_supportvisibleTopHeightParameter = m_supportEffect->createParameter("topVisibleHeight", 1000.0f);
		}
	}

	void ModelEffect::setSpaceBox(QVector3D minspace, QVector3D maxspace)
	{
		QVector3D offset(0.1, 0.1, 0.1);
		if (m_minSpaceParameter)
		{
			m_minSpaceParameter->setValue(minspace - offset);
		}
		if (m_maxSpaceParameter)
		{
			m_maxSpaceParameter->setValue(maxspace + offset);
		}

		m_supportminSpaceParameter->setValue(minspace);
		m_supportmaxSpaceParameter->setValue(maxspace);
	}

	void ModelEffect::setBottom(float bottom)
	{
		if (m_bottomParameter)
		{
			m_bottomParameter->setValue(bottom);
		}

		m_supportbottomParameter->setValue(bottom);
	}

	void ModelEffect::setVisibleBottomHeight(float bottomHeight)
	{
		if(m_visibleBottomHeightParameter) m_visibleBottomHeightParameter->setValue(bottomHeight);
		m_supportvisibleBottomHeightParameter->setValue(bottomHeight);
	}

	void ModelEffect::setVisibleTopHeight(float topHeight)
	{
		if(m_visibleTopHeightParameter) m_visibleTopHeightParameter->setValue(topHeight);
		m_supportvisibleTopHeightParameter->setValue(topHeight);
	}
}
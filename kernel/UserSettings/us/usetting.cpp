#include "usetting.h"
namespace us
{
	USetting::USetting(const SettingItemDef* def, QObject* parent)
		:QObject(parent)
		, m_itemDef(def)
	{
		m_value = m_itemDef->defaultValue();
	}

	USetting::~USetting()
	{
	}

	USetting* USetting::clone()
	{
		USetting* setting = new USetting(m_itemDef, nullptr);
		setting->m_value = m_value;
		return setting;
	}

	void USetting::setValue(const QVariant& value)
	{
		m_value = value;
	}

	void USetting::setValue(const QString& str)
	{
		m_value = m_itemDef->value(str);
	}

	const QString& USetting::key() const
	{
		return m_itemDef->name;
	}

	const QVariant& USetting::value() const
	{
		return m_value;
	}

	const QString USetting::getValue()
	{
		return m_itemDef->getShowValue();
	}
}

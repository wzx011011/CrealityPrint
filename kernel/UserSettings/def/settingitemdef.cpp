#include "def/settingitemdef.h"
#include "def/metatype.h"

#include <QtCore/QDebug>
namespace us
{
	SettingItemDef::SettingItemDef(QObject* parent)
		:QObject(parent)
		, m_metaType(nullptr)
	{
		name = "";
		label = "";
		unit = "";
		description = "";
		type = "";
		defaultStr = "";
		minimum = "";
		maximum = "";
		valueStr = "";
		showValueStr = "";
		miniwarning = "";
		maxwarning = "";
		isCustomSetting = false;
		enableStatus = "true";
		classType = "";
		paramLevel = "3";
		order = 0;
	}

	SettingItemDef::~SettingItemDef()
	{
	}

	void SettingItemDef::process()
	{
		if (type == "str")
			m_metaType = &sStrType;
		else if (type == "float")
			m_metaType = &sFloatType;
		else if (type == "int")
			m_metaType = &sIntType;
		else if (type == "bool")
			m_metaType = &sBoolType;
		else if (type == "enum")
			m_metaType = &sEnumType;
		else if (type == "polygon")
			m_metaType = &sPolygonType;
		else if (type == "polygons")
			m_metaType = &sPolygonsType;
		else if (type == "optional_extruder")
			m_metaType = &sOptionalExtruderType;
		else if (type == "[int]")
			m_metaType = &sIntArrayType;
		else if (type == "extruder")
			m_metaType = &sExtruderType;

		if (m_metaType)
			m_value = m_metaType->value(defaultStr);
		else
		{
			qDebug() << "undefined setting type.";
		}
	}

	void SettingItemDef::addSettingItemDef(SettingItemDef* settingItemDef)
	{
		items.push_back(settingItemDef);
	}

	QVariant SettingItemDef::defaultValue() const
	{
		return m_value;
	}

	QString SettingItemDef::getName() const
	{
		return name;
	}

	QString SettingItemDef::getType() const
	{
		return type;
	}

	QString SettingItemDef::getlabel() const
	{
		return label;
	}

	QString SettingItemDef::getValue() const
	{
		if (name == "layer_height")
		{
			int i = 0;
		}
		if (valueStr != "" && isCustomSetting == false)
		{
			return valueStr;
		}
		else if (showValueStr != "")
		{
			return showValueStr;
		}
		else
		{
			return defaultStr;
		}
	}

	QString SettingItemDef::getSourceValue() const
	{
		if (valueStr != "")
		{
			return valueStr;
		}
		else
		{
			return defaultStr;
		}
	}

	QString SettingItemDef::getDefValue() const
	{
		return defaultStr;
	}

	QString SettingItemDef::getDescription() const
	{
		return description;
	}

	QString SettingItemDef::getUnit() const
	{
		return unit;
	}

	QString SettingItemDef::getShowValue() const
	{
		if (showValueStr.indexOf("eval") != -1)
		{
			return "";
		}
		else
		{
			return showValueStr;
		}
	}

	QString SettingItemDef::getEnableStatus() const
	{
		return enableStatus;
	}

	void SettingItemDef::setValue(QString key, QString value)
	{
		showValueStr = value;
	}

	QString SettingItemDef::getMax() const
	{
		if (maxwarning != "")
		{
			return maxwarning;
		}
		return maximum;
	}

	QString SettingItemDef::getMin() const
	{
		if (miniwarning != "")
		{
			return miniwarning;
		}
		return minimum;
	}

	QVariantMap SettingItemDef::getOptions(void)
	{
		QVariantMap dataMap;
		dataMap.clear();

		if (type == "optional_extruder" || type == "extruder")
		{//挤出机
			dataMap.insert(QString("-1"), QVariant::fromValue(QString("Not overridden")));
			dataMap.insert(QString("0"), QVariant::fromValue(QString("Extruder 1")));
			dataMap.insert(QString("1"), QVariant::fromValue(QString("Extruder 2")));

			return dataMap;
		}

		QMap<QString, QString>::const_iterator it;//迭代器的使用,it一开始指向的是第0个元素之前的位置
		for (it = options.constBegin(); it != options.constEnd(); ++it)
		{
			dataMap.insert(it.key(), QVariant::fromValue(it.value()));
		}

		return dataMap;
	}

	QVariant SettingItemDef::value(const QString& str) const
	{
		QVariant value = m_value;
		if (m_metaType) value = m_metaType->value(str);
		return value;
	}

	bool SettingItemDef::getIsCustomSetting() const
	{
		return isCustomSetting;
	}

	void SettingItemDef::setIsCustomSetting(bool value)
	{
		isCustomSetting = value;
	}

	QString SettingItemDef::getclasstypevalue() const
	{
		return classType;
	}

	QString SettingItemDef::getParamLevel() const
	{
		return paramLevel;
	}
}

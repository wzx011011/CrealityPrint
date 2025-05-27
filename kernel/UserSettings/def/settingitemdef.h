#ifndef _US_SETTINGITEMDEF_1589438667492_H
#define _US_SETTINGITEMDEF_1589438667492_H
#include "usersettingsexport.h"
#include <QtCore/QVariant>

namespace us
{
	class MetaType;
	class USER_SETTINGS_API SettingItemDef: public QObject
	{
	Q_OBJECT
	public:
		SettingItemDef(QObject* parent = nullptr);
		virtual ~SettingItemDef();

		void process();

		void addSettingItemDef(SettingItemDef* settingItemDef);
		QVariant defaultValue() const;

		Q_INVOKABLE QString getName() const;
		Q_INVOKABLE QString getType() const;
		Q_INVOKABLE QString getValue() const;
		Q_INVOKABLE QString getSourceValue() const;
		Q_INVOKABLE QString getDefValue() const;
		Q_INVOKABLE QString getShowValue() const;
		Q_INVOKABLE QString getMax() const;
		Q_INVOKABLE QString getMin() const;
		Q_INVOKABLE QVariantMap getOptions(void);
		Q_INVOKABLE QString getEnableStatus() const;

		Q_INVOKABLE void setValue(QString key, QString value);

		Q_INVOKABLE bool getIsCustomSetting() const;
		Q_INVOKABLE void setIsCustomSetting(bool value);

		Q_INVOKABLE QString getclasstypevalue() const;
		Q_INVOKABLE QString getParamLevel() const;
		Q_INVOKABLE QString getlabel() const;
		Q_INVOKABLE QString getDescription() const;
		Q_INVOKABLE QString getUnit() const;
		

		QVariant value(const QString& str) const;

		QList<SettingItemDef*> items;

		QString name;
		QString label;
		QString unit;
		QString description;
		QString type;
		QString defaultStr;
		QString minimum;
		QString miniwarning;
		QString maximum;
		QString maxwarning;
		QString valueStr;
		QString showValueStr;
		QString enableStatus;
		QString paramLevel;
		bool isCustomSetting;
		int order;
		QString classType;
		QMap<QString, QString> options;

	protected:
		QVariant m_value;
		MetaType* m_metaType;
	};
}
#endif // _US_SETTINGITEMDEF_1589438667492_H

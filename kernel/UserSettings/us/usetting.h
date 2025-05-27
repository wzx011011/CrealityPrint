#ifndef _US_USETTING_1589354685441_H
#define _US_USETTING_1589354685441_H
#include "usersettingsexport.h"
#include "def/settingitemdef.h"
#include <QVariant>

namespace us
{
	class USER_SETTINGS_API USetting: public QObject
	{
		Q_OBJECT
		Q_PROPERTY(QVariant value READ value WRITE setValue)
		Q_PROPERTY(QString key READ key CONSTANT)
	public:
		USetting(const SettingItemDef* def, QObject* parent = nullptr);
		virtual ~USetting();

		USetting* clone();
		const QString& key() const;
		void setValue(const QVariant& value);
		void setValue(const QString& str);
		const QVariant& value() const;
		const QString getValue();

	protected:
		const SettingItemDef* m_itemDef;
		QVariant m_value;
	};
}
#endif // _US_USETTING_1589354685441_H

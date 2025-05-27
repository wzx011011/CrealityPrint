#ifndef _US_USETTINGS_1589354685439_H
#define _US_USETTINGS_1589354685439_H
#include "usersettingsexport.h"
#include "us/usetting.h"
#include <QtCore/QMap>
#include <QtCore/QHash>
namespace us
{
	class USER_SETTINGS_API USettings: public QObject
	{
	public:
		USettings(QObject* parent = nullptr);
		virtual ~USettings();

		const QHash<QString, USetting*>& settings() const;

		void insert(USetting* setting);
		void merge(USettings* settings);
		void mergeNewItem(USettings* settings);
		void clear();
		void deleteValueByKey(QString key);

		void loadDefault(const QString& file);
        void save(const QString& file);
		void update(const USettings* settings);


		QString value(const QString& key, const QString& defaultValue) const;
	protected:
		QHash<QString, USetting*> m_hashsettings;
	};
}
#endif // _US_USETTINGS_1589354685439_H

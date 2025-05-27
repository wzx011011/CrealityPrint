#ifndef _US_DEFAULTLOADER_1589458301532_H
#define _US_DEFAULTLOADER_1589458301532_H
#include "usersettingsexport.h"
#include "cryptfiledevice.h"
namespace us
{
	class USettings;
	class USER_SETTINGS_API DefaultLoader: public QObject
	{
	public:
		DefaultLoader(QObject* parent = nullptr);
		virtual ~DefaultLoader();

		void loadDefault(const QString& file, USettings* uSettings);

	private:
		CryptFileDevice* m_pFile;
		USettings* m_defRecommendSetting;
		QString m_configRoot;
	};
}
#endif // _US_DEFAULTLOADER_1589458301532_H

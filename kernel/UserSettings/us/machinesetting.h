#ifndef _US_MACHINESETTING_1589879654001_H
#define _US_MACHINESETTING_1589879654001_H
#include "usersettingsexport.h"
#include "us/usettings.h"
namespace us
{
	class USER_SETTINGS_API MachineSetting: public USettings
	{
	public:
		MachineSetting(QObject* parent = nullptr);
		virtual ~MachineSetting();

		void getMachineSetting(QString name);//��ȡָ�����͵�Setting ����
	};
}
#endif // _US_MACHINESETTING_1589879654001_H

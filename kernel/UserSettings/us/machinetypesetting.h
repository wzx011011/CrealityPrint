#ifndef _US_MACHINETYPESETTING_1589879653959_H
#define _US_MACHINETYPESETTING_1589879653959_H
#include "usersettingsexport.h"
#include <unordered_map>
namespace us
{
	class USER_SETTINGS_API MachineTypeSetting: public QObject
	{
	public:
		MachineTypeSetting(QObject* parent = nullptr);
		virtual ~MachineTypeSetting();
		void LoadMachineType();//设置所有机型的列表
		std::unordered_map<std::string, std::string>& GetMachineType();
	protected:
		std::unordered_map<std::string, std::string> m_MachineType;
	};
}
#endif // _US_MACHINETYPESETTING_1589879653959_H

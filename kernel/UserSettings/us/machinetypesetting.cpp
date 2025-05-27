#include "machinetypesetting.h"
#include <QtCore/QFile>
#include"us/configurationmanager.h"
namespace us
{
	MachineTypeSetting::MachineTypeSetting(QObject* parent)
		:QObject(parent)
	{
        LoadMachineType();
	}
	MachineTypeSetting::~MachineTypeSetting()
	{
	}
	void MachineTypeSetting::LoadMachineType()
	{
        const QString MachineFile = (us::ConfigurationManager::instance().getConfigRoot() + "/default/MachineType.default");

        QFile file(MachineFile);
        bool opened = file.open(QIODevice::ReadOnly);
        QByteArray barray;
        if (opened == true)
        {
            char buffer[1];
            qint64 length = 0;
            int icount = 0;
            QByteArray b;
            std::string strkey;
            std::string Temp;
            std::string strvalue;
            //bool  isture=false;
            while ((length = file.read(buffer, 1) != 0))
            {
                if (buffer[0] != '\"' && buffer[0] != '=' && icount > 0)
                {
                    Temp += buffer[0];
                }
                if (buffer[0] == '\"')
                {
                    icount++;
                }
                if (icount == 2 && buffer[0] == '=')
                {
                    strkey = Temp;
                    Temp.clear();
                }
                if (4 == icount)
                {
                    strvalue = Temp;
                    icount = 0;
                    Temp.clear();
                    if (m_MachineType.find(strkey) != m_MachineType.end()) //Already exists.
                    {
                        m_MachineType[strkey] = strvalue;
                    }
                    else //New setting.
                    {
                        m_MachineType.emplace(strkey, strvalue);
                    }
                }
            }
        }
        file.close();
    }

    std::unordered_map<std::string, std::string>& MachineTypeSetting::GetMachineType()
    {
        return m_MachineType;
    }
}

#include "modeSettings.h"
#include <QtCore/QFile>
#include <QDir>
#include <QtCore/QTextStream>
namespace cxsw {
#define DEFAULTCONFIG "DefaultMeshData.ini"

    cxsw::ModeSettings::ModeSettings()
    {

    }

    cxsw::ModeSettings::~ModeSettings()
    {
    }

    void ModeSettings::add(const std::string& key, const std::string value)
    {
        if (defaultsettings.find(key) != defaultsettings.end()) //Already exists.
        {
            defaultsettings[key] = value;
        }
        else //New setting.
        {
            defaultsettings.emplace(key, value);
        }
    }

    std::unordered_map<std::string, std::string>& ModeSettings::GetSetting()
    {
        return defaultsettings;
    }

    void ModeSettings::LoadDefault()
    {
        const static QString prefix = QString(":/config/config/");
        std::string DefaultPathName = prefix.toStdString() + DEFAULTCONFIG;
        QFile file(DefaultPathName.c_str());
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
                    if (defaultsettings.find(strkey) != defaultsettings.end()) //Already exists.
                    {
                        defaultsettings[strkey] = strvalue;
                    }
                    else //New setting.
                    {
                        defaultsettings.emplace(strkey, strvalue);
                    }
                }
            }
        }
        file.close();
    }

}



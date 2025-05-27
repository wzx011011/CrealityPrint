#include "MeshDataListSettings.h"
#include <QtCore/QFile>
#include <QDir>
#include <QtCore/QTextStream>

namespace cxsw {
//#define DEFAULTCONFIG "DefaultMeshDataList.ini"
    const std::string DEFAULTCONFIG = "DefaultMeshDataList.ini";

    cxsw::MeshDataListSettings::MeshDataListSettings()
    {
    }

    cxsw::MeshDataListSettings::~MeshDataListSettings()
    {
    }



    void MeshDataListSettings::LoadDefault()
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
                    if (m_DefaultSettings.find(strkey) != m_DefaultSettings.end()) //Already exists.
                    {
                        m_DefaultSettings[strkey] = strvalue;
                    }
                    else //New setting.
                    {
                        m_DefaultSettings.emplace(strkey, strvalue);
                    }
                }
            }
        }
        file.close();
    }
    std::map<std::string, std::string>& MeshDataListSettings::GetSetting()
    {
        return m_DefaultSettings;
    }
}


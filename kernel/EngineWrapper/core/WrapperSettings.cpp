#include "WrapperSettings.h"
#include <QtCore/QFile>
#include <QDir>
#include <QtCore/QTextStream>

namespace cxsw {


    WrapperSettings::WrapperSettings()
    {
    }

    WrapperSettings::~WrapperSettings()
    {
    }

    void WrapperSettings::Init(Settings* settings)
    {
        LoadDefault();
        m_GlobalSettings.m_EditSettings = settings->GetEditSettings();
        m_GlobalSettings.DefaultSettings = m_DefaultSettings;

        //m_MeshDataListSettings.m_EditSettings = settings->GetEditSettings();
        //m_MeshDataListSettings.m_DefaultSettings = m_DefaultSettings;

    }


    void WrapperSettings::add(const std::string& key, const std::string& value)
    {
        if (m_DefaultSettings.find(key) != m_DefaultSettings.end()) //Already exists.
        {
            m_DefaultSettings[key] = value;
        }
        else //New setting.
        {
            m_DefaultSettings.emplace(key, value);
        }
    }

    void WrapperSettings::add(std::pair<std::string, std::string> pair_element)
    {
        if (m_DefaultSettings.find(pair_element.first) != m_DefaultSettings.end()) //Already exists.
        {
            m_DefaultSettings[pair_element.first] = pair_element.second;
        }
        else //New setting.
        {
            m_DefaultSettings.emplace(pair_element.first, pair_element.second);
        }
    }

    void WrapperSettings::LoadDefault()
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
}

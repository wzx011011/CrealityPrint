#include "def/defaultloader.h"
#include "us/usettings.h"

#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QFileInfo>

#include "def/settingdef.h"
#include"us/configurationmanager.h"

namespace us
{
	DefaultLoader::DefaultLoader(QObject* parent)
		:QObject(parent)
	{
		m_pFile = new CryptFileDevice();
		m_pFile->setPassword("8JGSEwfmq5hWKwwW4R-=[X9%OmOh-qwC");
		m_pFile->setSalt("rluH$Z+(ss+E]hms");
		m_configRoot = us::ConfigurationManager::instance().getConfigRoot();
		m_defRecommendSetting = new USettings();
	}
	DefaultLoader::~DefaultLoader()
	{
		if (m_defRecommendSetting != nullptr)
		{
			delete m_defRecommendSetting;
			m_defRecommendSetting = nullptr;
		}
	}

	void DefaultLoader::loadDefault(const QString& fileName, USettings* uSettings)
	{
		if (uSettings != NULL)
		{
			uSettings->clear();
		}
        
		if (fileName.indexOf("sliceconfig/default")>=0)
		{
			QFile file(fileName);
			if (file.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				while (!file.atEnd())
				{
					QString line = file.readLine();
					QStringList lists = line.split("=");
					if (lists.size() == 1)
					{
						USetting* setting = SETTING(lists.at(0).trimmed());
						uSettings->insert(setting);
					}
					else
					{
						USetting* setting = SETTING2(lists.at(0).trimmed(), lists.at(1).trimmed());
						uSettings->insert(setting);
					}
				}
			}
			file.close();
		}
		else //decrypt
		{
			m_pFile->setFileName(fileName);
			//QFile file(fileName);
			if (m_pFile->open(QIODevice::ReadOnly | QIODevice::Text))
			{
				while (!m_pFile->atEnd())
				{
					QByteArray tmp = m_pFile->readLine();
					QString line = QString(tmp);
					QStringList lists = line.split("=");
					if (lists.size() == 1)
					{
                        if (lists[0] != "\n")//empty
                        {
                            USetting* setting = SETTING(lists.at(0).trimmed());
                            uSettings->insert(setting);
                        }
					}
					else
					{
						lists[1].replace("\"","");
						USetting* setting = SETTING2(lists.at(0).trimmed(), lists.at(1).trimmed());
						uSettings->insert(setting);
					}
				}
			}
			m_pFile->close();


			//Optimize the configuration file loading method
			QFileInfo fileInfo(fileName);
			QString defFile = "";
			if (!fileInfo.fileName().isEmpty())
			{
				defFile = m_configRoot+"/default/" + fileInfo.fileName();
			}

			m_defRecommendSetting->clear();
			QFile tmpFile(defFile);
			if (tmpFile.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				while (!tmpFile.atEnd())
				{
					QString line = tmpFile.readLine();
					QStringList lists = line.split("=");
					if (lists.size() == 1)
					{
						USetting* setting = SETTING(lists.at(0).trimmed());
						m_defRecommendSetting->insert(setting);
					}
					else
					{
						USetting* setting = SETTING2(lists.at(0).trimmed(), lists.at(1).trimmed());
						m_defRecommendSetting->insert(setting);
					}
				}
			}
			tmpFile.close();
			uSettings->mergeNewItem(m_defRecommendSetting);
			//end
		}

	}
}

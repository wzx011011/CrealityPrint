#include "extruderprofile.h"
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include "us/slicermanager.h"
#include "def/cryptfiledevice.h"
ExtruderProfile * ExtruderProfile::instance()
{
    static ExtruderProfile s_profileManager;
    return &s_profileManager;
}
ExtruderProfile::ExtruderProfile(QObject* parent)
{
    //init 
    m_data.clear();
//    m_pFile = new QFile(this);
    m_pFile = new CryptFileDevice();
    m_pFile->setPassword("8JGSEwfmq5hWKwwW4R-=[X9%OmOh-qwC");
    m_pFile->setSalt("rluH$Z+(ss+E]hms");
    m_configRoot = us::ConfigurationManager::instance().getConfigRoot();
}

ExtruderProfile::~ExtruderProfile()
{
    if (m_pFile != nullptr)
    {
        delete m_pFile;
        m_pFile = nullptr;
    }
}
QString ExtruderProfile::getCurrentMachine()
{
    QString currentMachine = SlicerManager::instance().getCurrentMachine();
//	auto vh = SlicerManager::instance().globalsettings_modify()->settings().find("machine_name");
//	if (vh == SlicerManager::instance().globalsettings_modify()->settings().end())
//	{
//		qDebug() << "getSetVal failed !";
//		return"";
//	}
	QString strtmp = SlicerManager::instance().globalsettings_modify()->settings().value("machine_name")->value().toString();
    return currentMachine;
}

void ExtruderProfile::initFile(int nExtruder)
{
	QString Directory = getLocalPath();
    QString Machine = getCurrentMachine();
	QDir dir(Directory);
    if (!dir.exists())
    {
        mkMutiDir(Directory);
    }

    if (nExtruder ==1)
    {
        QString machine_extruder_file = m_configRoot + "/default/"+ Machine +"extruder.default";
        if (QFile::exists(machine_extruder_file))
        {
            copyFileToPath(machine_extruder_file, Directory + Machine + "_extruder_0.default");
        }
        else
        {
            copyFileToPath(m_configRoot + "/default/extruder.default", Directory + Machine + "_extruder_0.default");
        }
    }
    
    if (nExtruder == 2)
    {
        copyFileToPath(m_configRoot + "/default/extruder_0.default", Directory + Machine + "_extruder_0.default", true);
        copyFileToPath(m_configRoot + "/default/extruder_1.default", Directory + Machine + "_extruder_1.default");
    }
    

}

void ExtruderProfile::addExtruder(QString newName, QString oldName)
{
    QString newPath = getLocalPath()+ newName+".default";
    QString oldPath = getLocalPath() + oldName + ".default";
    copyFileToPath(oldPath, newPath);
}


void ExtruderProfile::readDataFromFile(int nExtruder)
{
//    setCurrentType(strPath);
    QString Directory = getLocalPath();
    initFile(nExtruder+1);
    QString Machine = getCurrentMachine();
    QString _strPath = Directory + Machine + "_extruder_" + QString::number(nExtruder) + ".default";
    m_pFile->setFileName(_strPath);
    if (!m_pFile->open(QIODevice::ReadOnly))
    {
        qDebug() << "open file failed";
        return;
    }
    m_data.clear();
    while (m_pFile->atEnd() == false) {
        QByteArray tmp = m_pFile->readLine();
        QString strtmp = QString(tmp);
        QStringList list = strtmp.split("=");
        if (list.size() > 1)
        {
			auto vh = m_data.find(list.at(0));
			if (vh == m_data.end())
			{
				m_data.insert(list.at(0), list.at(1).simplified());
			}
			else
			{
				m_data[list.at(0)] = list.at(1).simplified();
			}
            qDebug() << list.at(0) << list.at(1).simplified();
        }
    }
    m_pFile->close();
    if(nExtruder ==0)
    {
        m_tmpdata0 = m_data;
    }
    else if(nExtruder == 1)
    {
        m_tmpdata1 = m_data;
    }
    qDebug() << m_data.size();
}

void ExtruderProfile::setKeyValue(QString key,QString value,int nExtruder)
{
    if(nExtruder ==0)
    {
        m_tmpdata0.insert(key,value);
    }
    else if(nExtruder == 1)
    {
        m_tmpdata1.insert(key,value);
    }
}

bool ExtruderProfile::copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
{
    toDir.replace("\\", "/");
    if (sourceDir == toDir) {
        return true;
    }
    if (!QFile::exists(sourceDir)) {
        return false;
    }
    QDir createfile;
    bool exist = createfile.exists(toDir);
    if (exist) {
        if (coverFileIfExist) {
            createfile.remove(toDir);
        }
        else
            return true;
    }//end if
//     if (!QFile::copy(sourceDir, toDir))
//     {
//         return false;
//     }
    //read
	QByteArray all;
	if (sourceDir.indexOf("sliceconfig/default") >= 0)
	{
		QFile sourcefile(sourceDir);
		if (sourcefile.open(QIODevice::ReadOnly))
		{
			all = sourcefile.readAll();
			sourcefile.close();
		}
	}
	else
	{
		m_pFile->setFileName(sourceDir);
		//QFile sourcefile(sourceDir);
		if (m_pFile->open(QIODevice::ReadOnly))
		{
			all = m_pFile->readAll();
			m_pFile->close();
		}
	}

	m_pFile->setFileName(toDir);
	if (m_pFile->open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		m_pFile->write(all);
		m_pFile->close();
	}

    QFile(toDir).setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    return true;
}

QString ExtruderProfile::mkMutiDir(const QString path)
{
    QDir dir(path);
    if (dir.exists(path)) {
        return path;
    }
    QString parentDir = mkMutiDir(path.mid(0, path.lastIndexOf('/')));
    QString dirname = path.mid(path.lastIndexOf('/') + 1);
    QDir parentPath(parentDir);
    if (!dirname.isEmpty())
        parentPath.mkpath(dirname);
    return parentDir + "/" + dirname;
}

QHash<QString, QString> ExtruderProfile::getData()
{
    return m_data;
}

bool ExtruderProfile::delFile(const QString& path)
{
	QFile file(path);
	if (file.exists())
	{
		if (file.remove())
		{
			return true;
		}
		else
		{
			qDebug() << "delete file fail";
			return false;
		}
	}
	return  false;
}

QString ExtruderProfile::getLocalPath()
{
     return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Extruder/";
}

QString ExtruderProfile::getValue(QString key,int nExtruder)
{
    if(nExtruder == 0)return m_tmpdata0.value(key);
    else if(nExtruder == 1)return m_tmpdata1.value(key);
    return m_data.value(key);
}

bool ExtruderProfile::deletefile()
{
    QString Machine = getCurrentMachine();
    delFile(getLocalPath() + Machine + "_extruder_0" + ".default");
    delFile(getLocalPath() + Machine + "_extruder_1" + ".default");
    return true;
}
void ExtruderProfile::getALLExtruderfile(std::map<QString,QString>& value,int nExtruder)
{
    value.clear();
    QString strPara1 = "machine_extruder_start_code,machine_extruder_end_code";
    QString strPara = strPara1;
    QStringList list = strPara.split(",");
    readDataFromFile(nExtruder);
    for(auto val: list)
    {
        value.insert(std::make_pair(val,getValue(val,nExtruder)));
    }
}
void ExtruderProfile::saveExtruderData(int nExtruder)
{
    QString path ;
    QString Machine = getCurrentMachine();
    path = getLocalPath() + Machine + "_extruder_" + QString::number(nExtruder) + ".default";
    QHash<QString, QString> tmpHash;
    initFile();
    if(nExtruder==0)
    {
        tmpHash = m_tmpdata0;
    }
    else if(nExtruder ==1){
        tmpHash = m_tmpdata1;
    }

    m_pFile->setFileName(path);

    QStringList allvalue;
    if (m_pFile->open(QIODevice::ReadOnly))
    {
        while (m_pFile->atEnd() == false) {
            QByteArray tmp = m_pFile->readLine();
            QString strtmp = QString(tmp);
            QStringList list = strtmp.split("=");
            if (list.size() > 1)
            {
                //qDebug() << list.at(0) << list.at(1).simplified();
                auto vh = tmpHash.find(list.at(0));
                if (vh == tmpHash.end())
                {
                    qDebug()<<list.at(0)<<" filed";
                    continue;
                }
                else
                {
                    QString value =  tmpHash.value(list.at(0));
                    strtmp = list.at(0) + "=" + value;
                }
                strtmp = strtmp.replace("\n","\\n");
                strtmp += "\n";
                allvalue.append(strtmp);
            }
        }
        m_pFile->close();
    }
    else
    {
        qDebug() << "open file failed";
        m_pFile->close();
    }

    m_pFile->setFileName(path);
    if (!m_pFile->open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "write file failed";
        return;
    }

    for (auto val : allvalue)
    {
        m_pFile->write(val.toUtf8());
    }

    m_pFile->close();
}

//void ExtruderProfile::writeDataToFile(QString key, QString value,int nExtruder)
//{
//    QString path ;
//    QString Machine = getCurrentMachine();
//    path = getLocalPath() + Machine + "_extruder_" + QString::number(nExtruder) + ".default";
//    QHash<QString, QString> tmpHash;
//    if(nExtruder==0)
//    {
//        tmpHash = m_tmpdata0;
//    }
//    else if(nExtruder ==1){
//        tmpHash = m_tmpdata1;
//    }

//    m_pFile->setFileName(path);

//    QStringList allvalue;
//    if (m_pFile->open(QIODevice::ReadOnly))
//    {
//		while (m_pFile->atEnd() == false) {
//			QByteArray tmp = m_pFile->readLine();
//			QString strtmp = QString(tmp);
//			QStringList list = strtmp.split("=");
//			if (list.size() > 1)
//			{
//				//qDebug() << list.at(0) << list.at(1).simplified();

//				if (list.at(0) == key)
//				{
//					strtmp = list.at(0) + "=" + value + "\n";

//					auto vh = m_data.find(list.at(0));
//					if (vh == m_data.end())
//					{
//						m_data.insert(list.at(0), list.at(1).simplified());
//					}
//					else
//					{
//						m_data[list.at(0)] = value;
//					}
//				}
//				else
//				{
//					strtmp = list.at(0) + "=" + list.at(1);
//				}
//				allvalue.append(strtmp);
//			}
//		}
//        m_pFile->close();
//    }
//    else
//    {
//        qDebug() << "open file failed";
//        m_pFile->close();
//    }
    

//    if (allvalue.isEmpty())
//    {
//        QString strtmp = key + "=" + value + "\n";
//        allvalue.append(strtmp);
//    }
//    m_pFile->setFileName(path);
//    if (!m_pFile->open(QIODevice::WriteOnly))
//    {
//        qDebug() << "write file failed";
//        return;
//    }

//    for (auto val : allvalue)
//    {
//        m_pFile->write(val.toUtf8());
//    }

//    m_pFile->close();
//}


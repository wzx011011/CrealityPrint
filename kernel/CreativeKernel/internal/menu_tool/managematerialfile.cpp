#include "managematerialfile.h"
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include "us/slicermanager.h"
#include "us/usetting.h"

using namespace  us;
MangeMaterialFile::MangeMaterialFile(QObject* parent)
    :m_currentType("supportL")
{
    //init 
    m_data.clear();
    //readDataFromFile(getLocalPath() + "supportL.default");
    //readDataFromFile(getLocalPath() + "supportAuto.default");
    m_pFile = new CryptFileDevice();
    m_pFile->setPassword("8JGSEwfmq5hWKwwW4R-=[X9%OmOh-qwC");
    m_pFile->setSalt("rluH$Z+(ss+E]hms");
    m_configRoot = us::ConfigurationManager::instance().getConfigRoot();
}

MangeMaterialFile::~MangeMaterialFile()
{
    if (m_pFile != nullptr)
    {
        delete m_pFile;
        m_pFile = nullptr;
    }
}

QStringList MangeMaterialFile::getAllMaterialName()
{
//    qDebug() << "getAllMaterialName" ;
    QStringList list;
    QFileInfoList listfile;
	QDir dir(getLocalPath());

	if (!dir.exists())
		return list;

	dir.setFilter(QDir::Files);
	dir.setSorting(QDir::Time);

    listfile = dir.entryInfoList();
    QString Machine = getCurrentMachine();
    for (auto name: listfile)
    {
//        qDebug()<< Machine << Machine;
        QString strtmp = getMaterialType(name.baseName());
        QString strRet = "";
		if (strtmp.indexOf("Castable") >= 0
			|| strtmp.indexOf("Normal") >= 0)
		{
			strRet = "DLP";
		}
		else
		{
			strRet = "FDM";
		}
        if (Machine == strRet)
        {
            list.append(name.baseName());
        }
    }

	return list;
}

QString MangeMaterialFile::getCurrentMachine()
{
	auto vh = SlicerManager::instance().globalsettings_modify()->settings().find("machine_name");
	if (vh == SlicerManager::instance().globalsettings_modify()->settings().end())
	{
		qDebug() << "getSetVal failed !";
		return"";
	}
	QString strtmp = SlicerManager::instance().globalsettings_modify()->settings().value("machine_name")->value().toString();
	if (strtmp.indexOf("LD") >= 0)
	{
        return "DLP";
	}
	else
	{
        return "FDM";
	}
}

void MangeMaterialFile::initFile()
{
	QString Directory = getLocalPath();
	QDir dir(Directory);
    if (!dir.exists())
    {
        mkMutiDir(Directory);
        QString srcDir = m_configRoot + "/default/Material/";
        dir.setPath(m_configRoot + "/default/Material/");
        auto fileNames = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
        for (auto file : fileNames)
        {
            auto filename = file.fileName();
            copyFileToPath(srcDir + "/" + file.fileName(), Directory + "/" + file.fileName(), false);
        }
    }
}

void MangeMaterialFile::addMaterial(QString newName, QString oldName)
{
    QString newPath = getLocalPath()+ newName+".default";
    QString oldPath = getLocalPath() + oldName + ".default";
    copyFileToPath(oldPath, newPath);
}

QString MangeMaterialFile::getMaterialType(const QString path)
{
	QString strRet = "";
	//file does not exist,create it!
	QString _path = getLocalPath() + path + ".default";

    m_pFile->setFileName(_path);
    //CryptFileDevice* pFile = new CryptFileDevice(_path, "8JGSEwfmq5hWKwwW4R-=[X9%OmOh-qwC", "rluH$Z+(ss+E]hms");
	//QFile file(_path);
// 	if (!pFile->exists())
// 	{
// 		qDebug() << "source file not existed!";
// 		return strRet;
// 	}
	//open file, read a line ,check type
	if (!m_pFile->open(QIODevice::ReadOnly))
	{
		qDebug() << "open file failed";
		return strRet;
	}
	QByteArray tmp = m_pFile->readLine();
	QString strtmp = QString(tmp);
	QStringList list = strtmp.split("=");
    if (list.size() == 1)
    {
        strRet = list[0].simplified();
    }
    else if (list.size() > 1)
    {
        strRet = list[1].simplified();
    }

    m_pFile->close();
	return strRet;
}

void MangeMaterialFile::readDataFromFile(QString strPath)
{
    setCurrentType(strPath);
    QString Directory = getLocalPath();
    initFile();

    QString _strPath = Directory + strPath + ".default"; 

    m_pFile->setFileName(_strPath);
    //CryptFileDevice pFile(_strPath, "8JGSEwfmq5hWKwwW4R-=[X9%OmOh-qwC", "rluH$Z+(ss+E]hms");
    //QFile file(_strPath);
    if (!m_pFile->open(QIODevice::ReadOnly))
    {
        qDebug() << "open file failed";
        return;
    }

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

    qDebug() << m_data.size();
}

bool MangeMaterialFile::copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist/*=true*/)
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

QString MangeMaterialFile::mkMutiDir(const QString path)
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

QHash<QString, QString> MangeMaterialFile::getData()
{
    return m_data;
}

void MangeMaterialFile::resetType()
{
	QString Directory = getLocalPath()+ getCurrentType()+".default";
    qDebug() << Directory;
    qDebug() << "dir.exists"<< getMaterialType(getCurrentType());
	QFileInfo file(Directory);
	if (file.exists())
	{
		qDebug() << "dir.exists";
        delFile(Directory + getCurrentType() + ".default");
		copyFileToPath(m_configRoot +"/default/"+ getMaterialType(getCurrentType())+ ".default", Directory);
		m_data.clear();
		readDataFromFile(getCurrentType());
    }
    else
    {
        qDebug() << "dir. not exists";
    }  
}

bool MangeMaterialFile::delFile(const QString& path)
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

QString MangeMaterialFile::getLocalPath()
{
     return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Materials/";
}

QString MangeMaterialFile::getValue(QString key)
{
    return m_data.value(key);
}

void MangeMaterialFile::setCurrentType(QString type)
{
    m_currentType= type;
}

QString MangeMaterialFile::getCurrentType()
{
    return m_currentType;
}

bool MangeMaterialFile::deletefile()
{
    if ("Normal" == getCurrentType()
        || "Castable" == getCurrentType()
        || "PLA" == getCurrentType()
        || "ABS" == getCurrentType())
    {
        return false;
    }

    delFile(getLocalPath() + m_currentType + ".default");
    return true;
}

void MangeMaterialFile::writeDataToFile(QString key, QString value, bool bAutoSupport)
{
    QString path ;
    path = getLocalPath() + m_currentType + ".default";

    m_pFile->setFileName(path);
    //CryptFileDevice pFile(path, "8JGSEwfmq5hWKwwW4R-=[X9%OmOh-qwC", "rluH$Z+(ss+E]hms");
    //QFile file(path);

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

				if (list.at(0) == key)
				{
					strtmp = list.at(0) + "=" + value + "\n";
//                    auto vh = SlicerManager::instance().globalsettings_modify()->settings().find();
                    auto vh = m_data.find(list.at(0));
					if (vh == m_data.end())
					{
						m_data.insert(list.at(0), list.at(1).simplified());
					}
					else
					{
						m_data[list.at(0)] = value;
					}

                    us::GlobalSetting *mysetting = SlicerManager::instance().globalsettings_modify();
                //    mysetting->value("material_type",value);
                    QHash<QString,us::USetting *>  tt = mysetting->settings();
                    us::USetting *pp = static_cast<us::USetting *>(tt.find("material_type").value());
                    pp->setValue(value);
				}
				else
				{
					strtmp = list.at(0) + "=" + list.at(1);
				}
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
    

    if (allvalue.isEmpty())
    {
        QString strtmp = key + "=" + value + "\n";
        allvalue.append(strtmp);
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
        //file.write("\n");
    }

    m_pFile->close();
}


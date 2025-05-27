#include "parameterfile.h"

#include <QDebug>
//#include "us/profile.h"
#include <QStandardPaths>
#include "us/slicermanager.h"
#include "us/profilemanager.h"
#include"../../buildinfo.h"
#include<QCoreApplication>
#include<QDir>
#include"us/configurationmanager.h"
#include"ccglobal/log.h"

ParameterFile::ParameterFile(const QString& configRoot)
{
    m_pFile = new CryptFileDevice();
    m_pFile->setPassword("8JGSEwfmq5hWKwwW4R-=[X9%OmOh-qwC");
    m_pFile->setSalt("rluH$Z+(ss+E]hms");
	m_config_root = configRoot;


    QString str1= tr("high");
    QString str2= tr("middle");
    QString str3 = tr("low");

    QString m_strHigh = tr("Dynamic");
    QString m_strMiddle = tr("Balanced");
    QString m_strLow = tr("Speedy");
	
	//m_pCurrentProfileModel = new ProfileListModel();
}

ParameterFile::~ParameterFile()
{

}

void ParameterFile::updateProfileLayerHeight(QString name)
{
	ProfileManager::instance().updateProfileLayerHeight(name);
}

void ParameterFile::updateProfileByMachineName(QString name)
{
	QList<Profile*> profiles;
	//m_pCurrentProfileModel->clean();
	//ProfileManager::instance().cleanCurrentProfileModel();

	//add default
	//if (name.indexOf("LD") < 0)
	//{
	//	profiles = getDefaultFDMProfiles(name);
	//	//profiles = getDefaultDLPProfiles(name);
	//}
	//else {
	//	profiles = getDefaultFDMProfiles(name);
	//}

	//profiles = getDefaultFDMProfiles(name);

	//profiles文件夹的文件拷贝到slice
	copyBaseQuality(name);
	QFileInfoList filelistt = getFileName(name);

	//copyBaseQuality(name);

	//ProfileManager::instance().setProfiles(profiles);
	ProfileManager::instance().createNewFiles(filelistt);
}

QString ParameterFile::getCoachPath(int type /*= ProfileType::PROFILE*/)
{
	QString Directory;
	if (type == ProfileType::MATERIAL)
	{
		Directory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Materials";

	}
	else if (type == ProfileType::MACHINE)
	{
		Directory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Machines";
	}
	else
	{
		Directory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Profiles";
	}

	QDir dir(Directory);
	if (!dir.exists())
	{
		mkMutiDir(Directory);
		if (type == ProfileType::MATERIAL)
		{
			dir.setPath(m_config_root + "/default/Material/");
			QStringList fileNames = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
			for (auto fileName : fileNames)
			{
				copyFileToPath(m_config_root + "/default/Material/" + fileName, Directory + "/" +fileName);
			}
		}
	}

	return dir.absolutePath() + "/";
}

void ParameterFile::copyBaseQuality(QString name)
{
	//Create folder
//QString currentMachine=getcurrentProfileName();
	const QString SETTINGPATH = getCoachPath();
	QString strPath = SETTINGPATH + name;
	QDir dir;
	if (!dir.exists(strPath))
	{
		dir.mkdir(strPath);
	}
	QString srcDir = m_config_root + "/default/" + name;
	dir.setPath(srcDir);
	if (dir.exists())
	{
		for (auto file : dir.entryInfoList())
		{
			auto filename = file.fileName();
			if (filename == "." || filename == "..")
			{
				continue;
			}
			copyFileToPath(srcDir + "/" + file.fileName(), strPath + "/" + file.fileName(), false);
		}
	}
	//if (getMachineType(name) == "DLP")
	//{
	//	copyFileToPath(":/us/default/high-dlp.default", strPath + "/high.default");
	//	copyFileToPath(":/us/default/middle-dlp.default", strPath + "/middle.default");
	//	copyFileToPath(":/us/default/low-dlp.default", strPath + "/low.default");
	//}
	else
	{
		QString highProfilePath = m_config_root+"/default/" + name + "/high.default";
		QString middleProfilePath = m_config_root + "/default/" + name + "/middle.default";
		QString lowProfilePath = m_config_root + "/default/" + name + "/low.default";
		/////high profile
		if (QFile::exists(highProfilePath)) {
			if (!QFile::exists(strPath + "/high.default")) {
				copyFileToPath(highProfilePath, strPath + "/high.default");
			}
			else {
				QFileInfo info(strPath + "/high.default");
				qint64 fileSize = info.size();
				if (fileSize < 10000)
				{
					copyFileToPath(highProfilePath, strPath + "/high.default");
				}
			}
		}
		else
		{
			if (!QFile::exists(strPath + "/high.default")) {
				copyFileToPath(m_config_root +"/default/high.default", strPath + "/high.default");
			}
			else {
				QFileInfo info(strPath + "/high.default");
				qint64 fileSize = info.size();
				//当判断配置文件小于10k时，认为是推荐原文件，则拷贝覆盖
				//反之则是用户保存文件，不做修改
				if (fileSize < 10000)
				{
					copyFileToPath(m_config_root + "/default/high.default", strPath + "/high.default");
				}
			}
		}
		//////middle profile
		if (QFile::exists(middleProfilePath)) {
			if (!QFile::exists(strPath + "/middle.default")) {
				copyFileToPath(middleProfilePath, strPath + "/middle.default");
			}
			else {
				QFileInfo info(strPath + "/middle.default");
				qint64 fileSize = info.size();
				if (fileSize < 10000)
				{
					copyFileToPath(middleProfilePath, strPath + "/middle.default");
				}
			}
		}
		else
		{
			if (!QFile::exists(strPath + "/middle.default")) {
				copyFileToPath(m_config_root + "/default/middle.default", strPath + "/middle.default");
			}
			else {
				QFileInfo info(strPath + "/middle.default");
				qint64 fileSize = info.size();
				if (fileSize < 10000)
				{
					copyFileToPath(m_config_root + "/default/middle.default", strPath + "/middle.default");
				}
			}
		}
		//////low profile
		if (QFile::exists(lowProfilePath)) {
			if (!QFile::exists(strPath + "/low.default")) {
				copyFileToPath(lowProfilePath, strPath + "/low.default");
			}
			else {
				QFileInfo info(strPath + "/low.default");
				qint64 fileSize = info.size();
				if (fileSize < 10000)
				{
					copyFileToPath(lowProfilePath, strPath + "/low.default");
				}
			}
		}
		else
		{
			if (!QFile::exists(strPath + "/low.default")) {
				copyFileToPath(m_config_root + "/default/low.default", strPath + "/low.default");
			}
			else {
				QFileInfo info(strPath + "/low.default");
				qint64 fileSize = info.size();
				if (fileSize < 10000)
				{
					copyFileToPath(m_config_root + "/default/low.default", strPath + "/low.default");
				}
			}
		}
	}
}

void ParameterFile::copyBaseMachine(QString name, bool isCover)
{
	const QString SETTINGPATH = getCoachPath(ProfileType::MACHINE);
	QString strPath = SETTINGPATH + name + ".default";
	QDir dir;

	if (dir.exists(strPath) && !isCover)
		return;

	copyFileToPath(m_config_root + "/default/" + name + ".default", strPath);

	//if (isCover)
	//{
	//	copyFileToPath(m_config_root + "/default/" + name + ".default", strPath);
	//}
	//else if (!dir.exists(strPath)) //去掉判断条件，在设备管理页面点击重置的时候需要拷贝出厂配置文件到Machines下面
	//{
	//	copyFileToPath(m_config_root + "/default/" + name +".default", strPath);
	//}
}

QString ParameterFile::getMachineType(const QString path)
{
	QString strRet = "";

	if (path.isEmpty())
	{
		qDebug() << "path is empty";
		return strRet;
	}
	//file does not exist,create it!
	QString _path = getCoachPath(ProfileType::MACHINE) + path + ".default";

	m_pFile->setFileName(_path);
	//QFile file(_path);
	if (!m_pFile->exists())
	{
		if (!copyFileToPath(m_config_root + "/default/"+ path+ ".default", _path))
		{
			qDebug() << "source file not existed!";
			return strRet;
		}
	}
	//open file, read a line ,check type
	if (!m_pFile->open(QIODevice::ReadOnly))
	{
		qDebug() << "open file failed"<<m_pFile->errorString();
		return strRet;
	}
    
	QByteArray tmp = m_pFile->readLine();
	QString strtmp = QString(tmp);
	//QStringList list = strtmp.split("=");
	if (strtmp.indexOf("LD") >= 0)
	{
		strRet = "DLP";
	}
	else
	{
		strRet = "FDM";
	}

	m_pFile->close();
	return strRet;
}

QStringList ParameterFile::getAllfile(QString name, int type /*= ProfileType::PROFILE*/)
{
	QStringList fileName;
	QFileInfoList filelist = getFileName(name, type);
    QString m_strHigh = tr("Dynamic");
    QString m_strMiddle = tr("Balanced");
    QString m_strLow = tr("Speedy");
	QString m_strBest = tr("Best quality");
    qDebug()<<"getAllfile";
	for (auto file : filelist)
	{
//        if(file.baseName() == "high")
//        {
//            fileName.append(m_strHigh);
//        }
//        else if(file.baseName() == "middle")
//        {
//            fileName.append(m_strMiddle);
//        }
//        else if(file.baseName() == "low")
//        {
//            fileName.append(m_strLow);
//        }
//        else
//        {
            fileName.append(file.baseName());
//        }
     //
	}

	if (type == ProfileType::MATERIAL)
	{
		QString fdmMaterials = "PLA,ABS,PC";
		QString dlpMaterials = "Normal,Castable";
		QStringList list;
		list = getMaterialType(getMachineType(name));
		// 		if (getMachineType(name) == "FDM")
		// 		{
		// 			list = dlpMaterials.split(",");
		// 		}
		//         else
		//         {
		//             list = fdmMaterials.split(",");
		//         }
		for (auto val : list)
		{
			//remove material
			if (fileName.contains(val))
			{
				fileName.removeOne(val);
			}
		}
	}

	return  fileName;
}

// QString ParameterFile::getcurrentProfileName()
// {
// 	Profile* profile = m_pCurrentProfileModel->selectProfile();
// 	return profile->getName();
// }

void ParameterFile::addNewProfile(QString newProfile, QString printer, QString material, QString oldProfile)
{
	QString Machine = SlicerManager::instance().getCurrentMachine();
	const QString SETTINGPATH = getCoachPath();
	QString strPath = SETTINGPATH + printer + "/" + oldProfile + ".default";
	//"USER"字符串作为判断配置文件是自定义还是默认的，不会显示在页面
	QString strPathnew = SETTINGPATH + Machine + "/" + material + "_" + "USER" + newProfile + ".default";
	QDir dir;
	if (!dir.exists(strPath))
	{
		delDir(strPath);
	}
	copyFileToPath(strPath, strPathnew);
	QString strMaterialPath = getCoachPath(ProfileType::MATERIAL);
	us::ConfigurationManager::instance().setProfilePath(strMaterialPath);
	SlicerManager::instance().setCurrentMaterial(material);
	QString strProfilePath = SETTINGPATH + Machine + "/";
	us::ConfigurationManager::instance().setProfilePath(strProfilePath);
}

// int ParameterFile::setNewProfileIndex(QString name)
// {
// 	return m_pCurrentProfileModel->setNewProfileIndex(name);
// }

void ParameterFile::saveProfile()
{
	QString currentMachine = SlicerManager::instance().getCurrentMachine();
	QString strCurrentQuality = ProfileManager::instance().getcurrentProfileName();
	QString path = getCoachPath() + currentMachine + "/" + strCurrentQuality + ".default";

	QStringList allvalue;
	QHash<QString, USetting*> globalsettingModify = SlicerManager::instance().globalsettings_modify()->settings();
	for (const auto val : globalsettingModify)
	{
		QString keyValue = val->key();
		QString value;

		if ("layer_height" == keyValue
			|| "line_width" == keyValue || "min_skin_width_for_expansion" == keyValue || "infill_wipe_dist" == keyValue)
		{
			value = QString("%1").arg(globalsettingModify.value(keyValue)->value().toDouble());
		}
		if ("gcode_start" == keyValue
			|| "inter_layer" == keyValue
			|| "gcode_end" == keyValue
			|| "machine_start_gcode" == keyValue
			|| "machine_end_gcode" == keyValue)
		{
			QString str = globalsettingModify.value(keyValue)->value().toString();
			value = str.replace("\n", "\\n");
		}
		else
		{
			value = globalsettingModify.value(keyValue)->getValue();
			if (value == "")
			{
				value = globalsettingModify.value(keyValue)->value().toString();
			}
		}

		QString strtmp = keyValue + "=" + value;
		allvalue.append(strtmp);
	}

    //delFileAbsolute(path);
	m_pFile->setFileName(path);

	if (!m_pFile->open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		qDebug() << "write file failed";
		return;
	}

	for (auto val : allvalue)
	{
		m_pFile->write(val.toUtf8());
		m_pFile->write("\n");
	}

	m_pFile->close();
}

QMap<QString, QString> ParameterFile::getSettingFile()
{
	QMap<QString, QString> mapSettting;
	QString currentMachine = SlicerManager::instance().getCurrentMachine();
	QString strCurrentQuality = ProfileManager::instance().getcurrentProfileName();
	QString path = getCoachPath() + currentMachine + "/" + strCurrentQuality + ".default";

	m_pFile->setFileName(path);
	//QFile file(path);
	if (!m_pFile->open(QIODevice::ReadOnly))
	{
		qDebug() << "open file failed";
		return mapSettting;
	}
	QStringList allvalue;

	while (m_pFile->atEnd() == false) {
		QByteArray tmp = m_pFile->readLine();
		QString strtmp = QString(tmp);

		QStringList list = strtmp.split("=");
		if (list.size() > 0)
		{
			if (list.at(0).size() > 0)
			{
				QString value;
				QHash<QString, USetting*> tt = SlicerManager::instance().globalsettings_modify()->settings();
				auto vh = SlicerManager::instance().globalsettings_modify()->settings().find(list.at(0));
				if (vh == SlicerManager::instance().globalsettings_modify()->settings().end())
				{
					qDebug() << "getSetVal failed !" + list.at(0) + "######";
					continue;
				}

				if ("layer_height" == list.at(0)
					|| "line_width" == list.at(0) || "min_skin_width_for_expansion" == list.at(0) || "infill_wipe_dist" == list.at(0))
				{
					value = QString("%1").arg(SlicerManager::instance().globalsettings_modify()->settings().value(list.at(0))->value().toDouble());
				}
				if ("gcode_start" == list.at(0)
					|| "inter_layer" == list.at(0)
					|| "gcode_end" == list.at(0))
				{
					QString str = SlicerManager::instance().globalsettings_modify()->settings().value(list.at(0))->value().toString();
					value = str.replace("\n", "\\n");
				}
				else
				{
					value = SlicerManager::instance().globalsettings_modify()->settings().value(list.at(0))->value().toString();
				}

				mapSettting.insert(list.at(0), value);
			}
		}
	}
	m_pFile->close();

	return mapSettting;
}

bool ParameterFile::delFile(const QString& path)
{
	if ("low" == path
		|| "middle" == path
		|| "high" == path)
	{
		return false;
	}
	QString strPath = getCoachPath() + SlicerManager::instance().getCurrentMachine() + "/" + path + ".default";
	QFile file(strPath);
	if (file.exists())
	{
		if (file.remove())
		{
			return true;
		}
		else
		{
			qDebug() << "fail";
			return false;
		}
	}
	return  false;
}

void ParameterFile::saveMachineProfile(const QString& machineName)
{
	QString currentMachine;
	if (machineName == QString())
		currentMachine = SlicerManager::instance().getCurrentMachine();
	else
		currentMachine = machineName;
	QString strOldPath = m_config_root + "/default/" + currentMachine + ".default";
    QString strPathnew = getCoachPath(ProfileType::MACHINE) + currentMachine + ".default";
	us::ConfigurationManager::instance().setProfilePath(strPathnew);
	QDir dir;
	if (!dir.exists(strPathnew))
	{
		copyFileToPath(strOldPath, strPathnew);
	}

	m_pFile->setFileName(strPathnew);
	//QFile file(strPathnew);
	if (!m_pFile->open(QIODevice::ReadOnly))
	{
		qDebug() << "open file failed";
		return;
	}
	QStringList allvalue;
	//us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();
	QStringList advanceList;
	initMachineSetting(advanceList);
	while (m_pFile->atEnd() == false) {
		QByteArray tmp = m_pFile->readLine();
		QString strtmp = QString(tmp);
		QStringList list = strtmp.split("=");
		if (list.size() > 1)
		{
			if (list.at(0).size() > 0)
			{
				auto vh = SlicerManager::instance().globalsettings_modify()->settings().find(list.at(0));
				if (vh == SlicerManager::instance().globalsettings_modify()->settings().end())
				{
					qDebug() << "getSetVal failed !";
					continue;

				}

				QString value;

				if ("layer_height" == list.at(0)
					|| "line_width" == list.at(0))
				{
					value = QString("%1").arg(SlicerManager::instance().globalsettings_modify()->settings().value(list.at(0))->value().toDouble());
				}
				if ("gcode_start" == list.at(0)
					|| "inter_layer" == list.at(0)
					|| "gcode_end" == list.at(0)
					|| "machine_start_gcode" == list.at(0)
					|| "machine_end_gcode" == list.at(0))
				{
					QString str = SlicerManager::instance().globalsettings_modify()->settings().value(list.at(0))->value().toString();
					value = str.replace("\n", "\\n");
				}
				else
				{
					value = SlicerManager::instance().globalsettings_modify()->settings().value(list.at(0))->value().toString();
				}

				strtmp = list.at(0) + "=" + value;
				allvalue.append(strtmp);
				advanceList.removeOne(list.at(0));
			}
		}
	}

	foreach(auto strData, advanceList)
	{
		QString value;
		if (strData.isEmpty())continue;
		QHash<QString, USetting*> tt = SlicerManager::instance().globalsettings_modify()->settings();
		auto vh = SlicerManager::instance().globalsettings_modify()->settings().find(strData);

		if (vh == SlicerManager::instance().globalsettings_modify()->settings().end())
		{
			qDebug() << "getSetVal  strData =" + strData + " failed !";
			continue;
		}

		if ("layer_height" == strData
			|| "line_width" == strData)
		{
			value = QString("%1").arg(SlicerManager::instance().globalsettings_modify()->settings().value(strData)->value().toDouble());
		}
		if ("gcode_start" == strData
			|| "inter_layer" == strData
			|| "gcode_end" == strData
			|| "machine_start_gcode" == strData
			|| "machine_end_gcode" == strData)
		{
			QString str = SlicerManager::instance().globalsettings_modify()->settings().value(strData)->value().toString();
			value = str.replace("\n", "\\n");
		}
		else
		{
			value = SlicerManager::instance().globalsettings_modify()->settings().value(strData)->value().toString();
		}

		QString strtmp = strData + "=" + value;
		allvalue.append(strtmp);
	}

	m_pFile->close();

	QFile file(strPathnew);
	if (file.exists())
	{
		if (!file.remove())
		{
			qDebug() << "fail";
		}
	}

	m_pFile->setFileName(strPathnew);
	//file.setFileName(strPathnew);
	if (!m_pFile->open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		qDebug() << "write file failed";
		return;
	}

	for (auto val : allvalue)
	{
		m_pFile->write(val.toUtf8());
		m_pFile->write("\n");
	}
	m_pFile->close();
}

void ParameterFile::addNewMachine(QString newPrinter, QString oldPrinter)
{
	const QString SETTINGPATH = getCoachPath(ProfileType::MACHINE);
	QString strPathnew = SETTINGPATH + newPrinter + ".default";

	QFile file(SETTINGPATH + oldPrinter + ".default");
	if (file.exists())
	{
		copyFileToPath(SETTINGPATH + oldPrinter + ".default", strPathnew);
	}
	else
	{
		copyFileToPath(m_config_root + "/default/" + oldPrinter + ".default", strPathnew);
	}
}

bool ParameterFile::delMachinesFile()
{
	QString strPath = getCoachPath(ProfileType::MACHINE) + SlicerManager::instance().getCurrentMachine() + ".default";
	
	QString machine = SlicerManager::instance().getCurrentMachine();
	if (!machine.isEmpty())
	{
		delDir(getCoachPath(ProfileType::PROFILE) + machine + "/");
	}

	QFile file(strPath);
	if (file.exists())
	{
		if (file.remove())
		{
			return true;
		}
		else
		{
			qDebug() << "fail";
			return false;
		}
	}
	return  false;
}

QList<Profile*> ParameterFile::getDefaultFDMProfiles(QString name)
{
	QList<Profile*> profiles;
	QFileInfoList filelistt = getFileName(name);
	for (auto file : filelistt)
	{
	//	Profile* profile = new Profile();
	//	profile->setName(file.baseName());
	//	//profile->setLayerHeight(QString("123"));
	//	profiles.append(profile);
	}

	return profiles;
}

QFileInfoList ParameterFile::getFileName(QString name, int type /*= ProfileType::PROFILE*/)
{
	QFileInfoList list;
	QString path;
	if (type == ProfileType::MATERIAL)
	{
		path = getCoachPath(ProfileType::MATERIAL);
	}
	else if (type == ProfileType::MACHINE)
	{
		path = getCoachPath(ProfileType::MACHINE);
	}
	else
	{
		path = getCoachPath() + name;
	}

	QDir dir(path);

	if (!dir.exists())
		return list;

	dir.setFilter(QDir::Files);
	dir.setSorting(QDir::Time | QDir::Reversed);//by TCJ

	list = dir.entryInfoList();

	//getMachineType
	if (ProfileType::MATERIAL == type || type == ProfileType::MACHINE)
	{
		return list;
	}

	//sort
	QStringList listSort = { "BEST", "HIGH","MIDDLE","LOW" };
	QFileInfoList newlist;
	for (auto index : listSort)
	{
		for (auto file : list)
		{
			if (file.baseName().toUpper() == index) {
				newlist.push_back(file);
				list.removeOne(file);
			}
		}
	}
	return newlist + list;
}

QString ParameterFile::mkMutiDir(const QString path)
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

bool ParameterFile::copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist /*= true*/)
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
		{
			return true;
		}
	}//end if

// 	if (!QFile::copy(sourceDir, toDir))
// 	{
// 		return false;
// 	}
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
	if (m_pFile->isOpen())
		m_pFile->close();
	if (m_pFile->open(QIODevice::ReadWrite | QIODevice::NewOnly))
	{
		m_pFile->write(all);
		m_pFile->close();
	}


	QFile(toDir).setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
	return true;
}

QStringList ParameterFile::getMaterialType(QString machineName)
{
	QStringList list;

	QFileInfoList filelistt = getFileName(machineName, ProfileType::MATERIAL);
	for (auto file : filelistt)
	{
		QString strtmp = readLine(file.filePath());
		QStringList _list = strtmp.split("=");
		if (_list.size() == 1)
		{
			strtmp = _list[0].simplified();
		}
		else if (_list.size() > 1)
		{
			strtmp = _list[1].simplified();
		}
		if ("Normal" == strtmp
			|| "Castable" == strtmp)
		{
			if ("FDM" == machineName)//filter
			{
				list.append(file.baseName());
			}
		}
		else
		{
			if ("DLP" == machineName)//filter
			{
				list.append(file.baseName());
			}
		}
	}
	return list;
}

QString ParameterFile::readLine(QString path)
{
	m_pFile->setFileName(path);
	//QFile file(path);
	if (!m_pFile->exists())
	{
		qDebug() << "source file not existed!" << path;
		return "";
	}
	//open file, read a line ,check type
	if (!m_pFile->open(QIODevice::ReadOnly))
	{
		qDebug() << "open file failed";
		return "";
	}
	QByteArray tmp = m_pFile->readLine();
	QString strtmp = QString(tmp);
	m_pFile->close();
	return strtmp;
}

bool ParameterFile::delDir(const QString& path)
{
	if (path.isEmpty()) {
		return false;
	}
	QDir dir(path);
	if (!dir.exists()) {
		return true;
	}
	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
	QFileInfoList fileList = dir.entryInfoList();
	foreach(QFileInfo file, fileList) {
		if (file.isFile()) {
			file.dir().remove(file.fileName());
		}
		else {
			delDir(file.absoluteFilePath());
		}
	}
	return dir.rmpath(dir.absolutePath());
}

void ParameterFile::initMachineSetting(QStringList& list)
{
	//Comprehensive
	/*QString strPara = "layer_height,layer_height_0,line_width,infill_line_width,support_line_width,initial_layer_line_width_factor";*/
	QString strPara1 = "machine_shape,machine_height,machine_width,machine_heated_bed,machine_extruder_type,machine_gcode_flavor,machine_center_is_zero";
	QString strPara2 = ",machine_extruder_count,machine_head_with_fans_polygon,machine_depth,machine_nozzle_size,has_preview_img,screen_size,machine_start_gcode,machine_end_gcode,preview_img_type";

	QString strPara = strPara1 + strPara2;
	list = strPara.split(",");
}

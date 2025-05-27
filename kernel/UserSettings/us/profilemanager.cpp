#include "profilemanager.h"
#include "configurationmanager.h"
#include "us/slicermanager.h"
#include <QStandardPaths>
#include <QDebug>


ProfileManager ProfileManager::s_profileManager;
ProfileManager::ProfileManager()
{
    m_pCurrentProfileModel = new ProfileListModel();
    m_pFile = new CryptFileDevice();
    m_pFile->setPassword("8JGSEwfmq5hWKwwW4R-=[X9%OmOh-qwC");
    m_pFile->setSalt("rluH$Z+(ss+E]hms");
    //m_pFile = new CryptFileDevice();
}

ProfileManager::~ProfileManager()
{

}

ProfileManager& ProfileManager::instance()
{
        return s_profileManager;
}

QString ProfileManager::getcurrentProfileName()
{
    QString res;
    Profile* profile = m_pCurrentProfileModel->selectProfile();
    if (profile)
        res = profile->fullName();
    return res;
}

USettings* ProfileManager::getSelectUSettings()
{
    Profile *profile = m_pCurrentProfileModel->selectProfile();
    if (nullptr != profile)
    {
        return ConfigurationManager::instance().settingsFromQualityType(profile->fullName());
    }
    else
    {
        return nullptr;
    }
}

void ProfileManager::updateProfileLayerHeight(QString name)
{
    m_pCurrentProfileModel->updateLayerHeigh();
}

void ProfileManager::cleanCurrentProfileModel()
{
    m_pCurrentProfileModel->clean();
}

void ProfileManager::setProfiles(QList<Profile*> list)
{
    m_pCurrentProfileModel->setProfiles(list);
}

void ProfileManager::createNewFiles(QFileInfoList files)
{
    m_pCurrentProfileModel->createProfiles(files);
}

QString ProfileManager::getMachineType(const QString path)
{
    QString strRet = "";
    //file does not exist,create it!
    QString _path = getCoachPath(ProfileType::MACHINE) + path + ".default";

    m_pFile->setFileName(_path);
    //QFile file(_path);

    //open file, read a line ,check type
    if (!m_pFile->open(QIODevice::ReadOnly))
    {
        qDebug() << "open file failed";
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

void ProfileManager::createMaterialsByMachineName(QString machine)
{
    QDir dir(getCoachPath() + machine);
    QStringList files;
    for (auto item : dir.entryInfoList(QDir::Filter::Files))
    {
        files.push_back(item.baseName());
    }
    m_pCurrentProfileModel->createMaterials(files);
}

QString ProfileManager::getCoachPath(int type /*= 0*/)
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
    return dir.absolutePath() + "/";
}

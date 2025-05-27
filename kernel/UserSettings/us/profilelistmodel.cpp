#include "profilelistmodel.h"
#include "slicermanager.h"
#include <QURL.H>
#include <qstandardpaths.h>

enum {
    NameRole = Qt::UserRole + 1,
    DespRole,
    LayerRole,
    MaterialRole,
    QualityRole,
    QualityImg,
    ISDefaultRole,
};
ProfileListModel::ProfileListModel(QObject *parent) : QAbstractListModel(parent)
{

}

bool ProfileListModel::setSelectIndex(int index)
{
    if (m_CurMetrialProfileList.empty())
    {
        return false;
    }
    m_selectIndex = index;
    bool isDefault = m_CurMetrialProfileList[index]->isDefaultProfile();

    QSettings setting;
    setting.beginGroup("profile_setting");
    setting.setValue("profile_index", index);
    setting.endGroup();

    return isDefault;
}
bool ProfileListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return true;
}
bool ProfileListModel::copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
{
    QUrl sourceUrl(sourceDir);
    QString srcUrl = sourceDir;
    QUrl destUrl(toDir);
    QString dstUrl = destUrl.toLocalFile();
    if (srcUrl == dstUrl) {
        return true;
    }
    if (QFile::exists(dstUrl)) {
        return false;
    }
    QDir* createfile = new QDir(dstUrl);

    if (coverFileIfExist) {
        createfile->remove(dstUrl);
    }

    if (!QFile::copy(srcUrl, dstUrl))
    {
        return false;
    }
    return true;
}

QString ProfileListModel::getCusFileFullName(QString name)
{
    return selectProfile()->fullName() + ".default";
}

QStringList ProfileListModel::currentMetrials()
{
    return m_CurMaterials;
}

QStringList ProfileListModel::profileList()
{
    m_ProfileList.clear();

    QString final;
    for (Profile* file : m_CurMetrialProfileList)
    {
        QString temp = file->getName();
        if (!temp.contains("USER"))
            final = temp;
        else {
            final = temp.mid(4);
        }
        m_ProfileList << final;
    }
    
    return m_ProfileList;
}

QHash<int, QByteArray> ProfileListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "profileName";
    roles[DespRole] = "profileTips";
    roles[LayerRole] = "profileLayerHeight";
    roles[MaterialRole] = "profileMaterial";
    roles[QualityRole] = "profileQuality";
    roles[QualityImg] = "profileQualityImg";
    roles[ISDefaultRole] = "profileIsDefault";

    return roles;
}

QVariant ProfileListModel::data(const QModelIndex& index, int role) const
{
    int i = index.row();
    if (i < 0 || i >= m_CurMetrialProfileList.size())
        return QVariant(QVariant::Invalid);
    if(role==NameRole)
    {
        QString name;
        if (!m_CurMetrialProfileList[i]->isDefaultProfile())
            name = m_CurMetrialProfileList[i]->getName().mid(4);
        else
            name = m_CurMetrialProfileList[i]->getName();

        if (name == "high")name = tr("Dynamic");
        else if (name == "middle") name = tr("Balanced");
        else if (name == "low")name = tr("Speedy");
        else if (name == "best")name = tr("Best quality");
        else if (name == "fast")name = tr("Very Speedy");
        return QVariant::fromValue(name);
    }

    if(role==DespRole)
    {
        return QVariant::fromValue(m_CurMetrialProfileList[i]->getDescription());
    }
    if (role == LayerRole)
    {
        return QVariant::fromValue(m_CurMetrialProfileList[i]->getLayerHeight().toDouble());
    }
    if (role == MaterialRole)
    {
        return QVariant::fromValue(m_CurMetrialProfileList[i]->getMaterialName());
    }
    if (role == QualityRole)
    {
        return QVariant::fromValue(m_CurMetrialProfileList[i]->getQualityName());
    }
    if (role == ISDefaultRole)
    {
        return QVariant::fromValue(m_CurMetrialProfileList[i]->isDefaultProfile());
    }
    return QVariant::fromValue(QString("---"));
}
void ProfileListModel::setProfiles(QList<Profile*> list)
{
    mData = list;
    layoutChanged();
}

void ProfileListModel::createProfiles(QFileInfoList files)
{
    clean();
    for (auto file : files)
    {
        QString baseName = file.baseName();
        Profile* profile = new Profile();
        QString material; //材料名称
        QString name; //文件名
        QString qualityName; //质量名称
        if (baseName.count("_") == 2)
        {//自定义格式
            QStringList strList = baseName.split("_");
            material = strList.at(0);
            name = strList.at(1);
            qualityName = strList.at(2);
        }
        else if(baseName.count("_") == 1)
        {//PC-PLA_FAST.default
            QStringList strList = baseName.split("_");
            material = strList.at(0);
            name = strList.at(1);
            qualityName = strList.at(1);
        }
        else if (baseName.count("_") == 0)
        {//high.default,middle.default
            material = QString();
            name = baseName;
            qualityName = baseName;
        }

        if (material.isEmpty())
        {
            material = "PLA";
        }

        profile->setFullName(baseName);
        profile->setMaterialName(material);
        profile->setName(name);
        profile->setQualityName(qualityName);

        profile->setMachine(SlicerManager::instance().getCurrentMachine());
        profile->setLayerHeight(getProfileLayerHeight(file.baseName()));
        profile->setIsDefaultProfile(jugeIsDefaultFile(name));
        
        mData.append(profile);
        if (!m_CurMaterials.contains(material))
        {
            m_CurMaterials.push_back(material);
        }
    }
    emit currentMetrialsChanged();
}

void ProfileListModel::createMaterials(QStringList files)
{
    m_CurMaterials.clear();
    for (auto file : files)
    {
        QString baseName = file;
        QString material; //材料名称
        if (baseName.count("_") == 2)
        {//自定义格式
            QStringList strList = baseName.split("_");
            material = strList.at(0);
        }
        else if (baseName.count("_") == 1)
        {//PC-PLA_FAST.default
            QStringList strList = baseName.split("_");
            material = strList.at(0);
        }
        else if (baseName.count("_") == 0)
        {//high.default,middle.default
            material = QString();
        }

        if (material.isEmpty())
        {
            material = "PLA";
        }
        if (!m_CurMaterials.contains(material))
        {
            m_CurMaterials.push_back(material);
        }
    }
    emit currentMetrialsChanged();
}

void ProfileListModel::clean()
{
    m_CurMaterials.clear();
    beginResetModel();
    for(Profile* p : mData) {
        if (p)
        {
            delete p;
            p = nullptr;
        }

    }
    m_CurMetrialProfileList.clear();
    mData.clear();
    endResetModel();
    emit currentMetrialsChanged();
}

QString ProfileListModel::getProfileLayerHeight(QString name)
{
    QString strPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Profiles/" + SlicerManager::instance().getCurrentMachine() + "/";
    QString strName = "";
    int nIndex = 0;

    if (name == "")
    {
        strName = mData[m_selectIndex]->getName();
        nIndex = m_selectIndex;
    }
    else
    {
        strName = name;
        for (int index = 0; index < mData.size(); index++)
        {
            if (name == mData[index]->getName())
            {
                nIndex = index;
                break;
            }
        }
    }


    QString fileName = strPath + strName + ".default";

    USettings* settings = new USettings(this);
    settings->loadDefault(fileName);
    //return settings;

    QString layerHeight = "0.10";

    QHash<QString, USetting*> hashSetting = settings->settings();

    if (hashSetting.find("layer_height") != hashSetting.end())
    {
        layerHeight = settings->settings().value("layer_height")->value().toString();
    }

    return layerHeight;
}

void ProfileListModel::currentMetrial(const QString & curMaterial)
{
    if (curMaterial == m_CurMaterial)
    {
        return;
    }
    m_CurMaterial = curMaterial;

    beginResetModel();
    m_CurMetrialProfileList.clear();
    foreach(auto item, mData) {
        if (item->getMaterialName() == curMaterial || item->getMaterialName() == "ALL")
        {
            m_CurMetrialProfileList.push_back(item);
        }
    }
    endResetModel();
}

void ProfileListModel::addProfile(QString material, QString fileName, QString quality)
{
    QString baseName = fileName;
    Profile* profile = new Profile();

    QString fullName = material + "_" + "USER" + fileName;
    profile->setFullName(fullName);
    profile->setMaterialName(material);
    profile->setName("USER" + fileName.split("_").at(0));
    profile->setQualityName(quality);
    profile->setIsDefaultProfile(false);

    profile->setMachine(SlicerManager::instance().getCurrentMachine());
    profile->setLayerHeight(getProfileLayerHeight(fullName));
    profile->setIsDefaultProfile(false);
    SlicerManager::instance().setCurrentQuality(fullName);

    mData.append(profile);
    if(m_CurMaterial != material)
        return;
    else{
        m_CurMetrialProfileList.push_back(profile);
        //setSelectIndex(m_CurMetrialProfileList.count() - 1);
        emit sigAddSuccess(m_CurMetrialProfileList.count() - 1);
        beginResetModel();
        endResetModel();
    }
}

void ProfileListModel::deleteCurFile()
{
    QString fileName = m_CurMetrialProfileList.at(m_selectIndex)->getName();
    int index = -1;
    for (auto item : mData)
    {
        if (item->getName() == fileName)
        {
            mData.removeAt(index);
            break;
        }
        ++index;
    }
    m_CurMetrialProfileList.removeAt(m_selectIndex);
    beginResetModel();
    endResetModel();
}

void ProfileListModel::updateLayerHeigh()
{
    for (int index = 0; index < mData.size(); index++)
    {
        mData[index]->setLayerHeight(getProfileLayerHeight(mData[index]->fullName()));
    }

    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomLeft = createIndex(mData.size()-1, 0);

    emit dataChanged(topLeft, bottomLeft);
}

bool ProfileListModel::jugeIsDefaultFile(const QString& name)
{
    if (name.mid(0, 4) == "USER")
        return false;
    else
        return true;
}

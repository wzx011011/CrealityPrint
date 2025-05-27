#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QObject>
#include <QStringList>
#include "profilelistmodel.h"
#include "usersettingsexport.h"
#include "usettings.h"
#include <QDir>
#include "def/cryptfiledevice.h"
using namespace us;
class USER_SETTINGS_API ProfileManager
{
    enum ProfileType
    {
        PROFILE,
        MATERIAL,
        MACHINE
    };

public:
    ProfileManager();
    ~ProfileManager();
    static ProfileManager& instance();
    static ProfileManager s_profileManager;
    //void updateProfileByMachineName(QString name);
    ProfileListModel *getProfileModel(){return m_pCurrentProfileModel;}
    USettings* getSelectUSettings();
    void cleanCurrentProfileModel();
    void updateProfileLayerHeight(QString name);
    void createNewFiles(QFileInfoList files);
    void setProfiles(QList<Profile*> list);

    QString getcurrentProfileName();
    QString getCoachPath(int = ProfileType::PROFILE);
    QString getMachineType(const QString path);
    void createMaterialsByMachineName(QString machine);

protected:
private:
    ProfileListModel *m_pCurrentProfileModel=nullptr;
private:
    CryptFileDevice* m_pFile;
};

#endif // PROFILEMANAGER_H

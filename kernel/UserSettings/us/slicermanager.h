#ifndef SLICERMANAGER_H
#define SLICERMANAGER_H

#include <QObject>
#include "us/globalsetting.h"
#include "settingcache.h"
#include "configurationmanager.h"
#include "profilemanager.h"
namespace us
{
    class GlobalSetting;
    class ExtruderSetting;
}
class USER_SETTINGS_API SlicerManager : public QObject
{
    Q_OBJECT
public:
    explicit SlicerManager(QObject *parent = nullptr);
    static SlicerManager& instance();
    enum SLICERTYPE {FDM,DLP,LASER};
    void setFDMSlicer(QObject *slicer);
    void setDLPSlicer(QObject *slicer);
    QObject* getFDMSlicer(){return m_pFDMSlicerPlgin;}
    QObject* getDLPSlicer(){return m_pDLPSlicerPlgin;}
    void loadDefault();
    void setCurrentMachine(QString machineName);
    void setCurrentMaterial(QString materialName);
    void setCurrentQuality(QString qualityName);
    void setCurrentSlicerType(SLICERTYPE slicetype);

    void insertSettingCache(QString key, QString value);
    void mergeSettingCache();
    void clearSettingCache();
    void updateExtruder();

    QString getCurrentMachine() { return m_currentMachineName;}
    QString getCurrentMaterial() { return m_currentMaterial;}
    QString getCurrentQuality(){ return m_currentQuality;}
    SLICERTYPE getCurrentSlicerType() {return m_currentSlicerType;}
    us::GlobalSetting* globalsettings(const QString& machineName = QString());
    us::GlobalSetting* globalsettings_modify();
    us::SettingCache* globalCache();
    void clearGlobalCache();
    QList<const us::ExtruderSetting*> extruderSettings() {return m_ExtruderSettingList;}

    void setMachineData(QString machineName);

    QString getSetVal(QString datakey, QString defaultvalue);
    bool isDLPMachine();

    void setDoingSlice(bool isDoing);
    bool getDoingSlice() { return m_isDoingSlice; }

    void setSliceParamDirty(bool dirty);
    bool sliceParamDirty();
signals:
    void SliceTypeChanged(SlicerManager::SLICERTYPE type);
protected:
    static SlicerManager s_slicerManager;
    us::GlobalSetting* m_globalSettings;
public slots:
private:
    QString m_currentMachineName;
    QString m_currentMaterial;
    QString m_currentQuality;
    QObject* m_pFDMSlicerPlgin;
    QObject* m_pDLPSlicerPlgin;
    us::SettingCache *m_settingCache;
    SLICERTYPE m_currentSlicerType;
    QList<const us::ExtruderSetting*> m_ExtruderSettingList;
    bool m_isDoingSlice;

    bool m_sliceParamDirty;
};
Q_DECLARE_METATYPE(SlicerManager::SLICERTYPE);

#define GLOBALPARAM(key, dValue) SlicerManager::instance().getSetVal(key, dValue)
#define ISDLPMACHINE SlicerManager::instance().isDLPMachine()
#endif // SLICERMANAGER_H

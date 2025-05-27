#ifndef CCOMMANDSDATA_H
#define CCOMMANDSDATA_H

#include <QObject>
#include "menu/actioncommand.h"
#include <QVariantList>
#include <QList>
#include "creativekernelexport.h"
class CREATIVE_KERNEL_API CCommandsData : public QObject
{
    Q_OBJECT

public:
    explicit CCommandsData(QObject *parent = nullptr);
    virtual ~CCommandsData();
    void createMenuNameMap();
    Q_INVOKABLE void addCommads(ActionCommand *pAction);
//    QString getMenuNameFromEType(eMenuType eType);
    Q_INVOKABLE QString getMenuNameFromKey(QString key);
    Q_INVOKABLE QVariantList  getCommandsList(void);
    Q_INVOKABLE QVariantMap getCommandsMap(void);

    Q_INVOKABLE QVariantList getSpecialSettingKey();
    Q_INVOKABLE void deleteSpecialSetting();

    Q_INVOKABLE QVariantMap getJsonMap(void);
    Q_INVOKABLE QVariantList getGlobalSettingKey(void);
    Q_INVOKABLE QVariantList getClassTypeList();
    Q_INVOKABLE QVariantList getAllSettingKey();
    Q_INVOKABLE QVariant getFileOpenOpt();
    Q_INVOKABLE QVariant getOpt(const QString& optName);

    void initMapData(QVariantMap &map);

private:
    void addCommonCommand();

public slots:
    void slotLanguageChanged();
private:
    QList<ActionCommand *>  m_varCommandList;
    QMap<QString,QString>  m_mapMenuName;
    ActionCommand* m_FileOpenOpt;

};
//Q_DECLARE_METATYPE(CCommandsData)
#endif // CCOMMANDSDATA_H

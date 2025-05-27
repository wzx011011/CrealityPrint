#ifndef RCLICKMENUDATA_H
#define RCLICKMENUDATA_H

#include <QObject>
#include "menu/actioncommand.h"
#include <QVariantList>
#include <QList>
#include "creativekernelexport.h"
class CREATIVE_KERNEL_API RClickMenuData : public QObject
{
    Q_OBJECT

public:
    explicit RClickMenuData(QObject *parent = nullptr);
    virtual ~RClickMenuData();
    void createMenuNameMap();
    Q_INVOKABLE void addCommads(ActionCommand *pAction);
//    QString getMenuNameFromEType(eMenuType eType);
    Q_INVOKABLE QString getMenuNameFromKey(QString key);
    Q_INVOKABLE QVariantList  getCommandsList(void);
    Q_INVOKABLE QVariantMap getCommandsMap(void);

    void initMapData(QVariantMap &map);
public slots:
    void slotLanguageChanged();
private:
    QList<ActionCommand *>  m_varCommandList;
    QMap<QString,QString>  m_mapMenuName;

};
//Q_DECLARE_METATYPE(RClickMenuData)
#endif // RCLICKMENUDATA_H

#ifndef RCLICKMENULIST_H
#define RCLICKMENULIST_H

#include <QObject>
#include "../external/creativekernelexport.h"
class ActionCommand;
class CREATIVE_KERNEL_API RClickMenuList : public QObject
{
    Q_OBJECT
public:
    explicit RClickMenuList(QObject *parent = nullptr);
    void addActionCommad(ActionCommand *pAction);
    void addActionCommads(QList< ActionCommand *>vecAction);
    //
    void addActionCommad(ActionCommand *pAction, int index);
    //
    void addActionCommad(ActionCommand *pAction,QString strInfo);
    void removeActionCommand(ActionCommand* command);
    int indexOfString(QString strInfo);
    QList<ActionCommand *> getActionCommandList();
    void startAddCommads();
     static RClickMenuList *getInstance();

//     void doRightClick(bool bEnable);
//signals:
//     void sigRightClick(bool bEnable);
private:
     QList<ActionCommand *> m_varCommandList;
     static RClickMenuList * m_pComInstance;
};

#endif // RClickMenuList_H

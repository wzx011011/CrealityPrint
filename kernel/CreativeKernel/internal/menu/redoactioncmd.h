#ifndef REDOACTIONCMD_H
#define REDOACTIONCMD_H

#include <QObject>
#include "menu/actioncommand.h"

//#include "creativeinterface.h"
//#include <QtQml/QQmlComponent>
class RedoActionCmd: public ActionCommand
{
    Q_OBJECT
public:
    RedoActionCmd();
    ~RedoActionCmd();

    Q_INVOKABLE void execute();
protected:
//    QObject* m_root;

//    QQmlComponent* m_component;
private slots:
    void slotLanguageChanged();
};

#endif // REDOACTIONCMD_H

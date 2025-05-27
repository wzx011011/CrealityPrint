#ifndef UPDATECOMMAND_H
#define UPDATECOMMAND_H

#include <QObject>
#include "menu/actioncommand.h"
#include "qtuserqml/updater/UpdateManager.h"

class UpdateCommand: public ActionCommand
{
    Q_OBJECT
    Q_PROPERTY(QObject* updateManager READ updateManager CONSTANT)
public:
    UpdateCommand();
    ~UpdateCommand();

    Q_INVOKABLE void execute();
    void  requestMenuDialog();
    QObject* updateManager();

private slots:
    void slotLanguageChanged();

private:
    UpdateManager* m_pUpdateManager;
};

#endif // UPDATECOMMAND_H

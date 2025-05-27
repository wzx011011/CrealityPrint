#ifndef CX3DRECNETPROJECTCOMMAND_H
#define CX3DRECNETPROJECTCOMMAND_H

#include <QObject>
#include "menu/actioncommand.h"
class ProjectInfoUI;
class Cx3dRecentProjectCommand: public ActionCommand
{
    Q_OBJECT
public:
    Cx3dRecentProjectCommand();
    ~Cx3dRecentProjectCommand();
    Q_INVOKABLE void execute();
private:
    QString m_strFilePath;
    ProjectInfoUI* m_projectUI;
public slots:
    void slotSaveLastPro();
    void slotCancelSaveLastPro();
};

#endif // CX3DRECNETPROJECTCOMMAND_H

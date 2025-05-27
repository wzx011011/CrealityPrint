#ifndef CX3DCLEARPROJECTCOMMAND_H
#define CX3DCLEARPROJECTCOMMAND_H

#include "menu/actioncommand.h"
class Cx3dClearProjectCommand:public ActionCommand
{
    Q_OBJECT
public:
    Cx3dClearProjectCommand();
    virtual ~Cx3dClearProjectCommand();
    Q_INVOKABLE void execute() ;
signals:
    void sigExecute();
private slots:
    void slotLanguageChanged();
};

#endif // CX3DCLEARPROJECTCOMMAND_H

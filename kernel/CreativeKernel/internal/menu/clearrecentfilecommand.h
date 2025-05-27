#ifndef CLEARRECENTFILECOMMAND_H
#define CLEARRECENTFILECOMMAND_H

#include "menu/actioncommand.h"
class ClearRecentFileCommand:public ActionCommand
{
    Q_OBJECT
public:
    ClearRecentFileCommand();
    virtual ~ClearRecentFileCommand();
    Q_INVOKABLE void execute() ;
signals:
    void sigExecute();
private slots:
    void slotLanguageChanged();
};

#endif // CLEARRECENTFILECOMMAND_H

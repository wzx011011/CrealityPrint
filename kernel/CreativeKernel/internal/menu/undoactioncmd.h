#ifndef UNDOACTIONCMD_H
#define UNDOACTIONCMD_H

#include <QObject>
#include "menu/actioncommand.h"

class UndoActionCmd: public ActionCommand
{
    Q_OBJECT
public:
    UndoActionCmd();
    ~UndoActionCmd();

    Q_INVOKABLE void execute();
    //void requestOpenFileDialog(QObject* receiver);
private slots:
    void slotLanguageChanged();
};

#endif // UNDOACTIONCMD_H

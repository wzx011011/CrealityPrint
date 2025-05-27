#ifndef OPENRECENTFILECmd_H
#define OPENRECENTFILECmd_H
#include <QtCore/QObject>
#include "menu/actioncommand.h"

class OpenRecentFileCmd: public ActionCommand
{
    Q_OBJECT
public:
    OpenRecentFileCmd();
    ~OpenRecentFileCmd();
    Q_INVOKABLE void execute();
private slots:
    void slotLanguageChanged();
};

#endif // OPENRECENTFILECmd_H

#ifndef NOZZLEACTION_H
#define NOZZLEACTION_H

#include <QObject>
#include "menu/actioncommand.h"
class NozzleAction:public ActionCommand
{
    Q_OBJECT
public:
    NozzleAction(int nNumber);
    ~NozzleAction();

    Q_INVOKABLE void execute();
    bool enabled() override;
private slots:
    void slotLanguageChanged();

private:
    int m_nNumber = 1;
};

#endif // NOZZLEACTION_H

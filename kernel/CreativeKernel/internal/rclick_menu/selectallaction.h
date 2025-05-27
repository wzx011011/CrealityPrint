#ifndef SELECTALLACTION_H
#define SELECTALLACTION_H

#include <QObject>
#include "menu/actioncommand.h"
class SelectAllAction:public ActionCommand
{
    Q_OBJECT
public:
    SelectAllAction();
    ~SelectAllAction();

    Q_INVOKABLE void execute();
    bool enabled() override;
private slots:
    void slotLanguageChanged();
};

#endif // SELECTALLACTION_H

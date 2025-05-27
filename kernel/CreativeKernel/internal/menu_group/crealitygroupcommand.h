#ifndef CREALITYGOURPCOMMAND_H
#define CREALITYGOURPCOMMAND_H

#include <QObject>
#include "menu/actioncommand.h"

class CrealityGroupCommand: public ActionCommand
{
    Q_OBJECT
public:


    CrealityGroupCommand();
    ~CrealityGroupCommand();

    Q_INVOKABLE void execute();
    //void requestOpenFileDialog(QObject* receiver);
private slots:
    void slotLanguageChanged();
};

#endif // CREALITYGOURPCOMMAND_H

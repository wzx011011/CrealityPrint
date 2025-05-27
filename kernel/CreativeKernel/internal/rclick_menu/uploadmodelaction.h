#ifndef UPLOADMODELACTION_H
#define UPLOADMODELACTION_H

#include <QObject>
#include "menu/actioncommand.h"
#include <QPoint>

class UploadModelAction:public ActionCommand
{
    Q_OBJECT
public:
    UploadModelAction();
    ~UploadModelAction();

    Q_INVOKABLE void execute();
    bool enabled() override;
private slots:
    void slotLanguageChanged();

};

#endif // UPLOADMODELACTION_H

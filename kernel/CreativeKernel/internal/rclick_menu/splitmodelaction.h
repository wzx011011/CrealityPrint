#ifndef SPLITMODELACTION_H
#define SPLITMODELACTION_H

#include <QObject>
#include <QPoint>
#include "menu/actioncommand.h"
class SplitModelAction:public ActionCommand
{
    Q_OBJECT
public:
    SplitModelAction();
    ~SplitModelAction();

    Q_INVOKABLE void execute();
    bool enabled() override;
private:
    QString m_strFilePath;
private slots:
    void slotLanguageChanged();
};

#endif // SPLITMODELACTION_H

#ifndef IMPORTMODELACTION_H
#define IMPORTMODELACTION_H

#include <QObject>
#include "menu/actioncommand.h"
class ImportModelAction: public ActionCommand
{
    Q_OBJECT
public:
    ImportModelAction();
    ~ImportModelAction();

    Q_INVOKABLE void execute();
    bool enabled() override;
  public  slots:
    void slotFinished(QString strFilePath);
private:
    QString m_strFilePath;
private slots:
    void slotLanguageChanged();
};

#endif // _IMPORTMODELACTION_H

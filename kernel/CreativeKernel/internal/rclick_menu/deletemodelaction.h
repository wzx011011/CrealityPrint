#ifndef DELETEMODELACTION_H
#define DELETEMODELACTION_H

#include <QObject>
#include "menu/actioncommand.h"
#include <QPoint>
class DeleteModelAction:public ActionCommand
{
    Q_OBJECT
public:
    DeleteModelAction();
    ~DeleteModelAction();

    Q_INVOKABLE void execute();
    Q_INVOKABLE QString getMessageText();
    Q_INVOKABLE void writeReg(bool flag);
    Q_INVOKABLE bool isPopPage();
    void requestMenuDialog();

    Q_INVOKABLE void accept();

    bool enabled() override;
private:
    QString m_strMessageText;
private slots:
    void slotLanguageChanged();
};

#endif // DELETEMODELACTION_H

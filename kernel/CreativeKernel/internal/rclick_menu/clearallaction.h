#ifndef CLEARALLACTION_H
#define CLEARALLACTION_H

#include <QObject>
#include <QPoint>
#include "menu/actioncommand.h"
class ClearAllAction:public ActionCommand
{
    Q_OBJECT
public:
    ClearAllAction();
    ~ClearAllAction();

    Q_INVOKABLE void execute();
    Q_INVOKABLE QString getMessageText();
    void requestMenuDialog();

    Q_INVOKABLE void accept();
    bool enabled() override;
private:
    QString m_strMessageText;
private slots:
    void slotLanguageChanged();

};

#endif // CLEARALLACTION_H

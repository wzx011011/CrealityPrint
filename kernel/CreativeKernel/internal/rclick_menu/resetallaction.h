#ifndef RESETALLACTION_H
#define RESETALLACTION_H

#include <QObject>
#include <QPoint>
#include "menu/actioncommand.h"
class ResetAllAction:public ActionCommand
{
    Q_OBJECT
public:
    ResetAllAction();
    ~ResetAllAction();

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

#endif // RESETALLACTION_H

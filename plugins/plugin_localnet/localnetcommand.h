#ifndef _LOCALNETCOMMAND_H
#define _LOCALNETCOMMAND_H
#include "menu/actioncommand.h"
class LocalNetCommand : public ActionCommand
{
    Q_OBJECT
public:
    LocalNetCommand(QObject* parent = nullptr);
    LocalNetCommand(int eType,QObject* parent = nullptr);
    virtual ~LocalNetCommand();

    Q_INVOKABLE void execute();
private slots:
    void slotLanguageChanged();
private:
    int m_nShowType = 0;
    bool m_bFirstShow = true;

};

#endif // _LOCALNETCOMMAND_H

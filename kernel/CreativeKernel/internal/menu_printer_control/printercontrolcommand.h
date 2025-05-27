#ifndef _PRINTERCONTROLCOMMAND_H
#define _PRINTERCONTROLCOMMAND_H
#include "menu/actioncommand.h"

class PrinterConrtolCommand : public ActionCommand
{
    Q_OBJECT
public:
    PrinterConrtolCommand(QObject* parent = nullptr);
    PrinterConrtolCommand(int eType,QObject* parent = nullptr);
    virtual ~PrinterConrtolCommand();

    Q_INVOKABLE void execute();

private:
    int m_nShowType = 0;
private slots:
    void slotLanguageChanged();
};

#endif // _PRINTERCONTROLCOMMAND_H

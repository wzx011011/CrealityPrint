#ifndef _USBPRINTCOMMAND_H
#define _USBPRINTCOMMAND_H
#include "menu/actioncommand.h"
class UsbPrintCommand : public ActionCommand
{
    Q_OBJECT
public:
    UsbPrintCommand(QObject* parent = nullptr);
    UsbPrintCommand(int eType,QObject* parent = nullptr);
    virtual ~UsbPrintCommand();

    Q_INVOKABLE void execute();
private slots:
    void slotLanguageChanged();
private:
    int m_nShowType = 0;
    bool m_bFirstShow = true;

};

#endif //

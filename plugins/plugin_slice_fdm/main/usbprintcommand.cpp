#include "usbprintcommand.h"
#include "kernel/translator.h"
#include "kernel/kernel.h"
#include "kernel/kernelui.h"
#include <QDebug>
UsbPrintCommand::UsbPrintCommand(QObject* parent)
    :ActionCommand(parent)
{
    m_actionname = tr("USB Printing");
    m_actionNameWithout = "USB Printing";
    m_eParentMenu = eMenuType_PrinterControl;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
using namespace creative_kernel;
UsbPrintCommand::UsbPrintCommand(int eType, QObject* parent)
    :ActionCommand(parent)
{
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
void UsbPrintCommand::slotLanguageChanged()
{
    m_actionname = tr("USB Printing");
}

UsbPrintCommand::~UsbPrintCommand()
{
}

void UsbPrintCommand::execute()
{
    QObject* pmainWinbj = AbstractKernelUI::getSelf()->getUI("UIAppWindow");
    QMetaObject::invokeMethod(pmainWinbj, "showFdmPmDlg", Q_ARG(QVariant, QVariant::fromValue(QString())));
}

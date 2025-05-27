#include "printercontrolcommand.h"
#include "kernel/translator.h"
#include "kernel/kernel.h"
#include "kernel/kernelui.h"
#include <QDebug>
PrinterConrtolCommand::PrinterConrtolCommand(QObject* parent)
    :ActionCommand(parent)
{
    m_actionname = "Print Control";
    m_actionNameWithout = "Print Control";
    m_eParentMenu = eMenuType_PrinterControl;
}
using namespace creative_kernel;
PrinterConrtolCommand::PrinterConrtolCommand(int eType, QObject* parent)
    :ActionCommand(parent)
{
    m_nShowType = eType;
    m_eParentMenu = eMenuType_PrinterControl;

    switch (eType)
    {
    case 0:
        m_actionname = tr("Single Control Printing");
        break;
    case 1: 
        m_actionname = tr("Group Control Printing");
       break;
    }

    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
void PrinterConrtolCommand::slotLanguageChanged()
{
    switch (m_nShowType)
    {
    case 0:
        m_actionname = tr("Single Control Printing");
        break;
    case 1:
        m_actionname = tr("Group Control Printing");
        break;
    }
}
PrinterConrtolCommand::~PrinterConrtolCommand()
{
}

void PrinterConrtolCommand::execute()
{
    QString type = "";
    QObject* pTopBar = getKernelUI()->getUI("topbar");
    switch (m_nShowType)
    {
    case 0:
        //operation: Single Control Printing
        type  = "singleprint";
        QMetaObject::invokeMethod(pTopBar, "selectPrintControlType", Q_ARG(QVariant, QVariant::fromValue(type)));
        break;
    case 1:
        //operation: Group Control Printing
        type = "clusterprint";
        QMetaObject::invokeMethod(pTopBar, "selectPrintControlType", Q_ARG(QVariant, QVariant::fromValue(type)));
        break;
    }
}

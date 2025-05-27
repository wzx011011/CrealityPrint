#include "localnetcommand.h"
#include "kernel/translator.h"
#include "kernel/kernel.h"
#include "kernel/kernelui.h"
#include <QDebug>
LocalNetCommand::LocalNetCommand(QObject* parent)
    :ActionCommand(parent)
{
    m_actionname = ("WIFI Online Printing");
    m_actionNameWithout = "WIFI Online Printing";
    m_eParentMenu = eMenuType_PrinterControl;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

}
using namespace creative_kernel;
LocalNetCommand::LocalNetCommand(int eType, QObject* parent)
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
void LocalNetCommand::slotLanguageChanged()
{
    m_actionname = tr("WIFI Online Printing");
}
LocalNetCommand::~LocalNetCommand()
{
}

void LocalNetCommand::execute()
{
    QString type = "";
    QObject* pmainObj = getKernelUI()->getUI("uiappwindow");
    if (pmainObj)
    {
        QObject *wifiObj = pmainObj->findChild<QObject*>("localWifiObj");

        if (wifiObj)
        {
            if (m_bFirstShow)
            {
                QMetaObject::invokeMethod(wifiObj, "showFirstWifiConnect");
                m_bFirstShow = false;
            }
            else
            {
                QMetaObject::invokeMethod(wifiObj, "showWifiDevice");
            }
        }
    }
    
    
}

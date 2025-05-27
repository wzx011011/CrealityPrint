#include "resetallaction.h"

#include <QSettings>
#include "kernel/translator.h"
#include "kernel/kernelui.h"
#include "rclickmenuinterface.h"
#include "interface/commandoperation.h"
#include "interface/spaceinterface.h"
ResetAllAction::ResetAllAction()
{
    m_actionname = tr("Reset All Model");
    m_actionNameWithout = "Reset All Model";
    m_strMessageText = tr("Do you Want to Reset All Model?");
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

}
ResetAllAction::~ResetAllAction()
{}
void ResetAllAction:: execute()
{
    requestMenuDialog();
}

void ResetAllAction::slotLanguageChanged()
{
    m_actionname = tr("Reset All Model");
    m_strMessageText = tr("Do you Want to Reset All Model?");
}
void ResetAllAction::requestMenuDialog()
{
     getKernelUI()->requestMenuDialog(this,"messageDlg");
}

QString ResetAllAction::getMessageText()
{
    return m_strMessageText;
}

void ResetAllAction::accept()
{
    creative_kernel::cmdResetAll();
}

bool ResetAllAction::enabled()
{
    return creative_kernel::modelns().size() > 0;
}


#include "updatecommand.h"
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>
#include "kernel/translator.h"
#include "external/kernel/kernelui.h"
UpdateCommand::UpdateCommand()
{
    m_actionname = tr("Update");
    m_actionNameWithout = "Update";
    m_eParentMenu = eMenuType_Help;

    //menu need to do change language opreator
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}


void UpdateCommand::slotLanguageChanged()
{
    m_actionname = tr("Update");
}
UpdateCommand::~UpdateCommand()
{}
void UpdateCommand::execute()
{
    requestMenuDialog();
}

void UpdateCommand::requestMenuDialog()
{
     getKernelUI()->requestMenuDialog(this,"updateObj");
}

QObject* UpdateCommand::updateManager()
{
    return m_pUpdateManager;
}

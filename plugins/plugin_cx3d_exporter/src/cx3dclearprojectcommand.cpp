#include "cx3dclearprojectcommand.h"
#include "kernel/translator.h"
Cx3dClearProjectCommand::Cx3dClearProjectCommand()
{
    m_actionname = tr("Clear");
    m_actionNameWithout = "Clear";
    m_eParentMenu = eMenuType_File;
    m_bSeparator = true;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}

Cx3dClearProjectCommand::~Cx3dClearProjectCommand()
{}
void Cx3dClearProjectCommand:: execute()
{
    emit sigExecute();
}

void Cx3dClearProjectCommand::slotLanguageChanged()
{
    m_actionname = tr("Clear");
}

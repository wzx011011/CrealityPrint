#include "clearrecentfilecommand.h"
#include "kernel/translator.h"
ClearRecentFileCommand::ClearRecentFileCommand()
{
    m_actionname = tr("Clear");
    m_eParentMenu = eMenuType_File;
    m_bSeparator = true;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}

ClearRecentFileCommand::~ClearRecentFileCommand()
{}
void ClearRecentFileCommand:: execute()
{
    emit sigExecute();
}

void ClearRecentFileCommand::slotLanguageChanged()
{
    m_actionname = tr("Clear");
}

#include "resetactioncommand.h"
#include <QDesktopServices>
#include <QUrl>

#include "interface/commandoperation.h"

#include "kernel/translator.h"

using namespace creative_kernel;
ResetActionCommand::ResetActionCommand(QObject* parent)
    :ActionCommand(parent)
{
    m_actionname = tr("Reset All Model");
    m_actionNameWithout = "Reset All Model";
    m_eParentMenu = eMenuType_View;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
ResetActionCommand::~ResetActionCommand()
{
}
void ResetActionCommand::slotLanguageChanged()
{
    m_actionname = tr("Reset All Model");
}
void ResetActionCommand::execute()
{
    creative_kernel::cmdReset();
}


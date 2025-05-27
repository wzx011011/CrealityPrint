#include "selectallaction.h"

#include <QSettings>
#include "kernel/translator.h"
#include "interface/modelinterface.h"
#include "interface/selectorinterface.h"
#include "interface/spaceinterface.h"
SelectAllAction::SelectAllAction()
{
    m_actionname = tr("Select All Model");
    m_actionNameWithout = "Select All Model";
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
SelectAllAction::~SelectAllAction()
{}
void SelectAllAction:: execute()
{
    using namespace creative_kernel;
    selectAll();
}

bool SelectAllAction::enabled()
{
    return creative_kernel::modelns().size() > 0;
}

void SelectAllAction::slotLanguageChanged()
{
    m_actionname = tr("Select All Model");
}

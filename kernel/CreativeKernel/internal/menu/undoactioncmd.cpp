#include "undoactioncmd.h"
#include "interface/undointerface.h"
#include "kernel/translator.h"
using namespace creative_kernel;
UndoActionCmd::UndoActionCmd()
{
    m_shortcut = "Ctrl+Z";      //不能有空格
    m_actionname = tr("Undo");
    m_actionNameWithout = "Undo";
    m_icon = "qrc:/kernel/images/undo.png";
    m_eParentMenu = eMenuType_Edit;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
UndoActionCmd::~UndoActionCmd()
{}
void UndoActionCmd::execute()
{
    undo();
}
void UndoActionCmd::slotLanguageChanged()
{
    m_actionname = tr("Undo") + "      " + m_shortcut;
}

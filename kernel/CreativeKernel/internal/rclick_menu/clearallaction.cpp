#include "clearallaction.h"
#include <QSettings>
#include "kernel/kernelui.h"
#include "kernel/translator.h"
#include "interface/modelinterface.h"
#include "interface/spaceinterface.h"
ClearAllAction::ClearAllAction()
{
    m_actionname = tr("Clear All");
    m_actionNameWithout = "Clear All";
    m_strMessageText = tr("Do you Want to Clear All Model?");
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
ClearAllAction::~ClearAllAction()
{}
void ClearAllAction:: execute()
{
//    using namespace creative_kernel;
//    removeAllModel(true);
    requestMenuDialog();


}
void ClearAllAction::requestMenuDialog()
{
     getKernelUI()->requestMenuDialog(this,"messageDlg");
}

QString ClearAllAction::getMessageText()
{
    return m_strMessageText;
}
void ClearAllAction::slotLanguageChanged()
{
    m_actionname = tr("Clear All");
    m_strMessageText = tr("Do you Want to Clear All Model?");
}
void ClearAllAction::accept()
{
    using namespace creative_kernel;
    removeAllModel(true);
    checkModelRange();
    checkBedRange();
    AbstractKernelUI::getSelf()->switchPickMode();//by TCJ
}
bool ClearAllAction::enabled()
{
    return creative_kernel::modelns().size() > 0;
}

#include "crealitygroupcommand.h"
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>
#include "kernel/translator.h"
#include "kernel/kernelui.h"
#include "interface/gadatainterface.h"
CrealityGroupCommand::CrealityGroupCommand()
{
    m_actionname = tr("Models");
    m_actionNameWithout = "Models";
    m_eParentMenu = eMenuType_CrealityGroup;

    //menu need to do change language opreator
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}


void CrealityGroupCommand::slotLanguageChanged()
{
    m_actionname = tr("Models");
}

CrealityGroupCommand::~CrealityGroupCommand()
{

}

void CrealityGroupCommand::execute()
{
    //QDesktopServices::openUrl(QUrl("https://model.crealitygroup.com/model"));
   
    QObject* pTopBar = getKernelUI()->getUI("topbar");
    QMetaObject::invokeMethod(pTopBar, "showModelLibraryDialog");
    creative_kernel::sendDataToGA("Model Library", "Model Library (Menu)");

}



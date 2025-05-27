#include "newprojectcommand.h"

#include "interface/modelinterface.h"
#include "interface/selectorinterface.h"
#include "kernel/translator.h"
#include "kernel/kernelui.h"
#include <QQmlProperty>
NewProjectCommand::NewProjectCommand(QObject* parent)
    :ActionCommand(parent)
{
    m_shortcut = "Ctrl+N";
    m_insertKey = "newPro";
    m_actionname = tr("New Project");
    m_actionNameWithout = "New Project";
    m_eParentMenu = eMenuType_File;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}


NewProjectCommand::~NewProjectCommand()
{
}
void NewProjectCommand::requestMenuDialog()
{
     getKernelUI()->requestMenuDialog(this,"newProjectObj");
}
void NewProjectCommand::execute()
{
//    using namespace creative_kernel;
//    removeAllModel(true);
    requestMenuDialog();
}
void NewProjectCommand::slotLanguageChanged()
{
    m_actionname = tr("New Project");//
    m_actionNameWithout = "New Project";
}

void NewProjectCommand::saveProject(QString projectName)
{
    using namespace creative_kernel;
    removeAllModel(true);
    QObject *obj = getKernelUI()->footer();
    QObject *pVersionText = obj->findChild<QObject *>("ProjectTitle");
    if(pVersionText)
    {
        QQmlProperty::write(pVersionText, "text", QVariant::fromValue(projectName));
    }
}

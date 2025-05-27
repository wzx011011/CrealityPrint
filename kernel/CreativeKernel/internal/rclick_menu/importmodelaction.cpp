#include "importmodelaction.h"
#include "../menu/submenurecentfiles.h"
#include <QSettings>
#include "interface/loadinterface.h"
#include "kernel/kernelui.h"
#include "utils/meshloaderwrapper.h"
#include "kernel/translator.h"
ImportModelAction::ImportModelAction()
{
    m_actionname = tr("Import Model");
    m_actionNameWithout = "Import Model";
    m_icon = "qrc:/kernel/images/open.png";
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
ImportModelAction::~ImportModelAction()
{}
void ImportModelAction:: execute()
{
   using namespace creative_kernel;
   requestLoad();
   MeshLoaderWrapper *p = getKernelUI()->getMeshLoaderWrapper();
   disconnect(p,SIGNAL(sigFinished(QString)),this,SLOT(slotFinished(QString)));
   connect(p,SIGNAL(sigFinished(QString)),this,SLOT(slotFinished(QString)));
}

bool ImportModelAction::enabled()
{
    return true;
}

void ImportModelAction::slotFinished(QString strFilePath)
{
    QSettings settings;
    SubMenuRecentFiles::getInstance()->setMostRecentFile(strFilePath);
}
void ImportModelAction::slotLanguageChanged()
{
    m_actionname = tr("Import Model");
}

#include "mergemodelcommand.h"
#include <QDesktopServices>
#include <QUrl>

#include "interface/commandoperation.h"

#include "kernel/translator.h"

//#include "interface/jobsinterface.h"
//#include "job/splitmodeljob.h"

using namespace creative_kernel;
MergeModelCommand::MergeModelCommand(QObject* parent)
    :ActionCommand(parent)
{
    m_actionname = tr("Merge Model Locations");
    m_actionNameWithout = "Unit As One";
    m_eParentMenu = eMenuType_View;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
MergeModelCommand::~MergeModelCommand()
{
}
void MergeModelCommand::slotLanguageChanged()
{
    m_actionname = tr("Merge Model Locations");
}
void MergeModelCommand::execute()
{
   creative_kernel::cmdMergeModel();

    //SplitModelJob* job = new SplitModelJob();
    //job->EnableSplit(false);
    //executeJob(JobPtr(job));
}


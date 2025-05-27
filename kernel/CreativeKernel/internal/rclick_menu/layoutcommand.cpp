#include "layoutcommand.h"
#include "interface/spaceinterface.h"
#include "job/autolayoutjob.h"
#include "interface/jobsinterface.h"
#include "kernel/translator.h"

using namespace qtuser_core;
LayoutCommand::LayoutCommand(QObject* parent)
	:ActionCommand(parent)
{
    m_actionname = tr("Auto arrange");
    m_actionNameWithout = "Auto arrange";
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}

LayoutCommand::~LayoutCommand()
{
}

void LayoutCommand::execute()
{
    AutoLayoutJob * layoutJob = new AutoLayoutJob();
    QList<JobPtr> jobs;
    jobs.push_back(JobPtr(layoutJob));
    creative_kernel::executeJobs(jobs);
}

bool LayoutCommand::enabled()
{
    
    return creative_kernel::modelns().size() > 0;
}

void LayoutCommand::slotLanguageChanged()
{
    m_actionname = tr("Auto arrange");
}

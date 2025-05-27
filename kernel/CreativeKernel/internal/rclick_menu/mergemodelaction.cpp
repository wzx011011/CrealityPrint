#include "mergemodelaction.h"

#include <QSettings>
#include "kernel/translator.h"
#include "interface/modelinterface.h"
#include "interface/selectorinterface.h"
#include <QtCore/QDebug>
#include "job/modelsplitalg.h"
#include "interface/jobsinterface.h"
#include "job/splitmodeljob.h"
using namespace creative_kernel;


MergeModelAction::MergeModelAction()
{
    m_actionname = tr("Merge Model");
    m_actionNameWithout = "Merge Model";
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

}


MergeModelAction::~MergeModelAction()
{
}

void MergeModelAction:: execute()
{
    SplitModelJob* job = new SplitModelJob();

    job->EnableSplit(false);

    //job->MergeModel(NULL);

    executeJob(JobPtr(job));
}

bool MergeModelAction::enabled()
{
    return selectionms().size() > 0;
}


void MergeModelAction::slotLanguageChanged()
{
    m_actionname = tr("Merge Model");
}

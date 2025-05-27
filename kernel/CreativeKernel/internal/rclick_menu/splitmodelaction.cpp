#include "splitmodelaction.h"

#include <QSettings>
#include "kernel/translator.h"

#include "data/modeln.h"

#include "interface/modelinterface.h"
#include "interface/selectorinterface.h"
#include <QtCore/QDebug>

#include "job/modelsplitalg.h"

#include "job/splitmodeljob.h"

#include "interface/jobsinterface.h"

using namespace creative_kernel;

SplitModelAction::SplitModelAction()
{
    m_actionname = tr("Split Model");
    m_actionNameWithout = "Split Model";
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}


SplitModelAction::~SplitModelAction()
{

}


void SplitModelAction:: execute()
{

    SplitModelJob* job = new SplitModelJob();

    job->EnableSplit(true);

    job->setModel(selectionms());

    //job->SplitModel(NULL);

    executeJob(JobPtr(job));
}

bool SplitModelAction::enabled() 
{
    return selectionms().size() > 0;
}

void SplitModelAction::slotLanguageChanged()
{
    m_actionname = tr("Split Model");
}


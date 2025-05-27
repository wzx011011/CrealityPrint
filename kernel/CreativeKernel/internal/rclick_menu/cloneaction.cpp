#include "cloneaction.h"
#include "interface/selectorinterface.h"
#include "interface/commandoperation.h"
#include "interface/jobsinterface.h"

#include <QtCore/QDebug>
#include "kernel/translator.h"

using namespace creative_kernel;

#include "kernel/kernelui.h"

CloneAction::CloneAction(QObject* parent)
    :ActionCommand(parent)
{
    m_actionname = tr("Clone Model");
    m_actionNameWithout = "Clone Model";
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

}

CloneAction::~CloneAction()
{
}
void CloneAction::slotLanguageChanged()
{
    m_actionname = tr("Clone Model");
}

bool CloneAction::isSelect()
{
    QList<ModelN*> selections = selectionms();
    if (selections.size() > 0)
    {
        return true;
    }else
	{
        return false;
    }
}

void CloneAction::clone(int numb)
{
    cmdClone(numb);
    m_layoutJob = new AutoLayoutJob(this);
    QList<JobPtr> jobs;
    jobs.push_back(JobPtr(m_layoutJob));
    executeJobs(jobs);
    AbstractKernelUI::getSelf()->switchPickMode();

}

void CloneAction::execute()
{
    //clone(1);
    //by TCJ
    getKernelUI()->requestMenuDialog(this, "cloneNumObj");
}
bool CloneAction::enabled()
{
    return isSelect();
}

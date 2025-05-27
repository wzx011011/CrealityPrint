#include "nozzleaction.h"

#include <QSettings>
#include "kernel/kernelui.h"
#include "kernel/translator.h"

#include "interface/modelinterface.h"
#include "interface/selectorinterface.h"
#include "interface/commandoperation.h"
#include "data/modeln.h"

using namespace creative_kernel;
NozzleAction::NozzleAction(int nNumber)
{
    m_nNumber = nNumber;
    m_actionname = tr("Nozzle ") +QString::number( m_nNumber + 1);
    m_actionNameWithout = "Nozzle";
    m_bSubMenu = true;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    QList<ModelN*> selections = selectionms();
    if(selections.size() >= 1)
    {
        ModelN* m = selections.at(0);
        if(m_nNumber == m->nozzle())
        {
            m_bChecked = true;
        }
    }
}
NozzleAction::~NozzleAction()
{}
void NozzleAction:: execute()
{
    qDebug() << "nozzle = " << m_nNumber;

    cmdSetNozzle(m_nNumber);
    
}

bool NozzleAction::enabled()
{
    return true;
}

void NozzleAction::slotLanguageChanged()
{
    m_actionname = tr("Nozzle ") +QString::number( m_nNumber);
}

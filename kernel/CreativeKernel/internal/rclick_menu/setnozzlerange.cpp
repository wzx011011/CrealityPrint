#include "setnozzlerange.h"
#include "nozzleaction.h"
#include <QSettings>

#include "kernel/translator.h"
#include "rclickmenuinterface.h"
SetNozzleRange::SetNozzleRange()
{
    m_actionname = tr("Set Nozzle Range");
    m_actionNameWithout = "Set Nozzle Range";
    m_bSubMenu = true;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

}
SetNozzleRange::~SetNozzleRange()
{}

void SetNozzleRange::slotLanguageChanged()
{
    m_actionname = tr("Set Nozzle Range");
}
QAbstractListModel* SetNozzleRange::subMenuActionmodel()
{
    return m_actionModelList;
}
void SetNozzleRange::updateActionModel()
{
    if(nullptr == m_actionModelList)
    {
        m_actionModelList = new ActionCommandModel(this);
    }
    m_actionModelList->removeAllCommand();
    for(int i=0; i<m_nNozzleCount;i++)
    {
        NozzleAction *pAction = new NozzleAction(i);
        m_actionModelList->addCommand(pAction);
    }
}

void SetNozzleRange::setNozzleCount(int nCount)
{
    m_nNozzleCount = nCount;
    updateActionModel();
}

bool SetNozzleRange::enabled()
{
    return true;
}

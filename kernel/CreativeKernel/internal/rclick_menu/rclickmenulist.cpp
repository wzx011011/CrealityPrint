#include "rclickmenulist.h"
#include "menu/actioncommand.h"
#include"internal/rclick_menu/deletemodelaction.h"
#include"internal/rclick_menu/importmodelaction.h"
#include"internal/rclick_menu/cloneaction.h"
#include"internal/rclick_menu/selectallaction.h"
#include"internal/rclick_menu/clearallaction.h"
#include"internal/rclick_menu/resetallaction.h"
#include"internal/rclick_menu/onesideasbottomface.h"
#include"internal/rclick_menu/placeonprinterbed.h"
#include"internal/rclick_menu/mergemodelaction.h"
#include"internal/rclick_menu/splitmodelaction.h"
#include "internal/rclick_menu/setnozzlerange.h"
#include"internal/rclick_menu/uploadmodelaction.h"
#include "internal/rclick_menu/layoutcommand.h"
RClickMenuList *RClickMenuList:: m_pComInstance = nullptr;
RClickMenuList *RClickMenuList:: getInstance()
{
    if (m_pComInstance == nullptr)
    {
        m_pComInstance = new RClickMenuList();
    }
    return m_pComInstance;
}

RClickMenuList::RClickMenuList(QObject *parent) : QObject(parent)
{
    m_varCommandList.clear();
    if(m_varCommandList.isEmpty())
    {
        startAddCommads();
    }
}
void RClickMenuList::addActionCommad(ActionCommand *pAction)
{
    if (pAction)
    {
        m_varCommandList.push_back(pAction);
    }
}
//void RClickMenuList::doRightClick(bool bEnable)
//{
//    emit sigRightClick(bEnable);

//}
void RClickMenuList::addActionCommad(ActionCommand *pAction,int index)
{
    if(pAction == nullptr)return ;
    if(index > m_varCommandList.size())return;

    m_varCommandList.insert(index,pAction);
}
//
void RClickMenuList::addActionCommad(ActionCommand *pAction,QString strInfo)
{
    if(pAction == nullptr || m_varCommandList.isEmpty() )return ;
    int index = indexOfString(strInfo);
    m_varCommandList.insert(index+1,pAction);
}

int RClickMenuList::indexOfString(QString strInfo)
{
    int index = -1;
    if(m_varCommandList.isEmpty())return index;
    for(int i = 0 ; i < m_varCommandList.size(); i++)
    {
        if(m_varCommandList[i]->name() == strInfo)
        {
            index = i;
            break;
        }
    }
    return  index;
}

void RClickMenuList::addActionCommads(QList< ActionCommand *>listAction)
{
    if(listAction.isEmpty())return;
    foreach (auto var, listAction)
    {
        m_varCommandList.push_back(var);
    }

}

void RClickMenuList::removeActionCommand(ActionCommand* command)
{
    int index = m_varCommandList.indexOf(command);
    if (index >= 0 && index < m_varCommandList.size())
    {
        m_varCommandList.removeAt(index);
    }
}

QList<ActionCommand *>RClickMenuList::getActionCommandList()
{
    return m_varCommandList;
}

void RClickMenuList::startAddCommads()
{
    ImportModelAction *pFile = new ImportModelAction();
    addActionCommad(pFile);

    DeleteModelAction *pFile2 = new DeleteModelAction();
    addActionCommad(pFile2);
    CloneAction *clone = new CloneAction();
    addActionCommad(clone);
    MergeModelAction* merge = new MergeModelAction();
    addActionCommad(merge);
    merge->setBSeparator(true);
    SplitModelAction* split = new SplitModelAction();
    addActionCommad(split);

    SelectAllAction *select = new SelectAllAction();
    addActionCommad(select);
    select->setBSeparator(true);
    LayoutCommand* layout = new LayoutCommand();
    addActionCommad(layout);

    ResetAllAction* resetall = new ResetAllAction();
    addActionCommad(resetall);

    ClearAllAction *clearAll = new ClearAllAction();
    addActionCommad(clearAll);
   
    PlaceOnPrinterBed *placeBed = new PlaceOnPrinterBed();
    addActionCommad(placeBed);
    placeBed->setBSeparator(true);
    OneSideAsBottomFace *oneSide = new OneSideAsBottomFace();
    addActionCommad(oneSide);

    UploadModelAction *uploadModel = new UploadModelAction();
    addActionCommad(uploadModel);
       
    SetNozzleRange *nozzle = SetNozzleRange::getInstance();
    addActionCommad(nozzle);
    nozzle->setNozzleCount(3);
    nozzle->updateActionModel();
}

#include "submenuviewsshow.h"
#include "kernel/translator.h"
#include "../menu/menuCmdDataType.h"
#include "viewshowcommand.h"
//SubMenuViewShow *SubMenuViewShow ::m_instance = nullptr;
//SubMenuViewShow *SubMenuViewShow:: getInstance()
//{
//    if (m_instance == nullptr)
//    {
//        m_instance = new SubMenuViewShow();
//    }
//    return m_instance;
//}

SubMenuViewShow::SubMenuViewShow()
{
    m_actionname = tr("View Show");
    m_actionNameWithout = "View Show";
    m_shortcut = "";      //不能有空格
    m_source = "";
    m_eParentMenu = eMenuType_View;
    m_bSubMenu = true;

    initActionModel();


    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
SubMenuViewShow::~SubMenuViewShow()
{

}
void SubMenuViewShow::slotLanguageChanged()
{
    m_actionname = tr("View Show");
}
QAbstractListModel* SubMenuViewShow::subMenuActionmodel()
{
    return m_actionModelList;
}
void SubMenuViewShow::initActionModel()
{
    if(nullptr == m_actionModelList)
    {
        m_actionModelList = new ActionCommandModel(this);
    }

    ViewShowCommand *pFront = new ViewShowCommand(eFrontViewShow);
    pFront->setParent(this);
    m_actionModelList->addCommand(pFront);
    ViewShowCommand *pBack = new ViewShowCommand(eBackViewShow);
    pBack->setParent(this);
    m_actionModelList->addCommand(pBack);
    ViewShowCommand *pLeft = new ViewShowCommand(eLeftViewShow);
    pLeft->setParent(this);
    m_actionModelList->addCommand(pLeft);
    ViewShowCommand *pRight = new ViewShowCommand(eRightViewShow);
    pRight->setParent(this);
    m_actionModelList->addCommand(pRight);
    ViewShowCommand *pTop = new ViewShowCommand(eTopViewShow);
    pTop->setParent(this);
    m_actionModelList->addCommand(pTop);
    ViewShowCommand *pBottom = new ViewShowCommand(eBottomViewShow);
    pBottom->setParent(this);
    m_actionModelList->addCommand(pBottom);
    ViewShowCommand *pPerspective= new ViewShowCommand(ePerspectiveViewShow);
    pPerspective->setParent(this);
    m_actionModelList->addCommand(pPerspective);
    ViewShowCommand *pOrthographic = new ViewShowCommand(eOrthographicViewShow);
    pOrthographic->setParent(this);
    m_actionModelList->addCommand(pOrthographic);
}

//Update recent file list

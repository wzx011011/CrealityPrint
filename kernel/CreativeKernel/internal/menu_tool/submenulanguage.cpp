#include "submenulanguage.h"
#include "languagecommand.h"
#include <QSettings>
#include "kernel/translator.h"
SubMenuLanguage::SubMenuLanguage()
{
    m_actionname = tr("Language");
    m_actionNameWithout = "Language";
    m_eParentMenu = eMenuType_Tool;
    m_bSubMenu = true;
    m_bCheckable = true;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    updateActionModel();
}
SubMenuLanguage::~SubMenuLanguage()
{}

//void SubMenuLanguage::slotRightClick(bool bEnable, QPoint point)
//{
//    m_enabled = bEnable;
//}
void SubMenuLanguage::slotLanguageChanged()
{
    m_actionname = tr("Language");
}
QAbstractListModel* SubMenuLanguage::subMenuActionmodel()
{
    return m_actionModelList;
}

ActionCommandModel * SubMenuLanguage::getSubMenuList()
{

    return m_actionModelList;
}
void SubMenuLanguage::updateActionModel()
{
    if(nullptr == m_actionModelList)
    {
        m_actionModelList = new ActionCommandModel(this);
    }
    m_actionModelList->removeAllCommand();
    LanguageCommand *chinese = new LanguageCommand(0,this);
    chinese->setParent(this);
    m_actionModelList->addCommand(chinese);
    LanguageCommand *english = new LanguageCommand(1,this);
    english->setParent(this);
    m_actionModelList->addCommand(english);
    LanguageCommand *traditional = new LanguageCommand(2,this);
    traditional->setParent(this);
    m_actionModelList->addCommand(traditional);
}

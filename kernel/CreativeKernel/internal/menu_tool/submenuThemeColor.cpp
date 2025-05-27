#include "submenuThemeColor.h"
#include "themecolorcommand.h"
#include <QSettings>
#include "kernel/translator.h"
SubMenuThemeColor::SubMenuThemeColor()
{
    m_actionname = tr("Theme color change");
    m_actionNameWithout = "Theme color change";
    m_eParentMenu = eMenuType_Tool;
    m_bSubMenu = true;
    m_bCheckable = true;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    updateActionModel();
}
SubMenuThemeColor::~SubMenuThemeColor()
{}

void SubMenuThemeColor::slotLanguageChanged()
{
    m_actionname = tr("Theme color change");
}
QAbstractListModel* SubMenuThemeColor::subMenuActionmodel()
{
    return m_actionModelList;
}

ActionCommandModel * SubMenuThemeColor::getSubMenuList()
{

    return m_actionModelList;
}
void SubMenuThemeColor::updateActionModel()
{
    if(nullptr == m_actionModelList)
    {
        m_actionModelList = new ActionCommandModel(this);
    }
    m_actionModelList->removeAllCommand();
    ThemeColorCommand * darkTheme = new ThemeColorCommand(0,this);
    m_actionModelList->addCommand(darkTheme);
    ThemeColorCommand* lightTheme = new ThemeColorCommand(1, this);
    m_actionModelList->addCommand(lightTheme);
}

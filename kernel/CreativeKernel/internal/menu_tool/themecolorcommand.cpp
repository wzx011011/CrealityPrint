#include "themecolorcommand.h"
#include "kernel/kernelui.h"
#include <QDebug>
#include <QSettings>
#include "kernel/translator.h"

ThemeColorCommand::ThemeColorCommand(QObject* parent)
    :ActionCommand(parent)
{
    m_actionname = tr("Theme color change");
    m_actionNameWithout = "Theme color change";
    m_eParentMenu = eMenuType_Tool;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

    disconnect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotLanguageChanged()));
    connect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotLanguageChanged()));

}

ThemeColorCommand::ThemeColorCommand(int nType,QObject* parent)
 :ActionCommand(parent)
{
    m_ntype = nType;

    switch (m_ntype) {
        case 0 : m_actionname = tr("Dark Theme");
        break;
        case 1: m_actionname = tr("Light Theme");
        break;
    }
    updateChecked();
    m_eParentMenu = eMenuType_Tool;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

    disconnect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotLanguageChanged()));
    connect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotLanguageChanged()));
}

void ThemeColorCommand::updateChecked()
{
    QSettings setting;
    setting.beginGroup("themecolor_config");
    int nThemeType = setting.value("themecolor_config",0).toInt();
    setting.endGroup();
    if((m_ntype ==0 && nThemeType == 0)
            || (m_ntype ==1 && nThemeType == 1))
    {
        //m_bChecked = true;
        setChecked(true);
    }
    else
    {
        //m_bChecked = false;
        setChecked(false);
    }
}

ThemeColorCommand::~ThemeColorCommand()
{
}

void ThemeColorCommand::execute()
{
    qDebug() << "change theme color";
    QSettings setting;
    setting.beginGroup("themecolor_config");
    setting.setValue("themecolor_config", m_ntype);
    setting.endGroup();
    getKernelUI()->changeTheme(m_ntype);
    getKernelUI()->changeMenuTheme();
}

void ThemeColorCommand::slotLanguageChanged()
{
    updateChecked();
    switch (m_ntype) {
        case 0 : m_actionname = tr("Dark Theme");
        break;
        case 1: m_actionname = tr("Light Theme");
        break;
    }
}



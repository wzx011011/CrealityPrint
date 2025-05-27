#include "languagecommand.h"
#include <interface/spaceinterface.h>
#include <interface/modelinterface.h>
#include "kernel/kernelui.h"
#include <QDebug>
#include <QSettings>
#include <qapplication.h>
#include "kernel/translator.h"
#include "kernel/projectinfoui.h"
#include "submenulanguage.h"
LanguageCommand::LanguageCommand(QObject* parent)
    :ActionCommand(parent)
{
    m_actionname = tr("Language");
    m_actionNameWithout = "Language";
    m_eParentMenu = eMenuType_Tool;
  //  m_fMinutes = 5.0;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

}

LanguageCommand::LanguageCommand(int nType,QObject* parent)
{
    m_ntype = nType;

    switch (m_ntype) {
    case 0: m_actionname = "English";//tr("English");
        break;
    case 1: m_actionname = "简体中文";//tr("Chinese");//m_actionname = tr("English");
        break;
    case 2: m_actionname = "繁體中文";//tr("Traditional");
        break;
    }

    updateChecked();
    m_eParentMenu = eMenuType_Tool;
  //  m_fMinutes = 5.0;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}

void LanguageCommand::updateChecked()
{
    QSettings setting;
    setting.beginGroup("language_perfer_config");
    QString strLanguageType = setting.value("language_type","en.ts").toString();
    setting.endGroup();
    if((m_ntype ==0 && strLanguageType == "en.ts")
            || (m_ntype ==1 && strLanguageType == "zh_CN.ts")
                || (m_ntype ==2 && strLanguageType == "zh_TW.ts"))
    {
        m_bChecked = true;
    }
    else
    {
        m_bChecked = false;
    }
}

LanguageCommand::~LanguageCommand()
{
}

void LanguageCommand::execute()
{
    switch (m_ntype) {
        case 0 : getKernelUI()->requestUpdateLanguage("en.ts",0);
        break;
        case 1: getKernelUI()->requestUpdateLanguage("zh_CN.ts",0);
        break;
        case 2: getKernelUI()->requestUpdateLanguage("zh_TW.ts",0);
        break;
    }

}

void LanguageCommand::requestMenuDialog()
{
     getKernelUI()->requestMenuDialog(this,"languageObj");
}

//int LanguageCommand::currentLanguageIndex()
//{
//    QSettings setting;
//    setting.beginGroup("language_perfer_config");
//    QString strLanguageType = setting.value("language_type","en.ts").toString();
//    setting.endGroup();
//    int index = m_keyValueMap.value(strLanguageType);
//    m_nLastLanguageIndex = index;
//    return index;

void LanguageCommand::slotLanguageChanged()
{
    updateChecked();
    switch (m_ntype) {
    case 0: m_actionname = "English";//tr("English");
        break;
        case 1: m_actionname = "简体中文";//tr("Chinese");
        break;
        case 2: m_actionname = "繁體中文";//tr("Traditional");
        break;
    }
}


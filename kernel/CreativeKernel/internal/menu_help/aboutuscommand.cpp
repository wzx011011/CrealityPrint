#include "aboutuscommand.h"
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>
#include "kernel/translator.h"
#include "kernel/kernelui.h"
AboutUsCommand::AboutUsCommand()
{
    m_actionname = tr("About Us");
    m_actionNameWithout = "About Us";
    m_eParentMenu = eMenuType_Help;

    //menu need to do change language opreator
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}


void AboutUsCommand::slotLanguageChanged()
{
    m_actionname = tr("About Us");
}
AboutUsCommand::~AboutUsCommand()
{}
void AboutUsCommand::execute()
{
//    QSettings setting;
//    setting.beginGroup("language_perfer_config");
//    QString strLanguageType = setting.value("language_type","en.ts").toString();
//    setting.endGroup();
//    if(strLanguageType == "en.ts")
//    {
//       QDesktopServices::openUrl(QUrl("https://www.creality.com/about-us"));
//    }
//    else if(strLanguageType == "zh_CN.ts")
//    {
//        QDesktopServices::openUrl(QUrl("https://www.cxsw3d.com/gongsijieshao.html"));

//    }
    requestMenuDialog();
}

void AboutUsCommand::requestMenuDialog()
{
     getKernelUI()->requestMenuDialog(this,"aboutUsObj");
}
QString AboutUsCommand::getWebsite()
{
    QSettings setting;
    setting.beginGroup("language_perfer_config");
    QString strLanguageType = setting.value("language_type","en.ts").toString();
    setting.endGroup();
    if(strLanguageType == "en.ts")
    {
        return "https://www.creality.com/about-us";
    }
    else if(strLanguageType == "zh_CN.ts")
    {
        return "https://www.creality.cn/about";
    }
    else if(strLanguageType == "zh_TW.ts")
    {
        return "https://www.creality.cn/about";
    }
    return "";
}
QString AboutUsCommand::getCurrentVersion()
{
    return getKernelUI()->currentVersion();
}

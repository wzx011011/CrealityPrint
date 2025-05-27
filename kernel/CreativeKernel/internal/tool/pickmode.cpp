#include "pickmode.h"
#include "kernel/translator.h"
#include "kernel/kernelui.h"

using namespace creative_kernel;
PickMode::PickMode(QObject* parent)
	:ToolCommand(parent)
{
    orderindex=0;
    m_name = tr("Pick");
    /*m_enabledIcon = "qrc:/UI/photo/pick.png";
    m_pressedIcon = "qrc:/UI/photo/pick_d.png";
    m_disableIcon = "qrc:/UI/photo/pick_f.png";*/
    slotThemeChanged();
    m_pickOp = nullptr;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

    disconnect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
    connect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));

}

PickMode::~PickMode()
{
}
void PickMode::slotLanguageChanged()
{
    m_name = tr("Pick");
}

void PickMode::slotThemeChanged()
{
    QSettings setting;
    setting.beginGroup("themecolor_config");
    int nThemeType = setting.value("themecolor_config", 0).toInt();
    setting.endGroup();

    setEnabledIcon(nThemeType == 0 ? "qrc:/UI/photo/pick.png" : "qrc:/UI/photo/pick2.png");
    m_pressedIcon = nThemeType == 0 ? "qrc:/UI/photo/pick_d.png" : "qrc:/UI/photo/pick_d.png";
    m_disableIcon = nThemeType == 0 ? "qrc:/UI/photo/pick.png" : "qrc:/UI/photo/pick.png";

    //AbstractKernelUI::getSelf()->switchPickMode();
}

void PickMode::execute()
{
    if (!m_pickOp)
    {
        m_pickOp = new PickOp(this);

    }
    setVisOperationMode(m_pickOp);


}

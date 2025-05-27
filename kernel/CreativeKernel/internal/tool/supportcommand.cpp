#include "supportcommand.h"

#include "data/modeln.h"

#include "interface/selectorinterface.h"
#include "interface/commandoperation.h"
#include "interface/jobsinterface.h"

#include "utils/modelpositioninitializer.h"

#include <QtCore/QDebug>
#include "interface/visualsceneinterface.h"
#include "kernel/translator.h"
#include "data/fdmsupportgroup.h"

#include "kernel/kernelui.h"
#include "us/slicermanager.h"
#include "interface/gadatainterface.h"
using namespace creative_kernel;
SupportCommand::SupportCommand(QObject* parent)
    :ToolCommand(parent)
{
    orderindex = 5;
    m_name = tr("Support");
    m_source = "qrc:/CrealityUI/secondqml/CusSupportPanel.qml";
    slotThemeChanged();

    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

    disconnect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
    connect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
}

SupportCommand::~SupportCommand()
{
}
void SupportCommand::slotLanguageChanged()
{
    m_name = tr("Support");
}

void SupportCommand::slotThemeChanged()
{
    QSettings setting;
    setting.beginGroup("themecolor_config");
    int nThemeType = setting.value("themecolor_config", 0).toInt();
    setting.endGroup();

    setEnabledIcon(nThemeType == 0 ? "qrc:/UI/photo/supporticon.png" : "qrc:/UI/photo/supporticon2.png");
    m_pressedIcon = nThemeType == 0 ? "qrc:/UI/photo/supporticon_d.png" : "qrc:/UI/photo/supporticon_d.png";
    m_disableIcon = nThemeType == 0 ? "qrc:/UI/photo/supporticon.png" : "qrc:/UI/photo/supporticon2.png";
}

bool SupportCommand::isSelect()
{
    QList<ModelN*> selections = selectionms();
    if (selections.size() > 0)
    {
        return true;
    }else
    {
        return false;
    }
}

void SupportCommand::execute()
{
    QObject* pmainWinbj = AbstractKernelUI::getSelf()->getUI("UIAppWindow");
    QMetaObject::invokeMethod(pmainWinbj, "supportTabBtnClicked");
    creative_kernel::sendDataToGA("Support", "Support (Button)");
}

bool SupportCommand::checkSelect()
{
    QList<ModelN*> selections = selectionms();
    if (selections.size() > 0)
    {
        return true;
    }
    return false;
}

#include "submenuprintercontrol.h"
#include "printercontrolcommand.h"
#include <QSettings>
#include "kernel/translator.h"
#include <QDesktopServices>
#include <Qurl>
#include "crealitycloud/cxnetworkmanager.h"
#include <string>

SubMenuPrinterControl::SubMenuPrinterControl()
{
    m_actionname = tr("Creality Cloud Printing");
    m_actionNameWithout = "Creality Cloud Print";
    m_eParentMenu = eMenuType_PrinterControl;
    //m_bSubMenu = true;
    //m_bCheckable = true;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    //initActionModel();
}

SubMenuPrinterControl::~SubMenuPrinterControl()
{

}

void SubMenuPrinterControl::slotLanguageChanged()
{
    m_actionname = tr("Creality Cloud Printing");
}

QAbstractListModel* SubMenuPrinterControl::subMenuActionmodel()
{
    return m_actionModelList;
}

void SubMenuPrinterControl::execute()
{
    creative_kernel::getcxNetworkManager()->getSSOToken(
        [=](std::string token) {
            QDesktopServices::openUrl(QUrl((creative_kernel::getcxNetworkManager()->getCloudUrl() + "/my-print?pageType=job&slice-token=" + token).c_str()));
        }
    );
}

void SubMenuPrinterControl::initActionModel()
{
    if(nullptr == m_actionModelList)
    {
        m_actionModelList = new ActionCommandModel(this);
    }

    PrinterConrtolCommand* single = new PrinterConrtolCommand(0);
    m_actionModelList->addCommand(single);
    PrinterConrtolCommand* group = new PrinterConrtolCommand(1);
    m_actionModelList->addCommand(group);
}

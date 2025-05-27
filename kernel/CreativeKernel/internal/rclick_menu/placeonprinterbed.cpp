#include "placeonprinterbed.h"

#include <QSettings>

#include "kernel/kernelui.h"
#include "kernel/translator.h"
#include "operation/translateop.h"

#include "interface/commandoperation.h"
#include "interface/selectorinterface.h"
PlaceOnPrinterBed::PlaceOnPrinterBed()
{
    m_actionname = tr("Place On PrinterBed");
    m_actionNameWithout = "Place On PrinterBed";
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

    if (!m_translateOp)
    {
        m_translateOp = new TranslateOp(getKernelUI(), this);
    }
}
PlaceOnPrinterBed::~PlaceOnPrinterBed()
{
    delete m_translateOp;

}

void PlaceOnPrinterBed:: execute()
{
    creative_kernel::cmdPlaceOnPrinterBed();
}

bool PlaceOnPrinterBed::enabled()
{
    return creative_kernel::selectionms().size() > 0;
}

void PlaceOnPrinterBed::slotLanguageChanged()
{
    m_actionname = tr("Place On PrinterBed");
}

#include "manageprinter.h"
#include <interface/spaceinterface.h>
#include <interface/modelinterface.h>
#include "kernel/kernelui.h"

#include <QDebug>

#include <QtQml/QQmlProperty>
#include "kernel/translator.h"
#include "def/cryptfiledevice.h"
#include "us/slicermanager.h"
#include "kernel/abstractkernel.h"
ManagePrinter::ManagePrinter(QObject* parent)
    :ActionCommand(parent)
    , m_obj(nullptr)
{
    m_actionname = tr("Manage Printer");
    m_actionNameWithout = "Manage Printer";
    m_eParentMenu = eMenuType_Tool;

    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}

ManagePrinter::~ManagePrinter()
{

}

void ManagePrinter::slotLanguageChanged()
{
    m_actionname = tr("Manage Printer");
}

void ManagePrinter::execute()
{
    QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();
    QObject* pRoot = engine->rootObjects().first();
    QObject* pSelectMachine = pRoot->findChild<QObject*>("managerPrinterDlg");
    QQmlProperty::write(pSelectMachine, "visible", "true");
}

void ManagePrinter::requestMenuDialog(QObject* receiver)
{
    //QMetaObject::invokeMethod(m_obj, "init", Q_ARG(QVariant, QVariant::fromValue(receiver)));
    //QMetaObject::invokeMethod(m_obj, "writeValue");
}



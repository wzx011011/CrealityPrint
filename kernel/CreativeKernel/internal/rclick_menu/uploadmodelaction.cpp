#include "uploadmodelaction.h"

#include <QSettings>
#include "kernel/kernelui.h"
#include "utils/meshloaderwrapper.h"
#include "kernel/translator.h"
#include "interface/selectorinterface.h"
#include "interface/gadatainterface.h"

UploadModelAction::UploadModelAction()
{
    m_actionname = tr("Upload Model");
    m_actionNameWithout = "Upload Model";
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));


}
UploadModelAction::~UploadModelAction()
{
}

void UploadModelAction:: execute()
{
    QString str = "modelUpload";

    QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();
    QObject* pRoot = engine->rootObjects().first();
    QObject* rightMenu = pRoot->findChild<QObject*>("rightbtnmenu");

    if(!rightMenu)
        return;
    QMetaObject::invokeMethod(rightMenu, "uploadModelBtnClick", Q_ARG(QVariant, QVariant::fromValue(str)));
    creative_kernel::sendDataToGA("Model Upload", "Modle Upload (Right Click)");
}

bool UploadModelAction::enabled()
{
    return creative_kernel::selectionms().size() > 0;
}

void UploadModelAction::slotLanguageChanged()
{
    m_actionname = tr("Upload Model");
}

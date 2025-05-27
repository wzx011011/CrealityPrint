#include "deletemodelaction.h"

#include <QSettings>
#include "interface/loadinterface.h"
#include "kernel/kernelui.h"
#include "utils/meshloaderwrapper.h"
#include "kernel/translator.h"
#include "interface/modelinterface.h"
#include "interface/selectorinterface.h"
#include "interface/spaceinterface.h"
#include "inout/cxopenandsavefilemanager.h"

DeleteModelAction::DeleteModelAction()
{
    m_actionname = tr("Delete Model");
    m_actionNameWithout = "Delete Model";
    m_strMessageText = tr("Do you Want to Delete SelectModel?");
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));


    qDebug()<<"DeleteModelAction init success!!" ;
}
DeleteModelAction::~DeleteModelAction()
{}
void DeleteModelAction:: execute()
{
    if(isPopPage())
        requestMenuDialog();
    else
    {
        accept();
    }
}

void DeleteModelAction::slotLanguageChanged()
{
    m_actionname = tr("Delete Model");
    m_strMessageText = tr("Do you Want to Delete SelectModel?");
}
void DeleteModelAction::requestMenuDialog()
{
     getKernelUI()->requestMenuDialog(this,"deletemessageDlg"/*"messageDlg"*/);
}

QString DeleteModelAction::getMessageText()
{
    return m_strMessageText;
}

Q_INVOKABLE void DeleteModelAction::writeReg(bool flag)
{
    QSettings settings;
    QString path = "UIparam/isPop";
    settings.setValue(path, QVariant(flag));
}

Q_INVOKABLE bool DeleteModelAction::isPopPage()
{
    QSettings setting;
    setting.beginGroup("UIparam");
    bool flag = setting.value("isPop", true).toBool();
    setting.endGroup();

    return flag;
}

void DeleteModelAction::accept()
{
    using namespace creative_kernel;
    removeSelectionModels();
    checkModelRange();
    AbstractKernelUI::getSelf()->switchPickMode();//by TCJ

    //set LastOpenFileName after delete model
    QList<ModelN*> models = modelns();
    if (models.size() > 0)
    {
        creative_kernel::ModelN* model = models[0];

        CXFILE.setLastOpenFileName(model->objectName());
    }
}

bool DeleteModelAction::enabled()
{
    QList<ModelN*> selections = selectionms();
    if (selections.size() > 0)
    {
        return true;
    }
    return false;
}

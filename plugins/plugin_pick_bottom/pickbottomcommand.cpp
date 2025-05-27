#include "pickbottomcommand.h"
#include "pickbottomop.h"
#include "interface/selectorinterface.h"
#include "interface/visualsceneinterface.h"
#include "kernel/translator.h"
#include "kernel/abstractkernel.h"

#include "kernel/kernelui.h"
#include "interface/gadatainterface.h"

using namespace creative_kernel;
PickBottomCommand::PickBottomCommand(QObject* parent)
	:ToolCommand(parent)
	, m_op(nullptr)
{
    m_name = tr("P Bottom");
   /* m_enabledIcon = "qrc:/UI/photo/pickBtn.png";
    m_pressedIcon = "qrc:/UI/photo/pickBtn_d.png";
    m_disableIcon = "qrc:/UI/photo/pickBtn_f.png";*/
    slotThemeChanged();

    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

    disconnect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
    connect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
}

PickBottomCommand::~PickBottomCommand()
{
}
void PickBottomCommand::slotLanguageChanged()
{
    m_name = tr("P Bottom");
}

void PickBottomCommand::slotThemeChanged()
{
    QSettings setting;
    setting.beginGroup("themecolor_config");
    int nThemeType = setting.value("themecolor_config", 0).toInt();
    setting.endGroup();

    setEnabledIcon(nThemeType == 0 ? "qrc:/UI/photo/pickBtn.png" : "qrc:/UI/photo/pickBtn2.png");
    m_pressedIcon = nThemeType == 0 ? "qrc:/UI/photo/pickBtn_d.png" : "qrc:/UI/photo/pickBtn_d.png";
    m_disableIcon = nThemeType == 0 ? "qrc:/UI/photo/pickBtn.png" : "qrc:/UI/photo/pickBtn.png";
}

void PickBottomCommand::slotMouseLeftClicked()
{
    message();
}

bool PickBottomCommand::message()
{
    if (m_op->getMessage())
    {
        getKernelUI()->requestMenuDialog(this, "deleteSupportDlg");
    }

    return true;
}

void PickBottomCommand::setMessage(bool isRemove)
{
    m_op->setMessage(isRemove);
}

void PickBottomCommand::execute()
{
	if (!m_op)
	{
		m_op = new PickBottomOp(this);
        connect(m_op, SIGNAL(mouseLeftClicked()), this, SLOT(slotMouseLeftClicked()));
	}

    if (!m_op->getShowPop())
    {
        setVisOperationMode(m_op);
    }
    else
    {
        setVisOperationMode(nullptr);
    }

    creative_kernel::sendDataToGA("Model Editing & Layout", "Place on Face");
}
bool PickBottomCommand::checkSelect()
{
    QList<ModelN*> selections = selectionms();
    if(selections.size()>0)
    {
        return true;
    }
    return false;
}

void PickBottomCommand::accept()
{
    setMessage(true);
}

void PickBottomCommand::cancel()
{
    setMessage(false);
    AbstractKernelUI::getSelf()->switchPickMode();
}
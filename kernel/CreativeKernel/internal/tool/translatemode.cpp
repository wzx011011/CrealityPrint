#include "translatemode.h"
#include "operation/translateop.h"

#include "interface/visualsceneinterface.h"
#include "interface/selectorinterface.h"
#include "kernel/translator.h"
#include "kernel/kernelui.h"

#include "us/slicermanager.h"
using namespace creative_kernel;
TranslateMode::TranslateMode(QObject* parent)
	:ToolCommand(parent)
	, m_translateOp(nullptr)
{
    orderindex = 0;
    m_name = tr("Move");
//	m_enabledIcon = "qrc:/kernel/images/move.png";
//	m_pressedIcon = "qrc:/kernel/images/move_d.png";
    /*m_enabledIcon = "qrc:/UI/photo/move.png";
    m_pressedIcon = "qrc:/UI/photo/move_d.png";
    m_disableIcon = "qrc:/UI/photo/move_f.png";*/
    slotThemeChanged();
    m_source = "qrc:/kernel/qml/MovePanel.qml";
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

    disconnect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
    connect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
}
bool TranslateMode::checkSelect()
{
    return  selected();
}
TranslateMode::~TranslateMode()
{
}
void TranslateMode::setMessage(bool isRemove)
{
    m_translateOp->setMessage(isRemove);
}
bool TranslateMode::message()
{
    if (m_translateOp->getMessage())
    {
        getKernelUI()->requestMenuDialog(this, "deleteSupportDlg");
    }

    return true;
}
void TranslateMode::accept()
{
    setMessage(true);
}
void TranslateMode::cancel()
{
    setMessage(false);
    AbstractKernelUI::getSelf()->switchPickMode();
}
void TranslateMode::slotLanguageChanged()
{
    m_name = tr("Move");
}

void TranslateMode::slotThemeChanged()
{
    QSettings setting;
    setting.beginGroup("themecolor_config");
    int nThemeType = setting.value("themecolor_config", 0).toInt();
    setting.endGroup();

    setEnabledIcon(nThemeType == 0 ? "qrc:/UI/photo/move.png" : "qrc:/UI/photo/move2.png");
    m_pressedIcon = nThemeType == 0 ? "qrc:/UI/photo/move_d.png" : "qrc:/UI/photo/move_d.png";
    m_disableIcon = nThemeType == 0 ? "qrc:/UI/photo/move.png" : "qrc:/UI/photo/move.png";
}

void TranslateMode::slotMouseLeftClicked()
{
    message();
}

void TranslateMode::execute()
{
    if (!m_translateOp)
    {
        m_translateOp = new TranslateOp(getKernelUI(), this);
        connect(m_translateOp, SIGNAL(positionChanged()), this, SIGNAL(positionChanged()));
        connect(m_translateOp, SIGNAL(mouseLeftClicked()), this, SLOT(slotMouseLeftClicked()));
    }

    if (!m_translateOp->getShowPop())
    {
        setVisOperationMode(m_translateOp);
    }
    else
    {
        setVisOperationMode(nullptr);
    }

    emit positionChanged();
}

void TranslateMode::reset()
{
    if (SlicerManager::instance().getDoingSlice())
    {
        return;
    }
	m_translateOp->reset();
}

QVector3D TranslateMode::position()
{
	return m_translateOp->position();
}

void TranslateMode::setPosition(QVector3D position)
{
    if (SlicerManager::instance().getDoingSlice())
    {
        return;
    }
	m_translateOp->setPosition(position);
}
void TranslateMode::center()
{
    if (SlicerManager::instance().getDoingSlice())
    {
        return;
    }
    m_translateOp->center();
}
void TranslateMode::bottom()
{
    if (SlicerManager::instance().getDoingSlice())
    {
        return;
    }
	m_translateOp->setBottom();
}

bool TranslateMode::selected()
{
	QList<ModelN*> selections = selectionms();
    if(selections.size()>0)
    {
        return true;
    }
    return false;
}
void TranslateMode::blockSignalMoveChanged(bool block)
{
    if(m_translateOp)
    {
        m_translateOp->blockSignals(block);
    }
}

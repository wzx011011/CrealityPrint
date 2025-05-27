#include "scalemode.h"
#include "operation/scaleop.h"

#include "interface/visualsceneinterface.h"
#include "interface/selectorinterface.h"
#include "kernel/translator.h"
#include "kernel/kernelui.h"

using namespace creative_kernel;
ScaleMode::ScaleMode(QObject* parent)
	:ToolCommand(parent)
	, m_op(nullptr)
{
    orderindex = 1;
    m_name = tr("Scale");
//	m_enabledIcon = "qrc:/kernel/images/scale.png";
//	m_pressedIcon = "qrc:/kernel/images/scale_d.png";
    /*m_enabledIcon = "qrc:/UI/photo/scale.png";
    m_pressedIcon = "qrc:/UI/photo/scale_d.png";
    m_disableIcon = "qrc:/UI/photo/scale_f.png";*/
    slotThemeChanged();
	m_source = "qrc:/kernel/qml/ScalePanel.qml";
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

    disconnect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
    connect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
}

ScaleMode::~ScaleMode()
{
    if (m_op != nullptr)
    {
        delete m_op;
        m_op = nullptr;
    }
}
void ScaleMode::setMessage(bool isRemove)
{
    m_op->setMessage(isRemove);
}
bool ScaleMode::message()
{
    if (m_op->getMessage())
    {
        getKernelUI()->requestMenuDialog(this, "deleteSupportDlg");
    }

    return true;
}
void ScaleMode::accept()
{
    setMessage(true);
}
void ScaleMode::cancel()
{
    setMessage(false);
    AbstractKernelUI::getSelf()->switchPickMode();
}

void ScaleMode::slotLanguageChanged()
{
    m_name = tr("Scale");
}

void ScaleMode::slotThemeChanged()
{
    QSettings setting;
    setting.beginGroup("themecolor_config");
    int nThemeType = setting.value("themecolor_config", 0).toInt();
    setting.endGroup();

    setEnabledIcon(nThemeType == 0 ? "qrc:/UI/photo/scale.png" : "qrc:/UI/photo/scale2.png");
    m_pressedIcon = nThemeType == 0 ? "qrc:/UI/photo/scale_d.png" : "qrc:/UI/photo/scale_d.png";
    m_disableIcon = nThemeType == 0 ? "qrc:/UI/photo/scale.png" : "qrc:/UI/photo/scale.png";
}

void ScaleMode::slotMouseLeftClicked()
{
    message();
}

void ScaleMode::execute()
{
	if (!m_op)
	{
		m_op = new ScaleOp(getKernelUI(), this);
        connect(m_op, SIGNAL(scaleChanged()), this, SIGNAL(scaleChanged()));
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
//	emit scaleChanged();
}

void ScaleMode::reset()
{
	m_op->reset();
}

QVector3D ScaleMode::size() const
{
//    qDebug()<<"m_op->box() =" << m_op->box();
    return m_op->box() * m_op->scale();
}

QVector3D ScaleMode::scale()
{
	return m_op->scale();
}
void ScaleMode::setScale(QVector3D scale)
{
    qDebug() << "setScale = " << scale;
	m_op->setScale(scale);
}
QVector3D ScaleMode::orgSize() const
{

    ScaleOp* op = (ScaleOp*)(m_op);
    return op->box()/*/op->scale()*/;

}
QVector3D ScaleMode::bindScale() const
{

    ScaleOp* op = (ScaleOp*)(m_op);
    return op->scale();
}
QVector3D ScaleMode::bindSize() const
{

    return m_op->box();
}


bool ScaleMode::uniformCheck()const
{
    return m_op->uniformCheck();
}
void ScaleMode::setUniformCheck(const bool check)
{
    m_op->setUniformCheck(check);
}

bool ScaleMode::checkSelect()
{
    QList<ModelN*> selections = selectionms();
    if(selections.size()>0)
    {
        return true;
    }
    return false;
}
void ScaleMode::blockSignalScaleChanged(bool block)
{
    if(m_op)
    {
        m_op->blockSignals(block);
    }
}

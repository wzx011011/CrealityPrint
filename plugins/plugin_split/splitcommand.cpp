#include "splitcommand.h"
#include "splitop.h"

#include "interface/visualsceneinterface.h"
#include "kernel/translator.h"
#include "interface/selectorinterface.h"

#include "kernel/kernel.h"
#include "kernel/kernelui.h"
//#include "data/modelspaceundo.h"
#include "qtuser3d/undo/undoproxy.h"
#include "kernel/abstractkernel.h"
#include "interface/gadatainterface.h"
using namespace creative_kernel;
SplitCommand::SplitCommand(QObject* parent)
	:ToolCommand(parent)
	, m_op(nullptr)
{
    m_name = tr("Split");
//    m_enabledIcon = "qrc:/kernel/images/part.png";
//    m_pressedIcon = "qrc:/kernel/images/part_d.png";
    /*m_enabledIcon = "qrc:/UI/photo/split.png";
    m_pressedIcon = "qrc:/UI/photo/split_d.png";
    m_disableIcon = "qrc:/UI/photo/split_f.png";*/
    slotThemeChanged();
	m_source = "qrc:/split/split/split.qml";
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

    disconnect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
    connect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
}

SplitCommand::~SplitCommand()
{
}
void SplitCommand::slotLanguageChanged()
{
    m_name = tr("Split");
}

void SplitCommand::slotThemeChanged()
{
    QSettings setting;
    setting.beginGroup("themecolor_config");
    int nThemeType = setting.value("themecolor_config", 0).toInt();
    setting.endGroup();

    setEnabledIcon(nThemeType == 0 ? "qrc:/UI/photo/split.png" : "qrc:/UI/photo/split2.png");
    m_pressedIcon = nThemeType == 0 ? "qrc:/UI/photo/split_d.png" : "qrc:/UI/photo/split_d.png";
    m_disableIcon = nThemeType == 0 ? "qrc:/UI/photo/split.png" : "qrc:/UI/photo/split.png";
}

void SplitCommand::slotMouseLeftClicked()
{
    message();
}

bool SplitCommand::message()
{
    if (m_op->getMessage())
    {
        getKernelUI()->requestMenuDialog(this, "deleteSupportDlg");
    }

    return true;
}

void SplitCommand::setMessage(bool isRemove)
{
    m_op->setMessage(isRemove);
}

void SplitCommand::execute()
{
	if (!m_op)
	{
		m_op = new SplitOp(this);
       connect(m_op, SIGNAL(posionChanged()), this, SIGNAL(onPositionChanged()));
       connect(m_op, SIGNAL(rotateAngleChanged()), this, SLOT(slotRotateAngleChanged()));
       connect(m_op, SIGNAL(mouseLeftClicked()), this, SLOT(slotMouseLeftClicked()));
           //rotateAngleChanged
	}
    
    if (!m_op->getShowPop())
    {
        //没有选择模型，不弹出操作界面
        /*QList<ModelN*> selections = selectionms();
        if (selections.size() < 1)
        {           
            return;
        }*/

        setVisOperationMode(m_op);
        //setDefauletDir(this->dir());
        setDefauletDir(QVector3D(0, 0, 0));
        setDefauletPos(this->position());
    }
    else
    {
        setVisOperationMode(nullptr);
    }
    creative_kernel::sendDataToGA("Model Editing & Layout", "Cut");
}
void SplitCommand::slotRotateAngleChanged()
{
    qDebug()<<"slotRotateAngleChanged!";
    emit onRotateAngleChanged();

}
void SplitCommand::split()
{
	if (m_op) m_op->split();
}


QVector3D SplitCommand::position()
{
    if (m_op)
        return m_op->plane().center;
    else
        return QVector3D(0,0,0);
}
QVector3D SplitCommand::defauletPos() {
    this->setPositon(m_DefauletPos);
    return m_DefauletPos;
}
QVector3D SplitCommand::defauletDir() {
    this->setDir(m_DefauletDir);
    return m_DefauletDir;
}
QVector3D SplitCommand::dir()
{
    if (m_op)
    {
        return m_op->planeRotateAngle();
        //return m_op->plane().dir;
    }
    else {
        return QVector3D(0,0,0);
    }
}


void SplitCommand::setPositon(QVector3D pos)
{
    if (m_op) m_op->setPlanePosition(pos);
}

void SplitCommand::setDir(QVector3D d)
{
    if (m_op) m_op->setPlaneDir(d);
}
void SplitCommand::setDefauletPos(QVector3D pos)
{
    m_DefauletPos = pos;
}
void SplitCommand::setDefauletDir(QVector3D pos)
{
    m_DefauletDir = pos;
}
bool SplitCommand::checkSelect()
{
    QList<ModelN*> selections = selectionms();
    if(selections.size()>0)
    {
        return true;
    }
    return false;
}
void SplitCommand::blockSignalSplitChanged(bool block)
{
    if (m_op)
    {
        m_op->blockSignals(block);
    }
}

void SplitCommand::undo()
{
    getKernel()->undoProxy()->undo();
}

void SplitCommand::accept()
{
    setMessage(true);
}

void SplitCommand::cancel()
{
    setMessage(false);
    AbstractKernelUI::getSelf()->switchPickMode();
}

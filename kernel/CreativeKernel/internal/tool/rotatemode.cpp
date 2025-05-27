#include "rotatemode.h"
#include "operation/rotateop.h"

#include "interface/visualsceneinterface.h"
#include "interface/selectorinterface.h"
#include "kernel/translator.h"
#include "kernel/kernelui.h"

using namespace creative_kernel;
RotateMode::RotateMode(QObject* parent)
	:ToolCommand(parent)
	, m_op(nullptr)
{
    orderindex = 3;
    m_name = tr("Rotate");

    /*m_enabledIcon = "qrc:/UI/photo/rotate.png";
    m_pressedIcon = "qrc:/UI/photo/rotate_d.png";
	m_disableIcon = "qrc:/UI/photo/rotate_f.png";*/
	slotThemeChanged();

	m_source = "qrc:/kernel/qml/RotatePanel.qml";
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

	disconnect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
	connect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));

}

RotateMode::~RotateMode()
{
	if (m_op != nullptr)
	{
		delete m_op;
		m_op = nullptr;
	}
}
void RotateMode::slotLanguageChanged()
{
    m_name = tr("Rotate");
}

void RotateMode::slotThemeChanged()
{
	QSettings setting;
	setting.beginGroup("themecolor_config");
	int nThemeType = setting.value("themecolor_config", 0).toInt();
	setting.endGroup();

	setEnabledIcon(nThemeType == 0 ? "qrc:/UI/photo/rotate.png" : "qrc:/UI/photo/rotate2.png");
	m_pressedIcon = nThemeType == 0 ? "qrc:/UI/photo/rotate_d.png" : "qrc:/UI/photo/rotate_d.png";
	m_disableIcon = nThemeType == 0 ? "qrc:/UI/photo/rotate.png" : "qrc:/UI/photo/rotate.png";
}

void RotateMode::slotMouseLeftClicked()
{
	message();
}

void RotateMode::setMessage(bool isRemove)
{
	m_op->setMessage(isRemove);
}

bool RotateMode::message()
{
	if (m_op->getMessage())
	{
		getKernelUI()->requestMenuDialog(this, "deleteSupportDlg");
	}
	
	return true;
}

void RotateMode::testMessage()
{
	qDebug() << "slots message";
	emit supportMessage();
}

void RotateMode::execute()
{
	if (!m_op)
	{
		m_op = new RotateOp(getKernelUI(), this);
		connect(m_op, SIGNAL(rotateChanged()), this, SIGNAL(rotateChanged()));
		connect(m_op, SIGNAL(supportMessage()), this, SLOT(testMessage()));
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
	emit rotateChanged();
}

void RotateMode::reset()
{
	m_op->reset();
}

QVector3D RotateMode::rotate()
{
	return m_op->rotate();
}

void RotateMode::setRotate(QVector3D position)
{
	m_op->setRotate(position);
}

Q_INVOKABLE void RotateMode::startRotate()
{
	m_op->startRotate();
}
bool RotateMode::checkSelect()
{
    QList<ModelN*> selections = selectionms();
    if(selections.size()>0)
    {
        return true;
    }
    return false;
}
void RotateMode::blockSignalScaleChanged(bool block)
{
    if(m_op)
    {
        m_op->blockSignals(block);
    }
}

void RotateMode::accept()
{
	setMessage(true);
}

void RotateMode::cancel()
{
	setMessage(false);
	AbstractKernelUI::getSelf()->switchPickMode();
}

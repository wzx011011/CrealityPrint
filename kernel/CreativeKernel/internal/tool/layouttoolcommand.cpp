#include "layouttoolcommand.h"

#include "data/modeln.h"

#include "interface/selectorinterface.h"
#include "interface/commandoperation.h"
#include "interface/jobsinterface.h"

#include "utils/modelpositioninitializer.h"

#include <QtCore/QDebug>
#include "interface/visualsceneinterface.h"
#include "kernel/translator.h"
#include "data/fdmsupportgroup.h"

#include "kernel/kernelui.h"
#include "us/slicermanager.h"
#include "interface/gadatainterface.h"
using namespace creative_kernel;
LayoutToolCommand::LayoutToolCommand(QObject* parent)
	:ToolCommand(parent)
{
    orderindex = 5;
    m_name = tr("Layout");

//    m_source = "qrc:/kernel/qml/Clone.qml";
    slotThemeChanged();

    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

    disconnect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
    connect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
}

LayoutToolCommand::~LayoutToolCommand()
{
    if (m_pickOp != nullptr)
    {
        delete m_pickOp;
        m_pickOp = nullptr;
    }
}
void LayoutToolCommand::slotLanguageChanged()
{
    m_name = tr("Layout");
}

void LayoutToolCommand::slotThemeChanged()
{
    QSettings setting;
    setting.beginGroup("themecolor_config");
    int nThemeType = setting.value("themecolor_config", 0).toInt();
    setting.endGroup();

    setEnabledIcon(nThemeType == 0 ? "qrc:/UI/photo/layouticon.png" : "qrc:/UI/photo/layouticon2.png");
    m_pressedIcon = nThemeType == 0 ? "qrc:/UI/photo/layouticon_d.png" : "qrc:/UI/photo/layouticon_d.png";
    m_disableIcon = nThemeType == 0 ? "qrc:/UI/photo/layouticon.png" : "qrc:/UI/photo/layouticon2.png";
}

bool LayoutToolCommand::isSelect()
{
    QList<ModelN*> selections = selectionms();
    if (selections.size() > 0)
    {
        return true;
    }else
	{
        return false;
    }
}

void LayoutToolCommand::execute()
{
    if (!m_pickOp)
    {
        m_pickOp = new PickOp(this);
    }
    setVisOperationMode(m_pickOp);

    AutoLayoutJob * layoutJob = new AutoLayoutJob();
    QList<JobPtr> jobs;
    jobs.push_back(JobPtr(layoutJob));
    creative_kernel::executeJobs(jobs);
}

bool LayoutToolCommand::checkSelect()
{
	QList<ModelN*> selections = selectionms();
	if (selections.size() > 0)
	{
		return true;
	}
	return false;
}

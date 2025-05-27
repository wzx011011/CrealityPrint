#include "clonecommand.h"

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
CloneCommand::CloneCommand(QObject* parent)
	:ToolCommand(parent)
{
    orderindex = 5;
    m_name = tr("Clone");

//	m_enabledIcon = "qrc:/kernel/images/clone.png";
//	m_pressedIcon = "qrc:/kernel/images/clone_d.png";
    m_layoutJob = NULL;

    /*m_enabledIcon = "qrc:/UI/photo/clone.png";
    m_pressedIcon = "qrc:/UI/photo/clone_d.png";
    m_disableIcon = "qrc:/UI/photo/clone_f.png";*/
    m_source = "qrc:/kernel/qml/Clone.qml";
    slotThemeChanged();

    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

    disconnect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
    connect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
}

CloneCommand::~CloneCommand()
{
}
void CloneCommand::slotLanguageChanged()
{
    m_name = tr("Clone");
}

void CloneCommand::slotThemeChanged()
{
    QSettings setting;
    setting.beginGroup("themecolor_config");
    int nThemeType = setting.value("themecolor_config", 0).toInt();
    setting.endGroup();

    setEnabledIcon(nThemeType == 0 ? "qrc:/UI/photo/clone.png" : "qrc:/UI/photo/clone2.png");
    m_pressedIcon = nThemeType == 0 ? "qrc:/UI/photo/clone_d.png" : "qrc:/UI/photo/clone_d.png";
    m_disableIcon = nThemeType == 0 ? "qrc:/UI/photo/clone.png" : "qrc:/UI/photo/clone.png";
}

bool CloneCommand::isSelect()
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

void CloneCommand::clone(int numb)
{
	us::GlobalSetting* gsets = SlicerManager::instance().globalsettings();
	bool isBelt = gsets->settings().value("machine_is_belt")->value().toBool();
	if (isBelt)
    { 
        cmdClone(numb);
        return;
    }
    else
    {
        try {
            cmdClone(numb);
        }
        catch (...)
        {
            qDebug() << "CloneCommand::clone, catch(...)";
            return;
        }

		m_layoutJob = new AutoLayoutJob(this);
		QList<JobPtr> jobs;
		jobs.push_back(JobPtr(m_layoutJob));
		executeJobs(jobs);
		AbstractKernelUI::getSelf()->switchPickMode();//by TCJ
    }
}

void CloneCommand::execute()
{
    if (!m_pickOp)
    {
        m_pickOp = new PickOp(this);

    }
    setVisOperationMode(m_pickOp);
    creative_kernel::sendDataToGA("Model Editing & Layout", "Clone");
}

bool CloneCommand::checkSelect()
{
	QList<ModelN*> selections = selectionms();
	if (selections.size() > 0)
	{
		return true;
	}
	return false;
}

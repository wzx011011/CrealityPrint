#include "mirrorcommand.h"

#include "interface/modelinterface.h"
#include "interface/selectorinterface.h"
#include "interface/visualsceneinterface.h"
MirrorToolCommand::MirrorToolCommand(QObject* parent)
	:ToolCommand(parent)
{
    orderindex=4;
	m_name = "Mirror";
//	m_enabledIcon = "qrc:/kernel/images/mirror.png";
//	m_pressedIcon = "qrc:/kernel/images/mirror_d.png";
    m_enabledIcon = "qrc:/UI/photo/mirror.png";
    m_pressedIcon = "qrc:/UI/photo/mirror.png";
	m_source = "qrc:/kernel/qml/MirrorPanel.qml";
}

MirrorToolCommand::~MirrorToolCommand()
{
}

void MirrorToolCommand::execute()
{

}

void MirrorToolCommand::mirrorX()
{
	creative_kernel::mirrorX(get(), true);
}

void MirrorToolCommand::mirrorY()
{
	creative_kernel::mirrorY(get(), true);
}

void MirrorToolCommand::mirrorZ()
{
	creative_kernel::mirrorZ(get(), true);
}

void MirrorToolCommand::reset()
{
	creative_kernel::mirrorReset(get(), true);
}

creative_kernel::ModelN* MirrorToolCommand::get()
{
	QList<creative_kernel::ModelN*> selections = creative_kernel::selectionms();
	return selections.size() > 0 ? selections.at(0) : nullptr;
}


#include "resetcommand.h"
#include <QDesktopServices>
#include <QUrl>

#include "interface/commandoperation.h"

using namespace creative_kernel;
ResetCommand::ResetCommand(QObject* parent)
	:ToolCommand(parent)
{
    orderindex = 4;
    m_name = tr("ResetModel");
    m_enabledIcon = "qrc:/kernel/images/reset.png";
    m_pressedIcon = "qrc:/kernel/images/reset.png";
}
ResetCommand::~ResetCommand()
{
}

void ResetCommand::execute()
{
    cmdReset();
}


#include "clearcommand.h"

ClearCommand::ClearCommand(QObject* parent)
	:ToolCommand(parent)
{
    orderindex=0;
	m_name = "Clear";
    m_enabledIcon = "qrc:/kernel/images/new.png";
    m_pressedIcon = "qrc:/kernel/images/new.png";
}

ClearCommand::~ClearCommand()
{
}

void ClearCommand::execute()
{
}

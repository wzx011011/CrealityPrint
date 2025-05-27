#include "settingscommand.h"

SettingsCommand::SettingsCommand(QObject* parent)
	:ToolCommand(parent)
{
    orderindex=0;
	m_name = "Clear";
    m_enabledIcon = "qrc:/kernel/images/setting.png";
    m_pressedIcon = "qrc:/kernel/images/setting.png";
}

SettingsCommand::~SettingsCommand()
{
}

void SettingsCommand::execute()
{
}

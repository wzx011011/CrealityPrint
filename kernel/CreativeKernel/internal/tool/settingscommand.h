#ifndef _NULLSPACE_SETTINGSCOMMAND_1591075293459_H
#define _NULLSPACE_SETTINGSCOMMAND_1591075293459_H
#include "qtuserqml/plugin/toolcommand.h"

class SettingsCommand: public ToolCommand
{
	Q_OBJECT
public:
    SettingsCommand(QObject* parent = nullptr);
    virtual ~SettingsCommand();

	Q_INVOKABLE void execute();
};
#endif // _NULLSPACE_CLEARCOMMAND_1591075293459_H

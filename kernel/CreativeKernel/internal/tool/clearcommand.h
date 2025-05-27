#ifndef _NULLSPACE_CLEARCOMMAND_1591075293459_H
#define _NULLSPACE_CLEARCOMMAND_1591075293459_H
#include "qtuserqml/plugin/toolcommand.h"

class ClearCommand: public ToolCommand
{
	Q_OBJECT
public:
	ClearCommand(QObject* parent = nullptr);
	virtual ~ClearCommand();

	Q_INVOKABLE void execute();
};
#endif // _NULLSPACE_CLEARCOMMAND_1591075293459_H

#ifndef _NULLSPACE_RESETCOMMAND_1594782203469_H
#define _NULLSPACE_RESETCOMMAND_1594782203469_H
#include "qtuserqml/plugin/toolcommand.h"
class ResetCommand: public ToolCommand
{
	Q_OBJECT
public:
	ResetCommand(QObject* parent = nullptr);
	virtual ~ResetCommand();
	Q_INVOKABLE void execute();
};
#endif // _NULLSPACE_RESETCOMMAND_1594782203469_H

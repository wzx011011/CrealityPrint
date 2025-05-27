#ifndef _NULLSPACE_MESHIMPORTCOMMAND_1589680074178_H
#define _NULLSPACE_MESHIMPORTCOMMAND_1589680074178_H
#include "qtuserqml/plugin/toolcommand.h"

class MeshImportCommand: public ToolCommand
{
	Q_OBJECT
public:
	MeshImportCommand(QObject* parent = nullptr);
	virtual ~MeshImportCommand();

	Q_INVOKABLE void execute();
};
#endif // _NULLSPACE_MESHIMPORTCOMMAND_1589680074178_H

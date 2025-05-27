#ifndef _NULLSPACE_MIRRORCOMMAND_1589621811479_H
#define _NULLSPACE_MIRRORCOMMAND_1589621811479_H
#include "qtuserqml/plugin/toolcommand.h"

namespace creative_kernel
{
	class ModelN;
}
class MirrorToolCommand: public ToolCommand
{
	Q_OBJECT
public:
	MirrorToolCommand(QObject* parent = nullptr);
	virtual ~MirrorToolCommand();

	Q_INVOKABLE void execute();
	Q_INVOKABLE void mirrorX();
	Q_INVOKABLE void mirrorY();
	Q_INVOKABLE void mirrorZ();
	Q_INVOKABLE void reset();

protected:
	creative_kernel::ModelN* get();
};
#endif // _NULLSPACE_MIRRORCOMMAND_1589621811479_H

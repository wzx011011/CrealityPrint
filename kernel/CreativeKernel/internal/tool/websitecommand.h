#ifndef _NULLSPACE_WEBSITECOMMAND_1591075293459_H
#define _NULLSPACE_WEBSITECOMMAND_1591075293459_H
#include "qtuserqml/plugin/toolcommand.h"

class WebsiteCommand: public ToolCommand
{
	Q_OBJECT
public:
    WebsiteCommand(QObject* parent = nullptr);
    virtual ~WebsiteCommand();

	Q_INVOKABLE void execute();
};
#endif // _NULLSPACE_CLEARCOMMAND_1591075293459_H

#ifndef _MERGEMODELCOMMAND_H
#define _MERGEMODELCOMMAND_H
#include "menu/actioncommand.h"
#include "qtuserqml/plugin/toolcommand.h"
class MergeModelCommand: public ActionCommand
{
	Q_OBJECT
public:
    MergeModelCommand(QObject* parent = nullptr);
    virtual ~MergeModelCommand();
	Q_INVOKABLE void execute();
private slots:
    void slotLanguageChanged();
};
#endif // _MERGEMODELCOMMAND_H

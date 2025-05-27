#ifndef _RESETACTIONCOMMAND_H
#define _RESETACTIONCOMMAND_H
#include "menu/actioncommand.h"
#include "qtuserqml/plugin/toolcommand.h"
class ResetActionCommand: public ActionCommand
{
	Q_OBJECT
public:
    ResetActionCommand(QObject* parent = nullptr);
    virtual ~ResetActionCommand();
	Q_INVOKABLE void execute();
private slots:
    void slotLanguageChanged();
};
#endif // _RESETACTIONCOMMAND_H

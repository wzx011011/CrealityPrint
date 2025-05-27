#ifndef _LAYOUTCOMMAND_1592892614853_H
#define _LAYOUTCOMMAND_1592892614853_H
#include "menu/actioncommand.h"

class LayoutCommand : public ActionCommand
{
	Q_OBJECT
public:
	LayoutCommand(QObject* parent = nullptr);
	virtual ~LayoutCommand();

	Q_INVOKABLE void execute();
	bool enabled() override;

private slots:
    void slotLanguageChanged();
};
#endif // _LAYOUTCOMMAND_1592892614853_H
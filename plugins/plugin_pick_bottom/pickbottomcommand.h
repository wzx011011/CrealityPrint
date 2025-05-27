#ifndef _NULLSPACE_PICKBOTTOMCOMMAND_1589850876397_H
#define _NULLSPACE_PICKBOTTOMCOMMAND_1589850876397_H
#include "qtuserqml/plugin/toolcommand.h"

class PickBottomOp;
class PickBottomCommand: public ToolCommand
{
	Q_OBJECT
public:
	PickBottomCommand(QObject* parent = nullptr);
	virtual ~PickBottomCommand();

	Q_INVOKABLE void execute();
    Q_INVOKABLE bool checkSelect() override;

	Q_INVOKABLE void accept();
	Q_INVOKABLE void cancel();

	void setMessage(bool isRemove);
	bool message();

private  slots:
    void slotLanguageChanged();
	void slotMouseLeftClicked();
	void slotThemeChanged();
protected:
	PickBottomOp* m_op;
};
#endif // _NULLSPACE_PICKBOTTOMCOMMAND_1589850876397_H

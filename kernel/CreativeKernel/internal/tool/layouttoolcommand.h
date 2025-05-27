#ifndef _NULLSPACE_LAYOUTTOOLCOMMAND_1589427153657_H
#define _NULLSPACE_LAYOUTTOOLCOMMAND_1589427153657_H
#include "qtuserqml/plugin/toolcommand.h"
#include "operation/pickop.h"
#include "job/autolayoutjob.h"
class LayoutToolCommand: public ToolCommand
{
	Q_OBJECT
public:
    LayoutToolCommand(QObject* parent = nullptr);
    virtual ~LayoutToolCommand();
    Q_INVOKABLE bool isSelect();
	Q_INVOKABLE void execute();
	Q_INVOKABLE bool checkSelect() override;
private slots:
    void slotLanguageChanged();
	void slotThemeChanged();
protected:
    PickOp* m_pickOp = nullptr;
};
#endif // _NULLSPACE_LAYOUTTOOLCOMMAND_1589427153657_H

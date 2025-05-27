#ifndef _NULLSPACE_SUPPORTCOMMAND_1589427153657_H
#define _NULLSPACE_SUPPORTCOMMAND_1589427153657_H
#include "qtuserqml/plugin/toolcommand.h"
#include "operation/pickop.h"
#include "job/autolayoutjob.h"
class SupportCommand: public ToolCommand
{
	Q_OBJECT
public:
    SupportCommand(QObject* parent = nullptr);
    virtual ~SupportCommand();
    Q_INVOKABLE bool isSelect();
	Q_INVOKABLE void execute();
	Q_INVOKABLE bool checkSelect() override;
private slots:
    void slotLanguageChanged();
	void slotThemeChanged();
protected:
    PickOp* m_pickOp = nullptr;
};
#endif // _NULLSPACE_SUPPORTCOMMAND_1589427153657_H

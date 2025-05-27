#ifndef _NULLSPACE_CLONECOMMAND_1589427153657_H
#define _NULLSPACE_CLONECOMMAND_1589427153657_H
#include "qtuserqml/plugin/toolcommand.h"
#include "operation/pickop.h"
#include "job/autolayoutjob.h"
class CloneCommand: public ToolCommand
{
	Q_OBJECT
public:
	CloneCommand(QObject* parent = nullptr);
	virtual ~CloneCommand();
    Q_INVOKABLE bool isSelect();
    Q_INVOKABLE void clone(int numb);
	Q_INVOKABLE void execute();
	Q_INVOKABLE bool checkSelect() override;
private slots:
    void slotLanguageChanged();
	void slotThemeChanged();
protected:
    PickOp* m_pickOp = nullptr;
	AutoLayoutJob* m_layoutJob;
};
#endif // _NULLSPACE_CLONECOMMAND_1589427153657_H

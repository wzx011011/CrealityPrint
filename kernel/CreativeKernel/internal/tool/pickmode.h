#ifndef _NULLSPACE_PICKMODE_1589765331924_H
#define _NULLSPACE_PICKMODE_1589765331924_H
#include "qtuserqml/plugin/toolcommand.h"
#include "interface/visualsceneinterface.h"
#include "operation/pickop.h"
class PickMode: public ToolCommand
{
	Q_OBJECT
public:
	PickMode(QObject* parent = nullptr);
	virtual ~PickMode();

	Q_INVOKABLE void execute();
private slots:
    void slotLanguageChanged();
	void slotThemeChanged();
protected:
    PickOp* m_pickOp;
};
#endif // _NULLSPACE_PICKMODE_1589765331924_H

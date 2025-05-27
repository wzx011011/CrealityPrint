#ifndef CLONEACTION_H
#define CLONEACTION_H
#include "menu/actioncommand.h"
#include <QPoint>
#include "job/autolayoutjob.h"
class CloneAction: public ActionCommand
{
	Q_OBJECT
public:
    CloneAction(QObject* parent = nullptr);
    virtual ~CloneAction();
    Q_INVOKABLE bool isSelect();
    Q_INVOKABLE void clone(int numb);
	Q_INVOKABLE void execute();
    bool enabled() override;
private slots:
    void slotLanguageChanged();

private:
    AutoLayoutJob* m_layoutJob;
};
#endif // CLONEACTION_H

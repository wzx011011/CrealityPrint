#ifndef SUBMENUPRINTERCONTROL_H
#define SUBMENUPRINTERCONTROL_H

#include <QObject>
#include "menu/actioncommand.h"
#include "menu/actioncommandmodel.h"
class SubMenuPrinterControl:public ActionCommand
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* subMenuActionmodel READ subMenuActionmodel CONSTANT)
public:
    SubMenuPrinterControl();
    ~SubMenuPrinterControl();
    void initActionModel();
    QAbstractListModel* subMenuActionmodel();

    Q_INVOKABLE void execute();
private:
    ActionCommandModel* m_actionModelList = nullptr;

private slots:
    void slotLanguageChanged();
};

#endif

#ifndef SUBMENUTHEMECOLOR_H
#define SUBMENUTHEMECOLOR_H

#include <QObject>
#include "menu/actioncommand.h"
#include "menu/actioncommandmodel.h"
class SubMenuThemeColor:public ActionCommand
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* subMenuActionmodel READ subMenuActionmodel CONSTANT)
public:
    SubMenuThemeColor();
    ~SubMenuThemeColor();
    void updateActionModel();
    QAbstractListModel* subMenuActionmodel();
    ActionCommandModel * getSubMenuList();

private:
    ActionCommandModel* m_actionModelList = nullptr;

private slots:
    void slotLanguageChanged();
};

#endif

#ifndef SUBMENULANGUAGE_H
#define SUBMENULANGUAGE_H

#include <QObject>
#include <QPoint>
#include "menu/actioncommand.h"
#include "menu/actioncommandmodel.h"
class SubMenuLanguage:public ActionCommand
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* subMenuActionmodel READ subMenuActionmodel CONSTANT)
public:
    SubMenuLanguage();
    ~SubMenuLanguage();
    void updateActionModel();
    QAbstractListModel* subMenuActionmodel();

    ActionCommandModel * getSubMenuList();

    static SubMenuLanguage * getInstance()
    {
        static SubMenuLanguage inst;
        return &inst;
    }
private:
    ActionCommandModel* m_actionModelList = nullptr;

private slots:
    void slotLanguageChanged();
};

#endif // SUBMENULANGUAGE_H

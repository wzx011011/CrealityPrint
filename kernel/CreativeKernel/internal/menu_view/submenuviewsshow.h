#ifndef SUBMENUVIEWSHOW_H
#define SUBMENUVIEWSHOW_H

#include <QObject>
#include "menu/actioncommand.h"
#include "menu/actioncommandmodel.h"
#define recentFileCount "Creality3DrecentFiles/numOfRecentFiles"
#define recentFileListId "Creality3DrecentFiles/fileList"

#include <QSettings>
class SubMenuViewShow : public ActionCommand
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* subMenuActionmodel READ subMenuActionmodel CONSTANT)
public:
    explicit SubMenuViewShow();
    virtual ~SubMenuViewShow();

    void initActionModel();
    QAbstractListModel* subMenuActionmodel();
//    static SubMenuViewShow * getInstance();
//signals:
private:
    ActionCommandModel* m_actionModelList = nullptr;

//    static SubMenuViewShow* m_instance;
private slots:
    void slotLanguageChanged();
};

#endif // SUBMENUVIEWSHOW_H

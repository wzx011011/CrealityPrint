#ifndef SUBMENURECENTFILES_H
#define SUBMENURECENTFILES_H

#include <QObject>
#include "menu/actioncommand.h"
//#include <QAbstractListModel>
#include "menu/actioncommandmodel.h"
#define recentFileCount "Creality3DrecentFiles/numOfRecentFiles"
#define recentFileListId "Creality3DrecentFiles/fileList"
#define Max_RecentFile_Size 10

#include <QSettings>
class SubMenuRecentFiles : public ActionCommand
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* subMenuActionmodel READ subMenuActionmodel CONSTANT)
public:
    explicit SubMenuRecentFiles();
    virtual ~SubMenuRecentFiles();

    void initActionModel();
    QAbstractListModel* subMenuActionmodel();

    void setNumOfRecentFiles(int n);
    void updateRecentFiles(QSettings &settings);
    void setMostRecentFile(const QString fileName);


    static SubMenuRecentFiles * getInstance();
signals:

public slots:
    void slotClear();
private:
    ActionCommandModel* m_actionModelList = nullptr;

    static SubMenuRecentFiles* m_instance;
private slots:
    void slotLanguageChanged();
};

#endif // SUBMENURECENTFILES_H

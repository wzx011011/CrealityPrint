#ifndef CX3DSUBMENURECENTPROJECT_H
#define CX3DSUBMENURECENTPROJECT_H


#define recentFileCount "Creality3DrecentProject/numOfRecentFiles"
#define recentFileListId "Creality3DrecentProject/fileList"
#define Max_RecentFile_Size 10

#include <QObject>
#include "menu/actioncommand.h"
#include "menu/actioncommandmodel.h"


#include <QSettings>
class Cx3dSubmenuRecentProject : public ActionCommand
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* subMenuActionmodel READ subMenuActionmodel CONSTANT)
public:
    explicit Cx3dSubmenuRecentProject();
    virtual ~Cx3dSubmenuRecentProject();

    void initActionModel();
    QAbstractListModel* subMenuActionmodel();

    void setNumOfRecentFiles(int n);
    void updateRecentFiles(QSettings &settings);
    void setMostRecentFile(const QString fileName);


    static Cx3dSubmenuRecentProject * getInstance();
signals:

public slots:
    void slotClear();
private:
    ActionCommandModel* m_actionModelList = nullptr;

    static Cx3dSubmenuRecentProject* m_instance;
private slots:
    void slotLanguageChanged();
};

#endif // CX3DSUBMENURECENTPROJECT_H

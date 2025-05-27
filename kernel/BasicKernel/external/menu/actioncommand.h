#ifndef ACTIONCOMMAND_H
#define ACTIONCOMMAND_H

#include <QObject>
#include "basickernelexport.h"
enum eMenuType
{
    eMenuType_File =0,
    eMenuType_Edit,
    eMenuType_View,
    //eMenuType_Repair,
    eMenuType_Slice,
    eMenuType_Tool,
    eMenuType_CrealityGroup,
    eMenuType_PrinterControl,
    eMenuType_Help
};
class BASIC_KERNEL_API ActionCommand : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged)
    Q_PROPERTY(QString source READ source)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString shortcut READ shortcut)
  //  Q_PROPERTY(eMenuType eParentMenu READ parentMenu)
    Q_PROPERTY(QString icon READ icon)
    Q_PROPERTY(bool bSubMenu READ bSubMenu)
    Q_PROPERTY(bool bSeparator READ bSeparator)
    //Q_PROPERTY(bool enabled READ enabled)
    Q_PROPERTY(bool visible READ visible)
    Q_PROPERTY(bool checked READ bChecked)
    Q_PROPERTY(bool checkable READ bCheckable)
public:
    explicit ActionCommand(QObject *parent = nullptr);
    virtual ~ActionCommand();
//    int orderindex;
    virtual bool enabled();

    QString source() const;
    QString name() const;
    QString nameWithout() const;
    QString shortcut() const;
    eMenuType parentMenu() const;
    QString icon() const;
    bool bSubMenu() const;
    bool bSeparator() const;


    void setName(const QString& name);
    void setSource(const QString& source);
    void setShortcut(const QString &shortcut);
    void setParentMenu(const eMenuType &parentMenu);
    void setIcon(const QString &icon);


    void setBSubMenu(const bool &subMenu);
    void setBSeparator(const bool &bSeparator);

    void setEnabled(bool enabled);

    Q_INVOKABLE void execute();

    bool visible(){return  m_bVisible;}
    void setVisible(bool visible){m_bVisible = visible;}

    bool bChecked(){return m_bChecked;}
    void setChecked(bool check){m_bChecked = check;}


    bool bCheckable(){return m_bCheckable;}
    void setCheckable(bool check){m_bCheckable = check;}

    QString insertKey() const {return m_insertKey;}
    void setInsertKey(QString key){m_insertKey = key;}
//    virtual void onExecute();
signals:
    void enabledChanged();
  //  void executed();
protected:
    bool m_enabled;

protected:
    QString m_source;
    QString m_actionname;
    QString m_actionNameWithout;//记录没有翻译的名称
    QString m_shortcut;
    QString m_icon;
    eMenuType m_eParentMenu;
    bool m_bSeparator;
    bool m_bSubMenu;
    bool m_bVisible;
    bool m_bChecked;
    bool m_bCheckable;
    QString m_insertKey;    //use to find and insert
};


#endif // ACTIONCOMMAND_H

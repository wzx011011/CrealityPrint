#ifndef SETNOZZLERANGE_H
#define SETNOZZLERANGE_H

#include <QObject>
#include <QPoint>
#include "menu/actioncommand.h"
#include "menu/actioncommandmodel.h"
class SetNozzleRange:public ActionCommand
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* subMenuActionmodel READ subMenuActionmodel CONSTANT)
public:

    ~SetNozzleRange();
    void updateActionModel();
    QAbstractListModel* subMenuActionmodel();
    static SetNozzleRange * getInstance()
    {
        static SetNozzleRange inst;
        return &inst;
    }
    Q_INVOKABLE void setNozzleCount(int nCount);
    bool enabled() override;
private:
    SetNozzleRange();
    int m_nNozzleCount;
    ActionCommandModel* m_actionModelList = nullptr;
private slots:
    void slotLanguageChanged();

};

#endif // SETNOZZLERANGE_H

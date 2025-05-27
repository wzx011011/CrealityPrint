#ifndef _NULLSPACE_ADDCOMMAND_1589699742645_H
#define _NULLSPACE_ADDCOMMAND_1589699742645_H
#include <QtCore/QObject>
#include <QModelIndex>
#include "job/autolayoutjob.h"
class AddCommand: public QObject
{
    Q_OBJECT
public:
    AddCommand(QObject* parent = nullptr);
    virtual ~AddCommand();

    Q_INVOKABLE void add();

    Q_INVOKABLE void remove(QModelIndexList indexs);
    Q_INVOKABLE void remove();
    Q_INVOKABLE void selects(QModelIndexList indexs);
    Q_INVOKABLE void selects(QList<int> indexs);
    Q_INVOKABLE void cancelSelect(QList<int> indexs);
    Q_INVOKABLE void layoutAll();
    Q_INVOKABLE void writeReg(bool flag);
    Q_INVOKABLE bool isPopPage();
    Q_INVOKABLE void gotoSupportTab();
    Q_INVOKABLE void setModelVisible(int index, bool isVisible);

protected:
    void remove(QObject* obj);
private:
    AutoLayoutJob *m_layoutJob;
};
#endif // _NULLSPACE_ADDCOMMAND_1589699742645_H

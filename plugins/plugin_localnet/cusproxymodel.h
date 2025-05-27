#ifndef CUSPROXYMODEL_H
#define CUSPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

#include "cuslistmodel.h"

class CusProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    Q_PROPERTY(QAbstractItemModel* cSourceModel READ cSourceModel WRITE setCSourceModel NOTIFY cSourceModelChanged)
    explicit CusProxyModel(QObject *parent = nullptr);

    //filter
    Q_INVOKABLE void multiConditionFilter(int state, int group);

    //q_property
    QAbstractItemModel *cSourceModel() const;
    void setCSourceModel(QAbstractItemModel *newCSourceModel);

    //override
    void setSourceModel(QAbstractItemModel *sourceModel) override;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const  override;

private:
    int m_state;
    int m_group;
    QAbstractItemModel *m_cSourceModel;

signals:
    void cSourceModelChanged();
};

#endif // CUSPROXYMODEL_H

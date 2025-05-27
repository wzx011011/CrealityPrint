#include "cusproxymodel.h"
#include "cuslistmodel.h"
#include <QDebug>

CusProxyModel::CusProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    m_state = 0;
    m_group = 1;
}

void CusProxyModel::multiConditionFilter(int state, int group)
{
    m_state = state;
    m_group = group;
    invalidateFilter();
}

QAbstractItemModel *CusProxyModel::cSourceModel() const
{
    return m_cSourceModel;
}

void CusProxyModel::setCSourceModel(QAbstractItemModel *newCSourceModel)
{
    if (m_cSourceModel == newCSourceModel)
        return;
    m_cSourceModel = newCSourceModel;
    emit cSourceModelChanged();
}

void CusProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    QSortFilterProxyModel::setSourceModel(sourceModel);
    //connect(sourceModel, &QAbstractItemModel::layoutChanged, this, [=](){invalidateFilter();});
    m_cSourceModel = sourceModel;
}

bool CusProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex stateIndex = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex groupIndex= sourceModel()->index(sourceRow, 0, sourceParent);

    int state = sourceModel()->data(stateIndex, ListModelItem::E_PrinterState).toInt();
    int group = sourceModel()->data(groupIndex, ListModelItem::E_GroupNo).toInt();
	bool stateFliter = m_state ? (state == 1 || state == 5) : (state != 1 && state != 5);//1:��ӡ��(������ͣ) 0:����(������ӡ���/ʧ��/ֹͣ)

    return (stateFliter && group == m_group);
}

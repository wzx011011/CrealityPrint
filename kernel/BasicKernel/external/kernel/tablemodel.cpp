#include "tablemodel.h"
#include <QDebug>
#include "tableinfo.h"
TableModel::TableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    //for(int i = 0; i < 3; ++i)
    //{
    //    TableInfo *tmpInfo = new TableInfo();
    //    tmpInfo->checkstate =false;
    //    tmpInfo->id = i;
    //    m_data.push_back(tmpInfo);
    //}
}


int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}
QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()/* || index.row() >= m_data.size()*/)
        return QVariant();
    if(role == OneRole_Check) {
        return m_data.at(index.row())->checkstate;
    }
    if(role == TwoRole_Name)
    {
        return m_data.at(index.row())->m_name;
    }
    if(role == ThreeRole_Ip)
    {
        return m_data.at(index.row())->m_ipAddr;
    }
    if(role == FourRole_Mac)
    {
        return m_data.at(index.row())->m_macAddr;
    }
    if(role == FiveRole_Status)
    {
        return m_data.at(index.row())->m_status;
    }
    return QVariant();
}
QHash<int,QByteArray> TableModel::roleNames() const
{
    QHash<int, QByteArray> roles;
     roles[OneRole_Check] = "select";
     roles[TwoRole_Name] = "name";
     roles[ThreeRole_Ip] = "ipaddr";
     roles[FourRole_Mac] = "macaddr";
     roles[FiveRole_Status] = "status";
     return roles;
}
int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.size();
}
void TableModel::selectAll(bool checked)
{
    beginResetModel();
    for(int i = 0; i < m_data.size(); ++i)
    {
        m_data.at(i)->checkstate = checked;
    }
    endResetModel();
}
void TableModel::setCheckStateByRow(int row, bool checked)
{
    if(row >= 0 && row < m_data.size())
    {
        beginResetModel();
        m_data.at(row)->checkstate = !m_data.at(row)->checkstate ;
        endResetModel();
    }

}

bool TableModel::getCheckStateByRow(int row)
{
//    beginResetModel();
    if (row == -1){return false;}
    bool bRet = m_data.at(row)->checkstate;
//    endResetModel();
    return bRet;
}

void TableModel::add()
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    TableInfo *tmpInfo = new TableInfo();
    m_data.push_back(tmpInfo);
    endInsertRows();
}
void TableModel::addTableData(QString name,QString ipAddr,QString macAddr)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    TableInfo* tmpInfo = new TableInfo();
    tmpInfo->m_name = name;
    tmpInfo->m_ipAddr = ipAddr;
    tmpInfo->m_macAddr = macAddr;
    m_data.push_back(tmpInfo);
    endInsertRows();
    
    emit signalAddPrintnetData(name, ipAddr, macAddr);

}

void TableModel::setNameByRow(int row, QString text)
{
    if (row == -1){return;}
    beginResetModel();
    m_data[row]->m_name = text;
    endResetModel();
}

void TableModel::delLastRow()
{
    if (m_data.size() <= 0) return;
    beginRemoveRows(QModelIndex(), m_data.size() - 1, m_data.size() - 1);
    m_data.removeLast();
    endRemoveRows();
}
void TableModel::clearTableModel()
{
    if (m_data.size() <= 0) return;
    int nCount = m_data.size();
    m_data.clear();
    beginResetModel();
    endResetModel();
}
void TableModel::delAllSelectRow()
{
    if (m_data.size() <= 0) return;
    beginRemoveRows(QModelIndex(), m_data.size() - 1, m_data.size() - 1);
    int nCount = m_data.size();
    for(int row =  nCount - 1;row >= 0;row --)
    {
        if(m_data.at(row)->checkStatus())
            m_data.removeAt(row);
    }
    endRemoveRows();
    beginResetModel();
    endResetModel();

}
void TableModel::sendSelectModelData()
{
    //int nCount = m_data.size();
    //QList<QString> rowsMac;
    //QMap<int, QString> map_rowAndMac;
    //for (int row = nCount - 1; row >= 0; row--)
    //{
    //    if (m_data.at(row)->checkStatus())
    //    {
    //        //rowsMac.push_back(m_data.at(row)->m_macAddr);
    //        map_rowAndMac.insert(row, m_data.at(row)->m_macAddr);
    //    }
    //}
    //if (map_rowAndMac.size() == 0)return;
    //emit sendSelectMacAndRow(map_rowAndMac);
}
QMap<int, QString> TableModel::getSelectPrinterMap()
{
    int nCount = m_data.size();
    QMap<int, QString> rowsRowAndMac;
    for (int row = nCount - 1; row >= 0; row--)
    {
        if (m_data.at(row)->checkStatus() && m_data.at(row)->m_status!=-1)
        {
            //rowsMac.push_back(m_data.at(row)->m_macAddr);
            rowsRowAndMac.insert(row, m_data.at(row)->m_macAddr);
        }
    }
    //if (rowsMac.size() == 0)return rowsMac;
    return rowsRowAndMac;
}
QList<QString> TableModel::getSelectPrinterList()
{
    int nCount = m_data.size();
    QList<QString> rowsMac;
    for (int row = nCount - 1; row >= 0; row--)
    {
        if (m_data.at(row)->checkStatus())
        {
            rowsMac.push_back(m_data.at(row)->m_macAddr);
        }
    }
    //if (rowsMac.size() == 0)return rowsMac;
    return rowsMac;
}

int TableModel::getSelectRowCount()
{
    int nCount = m_data.size();
    int nRows = 0;
    for (int row = nCount - 1; row >= 0; row--)
    {
        if (m_data.at(row)->checkStatus())
        {
            nRows++;
        }
    }
    return nRows;
}

void TableModel::addWifiPrinter(creative_kernel::PrinterData pd)
{
    TableInfo* tmpInfo = new TableInfo();
    tmpInfo->m_macAddr = pd.m_mac;
    tmpInfo->m_name = pd.m_printerName;
    tmpInfo->m_status = pd.m_status;

    bool conversionOK = false;
    QHostAddress ip4Address(pd.m_hostAddr.toIPv4Address(&conversionOK));
    QString ip4String;
    if (conversionOK)
    {
        ip4String = ip4Address.toString();
    }
    tmpInfo->m_ipAddr = ip4String;
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data.push_back(tmpInfo);
    endInsertRows();
}
void TableModel::refresh()
{
    beginResetModel();
    endResetModel();
}
void TableModel::slotProgressValue(int value,int row)
{
    TableInfo* tmpInfo = m_data.at(row);
    if (tmpInfo)
    {
        beginResetModel();
        tmpInfo->m_status = value;
        endResetModel();
    }
    
}
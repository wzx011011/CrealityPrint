#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include "basickernelexport.h"
#include "data/printerdata.h"
#include <QList>
class TableInfo;
enum TableRole {
    OneRole_Check,
    TwoRole_Name,
    ThreeRole_Ip,
    FourRole_Mac,
    FiveRole_Status
};
namespace creative_kernel
{
    struct PrinterData;
}
class BASIC_KERNEL_API TableModel:public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel(QObject *parent = Q_NULLPTR);
public:
    Q_INVOKABLE void selectAll(bool checked);
    Q_INVOKABLE void setCheckStateByRow(int row, bool checked);

    Q_INVOKABLE bool getCheckStateByRow(int row);

    //
    Q_INVOKABLE void add();
    Q_INVOKABLE bool isTranslateZip() { return m_isTranslateZip; };
    Q_INVOKABLE void setTranslateZip(bool iszip) { m_isTranslateZip = iszip; };

    Q_INVOKABLE void addTableData(QString name, QString ipAddr, QString macAddr);
    Q_INVOKABLE void setNameByRow(int row, QString text);
    Q_INVOKABLE void delLastRow();
    void clearTableModel();
    Q_INVOKABLE void refresh();

    Q_INVOKABLE void delAllSelectRow();

    Q_INVOKABLE void sendSelectModelData();
    Q_INVOKABLE int getSelectRowCount();
    QList<QString> getSelectPrinterList();
    
    void addWifiPrinter(creative_kernel::PrinterData pd);
    QMap<int, QString> getSelectPrinterMap();
signals:
    void sendSelectMacData(QList<QString> macData);
    void sendSelectMacAndRow(QMap<int, QString> mapRowAndMac);
    void signalAddPrintnetData(QString name, QString ip, QString mac);
    void signalDeletePrinterByMac(QList<QString> listMac);
public slots:
    void slotProgressValue(int value,int row);
private:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int,QByteArray> roleNames() const;
private:
    QVector<TableInfo *> m_data;
    bool m_isTranslateZip = false;
};

#endif // TABLEMODEL_H

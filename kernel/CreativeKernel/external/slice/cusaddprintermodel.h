#ifndef CUSADDPRINTERMODEL_H
#define CUSADDPRINTERMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QTimer>
#include <QJsonObject>
//#include "basickernelexport.h"

class AddPrinterInfo
{
public:
    AddPrinterInfo();
    ~AddPrinterInfo();

    QString printerImageUrl() const { return m_printerImageUrl; }
    void setPrinterImageUrl(QString url) { m_printerImageUrl = url; }

    QString printerName() const { return m_printerName; }
    void setPrinterName(QString name) { m_printerName = name; }

    int machineDepth() const { return m_MachineDepth; }
    void setMachineDepth(int depth) { m_MachineDepth = depth; }

    int machineWidth() const { return m_MachineWidth; }
    void setMachineWidth(int width) { m_MachineWidth = width; }

    int machineHeight() const { return m_MachineHeight; }
    void setMachineHeight(int height) { m_MachineHeight = height; }

    const float nozzleSize() const { return m_NozzleSize; }
    void setNozzleSize(float size) { m_NozzleSize = size; }

private:
    QString m_printerImageUrl = "";
    QString m_printerName = "";
    int m_MachineDepth = 0;
    int m_MachineWidth = 0;
    int m_MachineHeight = 0;
    float m_NozzleSize = 0.0;
};

class QQmlApplicationEngine;

class /*BASIC_KERNEL_API*/ CusAddPrinterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum WifiInfoRoles {
        PRINTERIMAGEURL_ROLE = Qt::UserRole + 1,
        PRINTERNAME_ROLE,
        MACHINEDEPTH_ROLE,
        MACHINEWIDTH_ROLE,
        MACHINEHEIGHT_ROLE,
        NOZZLESIZE_ROLE
    };

    enum CurDataType {
        ENDER_DATA = 1,
        CR_DATA,
        SERMOON_DATA,
        OTHERS_DATA,
        FAST_DATA
    };

    CusAddPrinterModel();

    Q_INVOKABLE void changePrinterDataSource(int dataType);

    void addPrinterInfoListFromJson();


protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const;

private:
    void addModelData(AddPrinterInfo info);
    void clearPrinterInfoList() { m_AddPrinterInfoList.clear(); }
    void updatePrinterInfoList(QList<AddPrinterInfo>& list);

private:
    QList<AddPrinterInfo> m_AddPrinterInfoList;
    QList<AddPrinterInfo> m_EnderInfoList;
    QList<AddPrinterInfo> m_CrInfoList;
    QList<AddPrinterInfo> m_SermoonInfoList;
    QList<AddPrinterInfo> m_PrintScreenList;
    QList<AddPrinterInfo> m_OthersInfoList;
    int m_preDataSource = 0;
};

#endif // CUSADDPRINTERMODEL_H

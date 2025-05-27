#ifndef TABLEINFO_H
#define TABLEINFO_H

#include <QObject>

class TableInfo :public QObject
{
    Q_OBJECT
public:
    TableInfo(QObject *parent = Q_NULLPTR);
    Q_INVOKABLE bool checkStatus();
public:
    bool checkstate;//是否选择
    int id;
    QString m_name;
    int m_status;
    QString m_ipAddr;
    QString m_macAddr;
};

#endif // TABLEINFO_H

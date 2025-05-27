// Copyright (C) 2021 Creality3D company. All rights reserved.
// Use of this source code is governed by a GNU General Public License(GPL) 
// and a GNU Lesser General Public License (LGPL).

// Created: 2022.05.16 @ xx


#ifndef _DEVICE_INFO_TASK_OLD_IOT_1588905534361_H
#define _DEVICE_INFO_TASK_OLD_IOT_1588905534361_H

#include <QObject>
#include <QRunnable>
#include <QString>
#include <functional>
#include <QByteArray>

class QueryEquipmentInfoTask;
class DeviceInfoTaskOldIot : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit DeviceInfoTaskOldIot(QueryEquipmentInfoTask* parent);
    ~DeviceInfoTaskOldIot();
    void run();

    void setProductKey(QString pn);
    void setDeviceName(QString dn);
    void setIotId(QString iotId);

private: 
    void getAliyunInfo();
    void getAliyunInfoStatus();
    QString hmacSha1(QByteArray key, QByteArray baseString);
    QString currentTimeToUTC();
    int randTime();
    QString percentEncodeUrl(QString value);
    QString percentEncodeToken(QString value);
    QString generateQueryString(QMap<QString, QString> parameter, bool isEncodeKV);
    QString generateSignString(QString method, QMap<QString, QString> parameter);
    QString generate(QString method, QMap<QString, QString> parameter, QString accessKeySecret);

    void funcGetAliyunInfo(const QByteArray& response);
    void funcGetAliyunInfoStatus(const QByteArray& response);
    void funcQueryDevicePropertyStatus(const QByteArray& response);
    void funcGetDeviceStatus(const QByteArray& response);

private:
    QString m_iotId = "";
    QString m_deviceName = "";
    QString m_productKey = "";
    QueryEquipmentInfoTask* m_op = nullptr;
    int m_reply = 0;
};


#endif  // ! _DEVICE_INFO_TASK_OLD_IOT_1588905534361_H

// Copyright (C) 2021 Creality3D company. All rights reserved.
// Use of this source code is governed by a GNU General Public License(GPL) 
// and a GNU Lesser General Public License (LGPL).

// Created: 2021.10.19 @ xx


#ifndef _QTUSER_CORE_QUERY_EQUIPMENT_INFO_TASK_1588905534361_H
#define _QTUSER_CORE_QUERY_EQUIPMENT_INFO_TASK_1588905534361_H

#include <QObject>
#include <QRunnable>
#include <QString>
#include <functional>
#include <QByteArray>
#include "crealitycloudui.h"

class QueryEquipmentInfoTask : public QThread
{
    Q_OBJECT
public:
    explicit QueryEquipmentInfoTask(CrealityCloudUI* parent);
    ~QueryEquipmentInfoTask();
    void run();

    void setEquipmentInfo(const std::string& infomation);
    void setStop(bool isStop);

    void setDeviceStatus(const std::string& status, const std::string& id);
    void setDeviceInformation(const std::string& date, const std::string& id, bool isNewIOT);
    void setDeviceInformationRealTime(const std::string& date, const std::string& id);
    void setDeviceInformationPosition(const std::string& date, const std::string& id);

private:
    std::mutex m_mtxList;
    std::list<std::tuple<int,std::string, std::string, std::string, std::string> > m_info;
    bool is_stop = false;
    CrealityCloudUI* m_op = nullptr;
};


#endif  // ! _QTUSER_CORE_QUERY_EQUIPMENT_INFO_TASK_1588905534361_H

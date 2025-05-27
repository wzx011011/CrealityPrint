// Copyright (C) 2021 Creality3D company. All rights reserved.
// Use of this source code is governed by a GNU General Public License(GPL) 
// and a GNU Lesser General Public License (LGPL).

// Created: 2021.07.08 @ xx


#ifndef _QTUSER_CORE_DOWN_IMAGE_TASK_1588905534361_H
#define _QTUSER_CORE_DOWN_IMAGE_TASK_1588905534361_H

#include <QObject>
#include <QRunnable>
#include <QString>
#include <functional>

class DownImageTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit DownImageTask(const QString imageUrl, const QString fileName,  const QString filePath, const std::function<void(QString)>& call_back, bool isImage);
    ~DownImageTask();
    void run();

private:
    QString m_imageUrl;
    QString m_fileName;
    QString m_filePath;
    std::function<void(QString)> call_back_;
    bool m_IsImage;

signals:
    void signal_loadTaskFinished();
};


#endif  // ! _QTUSER_CORE_DOWN_IMAGE_TASK_1588905534361_H

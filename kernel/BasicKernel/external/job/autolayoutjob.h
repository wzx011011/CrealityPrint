#ifndef AUTOLAYOUTJOB_H
#define AUTOLAYOUTJOB_H
#include "basickernelexport.h"
#include <QObject>
#include "qtusercore/module/job.h"
class BASIC_KERNEL_API AutoLayoutJob : public qtuser_core::Job
{
    Q_OBJECT
public:
    explicit AutoLayoutJob(QObject *parent = nullptr);
protected:
    QString name();
    QString description();
    void failed();                        // invoke from main thread
    void successed(qtuser_core::Progressor* progressor);                     // invoke from main thread
    void work(qtuser_core::Progressor* progressor);    // invoke from worker thread
protected:
signals:

public slots:
};

#endif // AUTOLAYOUTJOB_H

#ifndef OPENEVENTFILTER_H
#define OPENEVENTFILTER_H
#include <QFileOpenEvent>
#include <QMessageBox>
#include "cxkernel.h"
class OpenEventFilter : public QObject
{
public:
    bool eventFilter(QObject* obj, QEvent* event)
    {
    	bool result=false;
        if (event->type() == QEvent::FileOpen)
        {            
            QFileOpenEvent* fileEvent = static_cast<QFileOpenEvent*>(event);
            if (!fileEvent->url().isEmpty())
            {                                
                QString url = fileEvent->url().toString();
                cxkernel::cxKernel->processCrealink(url);
                //QMessageBox::information(nullptr, "完整url scheme", url,"确定");            
            }
            result=true;
        }
        else
        {
            result = QObject::eventFilter(obj, event);
        }
        return result;
    }
};
#endif
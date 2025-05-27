#ifndef RCLICKMENUINTERFACE_H
#define RCLICKMENUINTERFACE_H

#include <QObject>
#include <QPoint>
//#include "../external/creativekernelexport.h"
class ActionCommand;
class /*CREATIVE_KERNEL_API*/ RClickMenuInterface : public QObject
{
    Q_OBJECT
public:
    explicit RClickMenuInterface(QObject *parent = nullptr);
    static RClickMenuInterface *getInstance();

     void doRightClick(bool bEnable, QPoint pos, bool hasModel);//by TCJ 增加一个参数
signals:
     void sigRightClick(bool bEnable, QPoint point, bool hasModel);
private:
     static RClickMenuInterface * m_pComInstance;
};

#endif // RCLICKMENUINTERFACE_H

#include "rclickmenuinterface.h"
#include <QDebug>
#include <QPoint>

RClickMenuInterface *RClickMenuInterface:: m_pComInstance = nullptr;
RClickMenuInterface *RClickMenuInterface:: getInstance()
{
    if (m_pComInstance == nullptr)
    {
        m_pComInstance = new RClickMenuInterface();
    }
    return m_pComInstance;
}

RClickMenuInterface::RClickMenuInterface(QObject *parent) : QObject(parent)
{
//    doRightClick(true);
}

void RClickMenuInterface::doRightClick(bool bEnable, QPoint pos, bool hasModel)
{
    qDebug()<<"emit sigRightClick success!!" ;
    emit sigRightClick(bEnable, pos, hasModel);

}

#include "kernel/kernelevent.h"
#include <QtCore/QDebug>
#include <qdebug.h>
#include "kernelui.h"
#include "us/profilemanager.h"
#include "us/slicermanager.h"
#include "internal/rclick_menu/setnozzlerange.h"
#include "internal/rclick_menu/rclickmenuinterface.h"
#include "data/modeln.h"
#include "kernel/kernel.h"
#include "kernel/modelnselector.h"
#include <QList>
#include "interface/spaceinterface.h"
SINGLETON_IMPL(KernelEvent)

KernelEvent::KernelEvent()
    : m_savePoint(QPoint(-1, -1))
{

}

KernelEvent::~KernelEvent()
{
}


void KernelEvent::onRightMouseButtonPress(QMouseEvent* event)
{
    qDebug() << "KernelEvent::onRightMouseButtonPress";
    (void)(event);
}

void KernelEvent::onRightMouseButtonMove(QMouseEvent* event)
{
    (void)(event);
}

void KernelEvent::onRightMouseButtonRelease(QMouseEvent* event)
{
    (void)(event);
}

void KernelEvent::onRightMouseButtonClick(QMouseEvent* event)
{
    qDebug() << "KernelEvent::onRightMouseButtonClick";
    (void)(event);
    using namespace creative_kernel;
    QList<ModelN*> selections = getKernel()->modelSelector()->selectionms();
    //use to actionCmd enable menu function
    bool  hasModel = true;
    if(selections.size() == 0)
    {
        haveModels() ? (hasModel = true) : (hasModel = false);
        RClickMenuInterface::getInstance()->doRightClick(false, event->pos(), hasModel);
    }
    else
    {
        RClickMenuInterface::getInstance()->doRightClick(true, event->pos(), hasModel);
    }

    QObject *obj = getKernelUI()->glMainView();


    QString machine = SlicerManager::instance().getCurrentMachine();
    int nNozzleCount = 0;
    auto t = SlicerManager::instance().globalsettings_modify()->settings().value("machine_extruder_count");
    if (t != nullptr)
    {
        nNozzleCount = t->value().toInt();
    }
    
    //    qDebug()<<"nNozzleCount =" << nNozzleCount;

    SetNozzleRange::getInstance()->setNozzleCount(nNozzleCount);
    if(ProfileManager::instance().getMachineType(machine) == "DLP")
    {
        SetNozzleRange::getInstance()->setVisible(false);
    }
    else if(ProfileManager::instance().getMachineType(machine) == "FDM")
    {

        SetNozzleRange::getInstance()->setVisible(true);
    }

    //wzx:control mouseRightBtnEvent
//    if (getKernelUI()->isSupportBtnChecked())
    {
        //暂时屏蔽鼠标右键
        QMetaObject::invokeMethod(obj, "requstShowRightMenu");
    }
}


#ifndef _creative_kernel_KERNELEVENT_H
#define _creative_kernel_KERNELEVENT_H

#include "creativekernelexport.h"
#include "qtusercore/module/singleton.h"
#include "qtuser3d/event/eventhandlers.h"

class CREATIVE_KERNEL_API KernelEvent: public QObject
	, public qtuser_3d::RightMouseEventHandler
{
	Q_OBJECT
	
	SINGLETON_DECLARE(KernelEvent)
public:
	virtual ~KernelEvent();

protected:
	void onRightMouseButtonPress(QMouseEvent* event) override;
	void onRightMouseButtonRelease(QMouseEvent* event) override;
	void onRightMouseButtonMove(QMouseEvent* event) override;
	void onRightMouseButtonClick(QMouseEvent* event) override;

protected:
	QPoint m_savePoint;

};

SINGLETON_EXPORT(CREATIVE_KERNEL_API, KernelEvent)
#endif // _creative_kernel_KERNELUI_1589818161757_H

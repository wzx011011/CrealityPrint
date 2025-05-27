#ifndef CREATIVE_KERNEL_VISSCENELEFTMOUSEHANDLER_1594483094948_H
#define CREATIVE_KERNEL_VISSCENELEFTMOUSEHANDLER_1594483094948_H
#include "basickernelexport.h"
#include "qtuser3d/event/eventhandlers.h"

namespace creative_kernel
{
	class BASIC_KERNEL_API VisSceneLeftMouseHandler : public QObject
		, public qtuser_3d::LeftMouseEventHandler
	{
		Q_OBJECT
	public:
		VisSceneLeftMouseHandler(QObject* parent = nullptr);
		virtual ~VisSceneLeftMouseHandler();

	protected:
		void onLeftMouseButtonPress(QMouseEvent* event) override;
		void onLeftMouseButtonRelease(QMouseEvent* event) override;
		void onLeftMouseButtonMove(QMouseEvent* event) override;
		void onLeftMouseButtonClick(QMouseEvent* event) override;
	};
}
#endif // CREATIVE_KERNEL_VISSCENELEFTMOUSEHANDLER_1594483094948_H
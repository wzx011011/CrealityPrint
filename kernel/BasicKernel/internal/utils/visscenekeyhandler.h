#ifndef CREATIVE_KERNEL_VISSCENEKEYHANDLER_1594460995569_H
#define CREATIVE_KERNEL_VISSCENEKEYHANDLER_1594460995569_H
#include "basickernelexport.h"
#include "qtuser3d/event/eventhandlers.h"

namespace creative_kernel
{
	class BASIC_KERNEL_API VisSceneKeyHandler : public QObject,
		public qtuser_3d::KeyEventHandler
	{
		Q_OBJECT
	public:
		VisSceneKeyHandler(QObject* parent = nullptr);
		virtual ~VisSceneKeyHandler();

	protected:
		void onKeyPress(QKeyEvent* event) override;
		void onKeyRelease(QKeyEvent* event) override;
	};
}
#endif // CREATIVE_KERNEL_VISSCENEKEYHANDLER_1594460995569_H
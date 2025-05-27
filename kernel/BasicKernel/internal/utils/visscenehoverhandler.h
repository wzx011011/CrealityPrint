#ifndef CREATIVE_KERNEL_VISSCENEHOVERHANDLER_1594483094926_H
#define CREATIVE_KERNEL_VISSCENEHOVERHANDLER_1594483094926_H
#include "basickernelexport.h"
#include "qtuser3d/event/eventhandlers.h"

namespace creative_kernel
{
	class BASIC_KERNEL_API VisSceneHoverHandler : public QObject
		,public qtuser_3d::HoverEventHandler
	{
		Q_OBJECT
	public:
		VisSceneHoverHandler(QObject* parent = nullptr);
		virtual ~VisSceneHoverHandler();

	protected:
		void onHoverEnter(QHoverEvent* event) override;
		void onHoverLeave(QHoverEvent* event) override;
		void onHoverMove(QHoverEvent* event) override;
	};
}
#endif // CREATIVE_KERNEL_VISSCENEHOVERHANDLER_1594483094926_H
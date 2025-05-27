#include "visscenehoverhandler.h"

#include "interface/selectorinterface.h"
#include "interface/visualsceneinterface.h"

namespace creative_kernel
{
	VisSceneHoverHandler::VisSceneHoverHandler(QObject* parent)
		:QObject(parent)
	{
	}
	
	VisSceneHoverHandler::~VisSceneHoverHandler()
	{
	}

	void VisSceneHoverHandler::onHoverEnter(QHoverEvent* event)
	{

	}

	void VisSceneHoverHandler::onHoverLeave(QHoverEvent* event)
	{

	}

	void VisSceneHoverHandler::onHoverMove(QHoverEvent* event)
	{
		if (hoverVis(event->pos()))
			requestVisUpdate(false);
	}
}
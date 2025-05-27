#include "vissceneleftmousehandler.h"

#include "interface/selectorinterface.h"
#include "interface/visualsceneinterface.h"

namespace creative_kernel
{
	VisSceneLeftMouseHandler::VisSceneLeftMouseHandler(QObject* parent)
		:QObject(parent)
	{
	}
	
	VisSceneLeftMouseHandler::~VisSceneLeftMouseHandler()
	{
	}

	void VisSceneLeftMouseHandler::onLeftMouseButtonPress(QMouseEvent* event)
	{

	}

	void VisSceneLeftMouseHandler::onLeftMouseButtonRelease(QMouseEvent* event)
	{

	}

	void VisSceneLeftMouseHandler::onLeftMouseButtonMove(QMouseEvent* event)
	{

	}

	void VisSceneLeftMouseHandler::onLeftMouseButtonClick(QMouseEvent* event)
	{
		selectVis(event->pos());
		requestVisUpdate();
	}
}
#include "pickbottom.h"
#include "pickbottomcommand.h"

#include "kernel/abstractkernel.h"

using namespace creative_kernel;
PickBottom::PickBottom(QObject* parent)
	:QObject(parent)
	, m_command(nullptr)
{
}

PickBottom::~PickBottom()
{
}

QString PickBottom::name()
{
	return "";
}

QString PickBottom::info()
{
	return "";
}

void PickBottom::initialize()
{
	if (!m_command)
		m_command = new PickBottomCommand(this);
    AbstractKernelUI::getSelf()->addToolCommand(m_command, "left", 6);
	//addLCommand(m_command);
}

void PickBottom::uninitialize()
{
	AbstractKernelUI::getSelf()->removeToolCommand(m_command, "left");
	//removeLCommand(m_command);
}

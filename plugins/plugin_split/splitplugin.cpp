#include "splitplugin.h"
#include "splitcommand.h"

#include "kernel/abstractkernel.h"
using namespace creative_kernel;

SplitPlugin::SplitPlugin(QObject* parent)
	:QObject(parent)
	, m_command(nullptr)
{
}

SplitPlugin::~SplitPlugin()
{
}

QString SplitPlugin::name()
{
	return "";
}

QString SplitPlugin::info()
{
	return "";
}

void SplitPlugin::initialize()
{
	if (!m_command)
		m_command = new SplitCommand(this);

    AbstractKernelUI::getSelf()->addToolCommand(m_command, "left", 7);
}

void SplitPlugin::uninitialize()
{
	AbstractKernelUI::getSelf()->removeToolCommand(m_command, "left");
}

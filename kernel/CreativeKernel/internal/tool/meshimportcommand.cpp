#include "meshimportcommand.h"

#include "interface/loadinterface.h"
MeshImportCommand::MeshImportCommand(QObject* parent)
	:ToolCommand(parent)
{
    orderindex = 1;
    m_name = tr("Open");
	m_enabledIcon = "qrc:/kernel/images/open.png";
	m_pressedIcon = "qrc:/kernel/images/open.png";
}

MeshImportCommand::~MeshImportCommand()
{
}

void MeshImportCommand::execute()
{
	using namespace creative_kernel;
	requestLoad();
}


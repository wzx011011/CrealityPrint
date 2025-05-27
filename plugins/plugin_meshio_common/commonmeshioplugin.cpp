#include "commonmeshioplugin.h"
#include "data/meshiocenter.h"

CommonMeshIOPlugin::CommonMeshIOPlugin(QObject* parent)
	:QObject(parent)
	, m_meshIOImpl(nullptr)
{
	m_meshIOImpl = new MeshIOImpl(this);
}

CommonMeshIOPlugin::~CommonMeshIOPlugin()
{
}

QString CommonMeshIOPlugin::name()
{
	return "CommonMeshIO";
}

QString CommonMeshIOPlugin::info()
{
	return "";
}

void CommonMeshIOPlugin::initialize()
{
	MESHIO.addMeshIOInterface(m_meshIOImpl);
}

void CommonMeshIOPlugin::uninitialize()
{
	MESHIO.removeMeshIOInterface(m_meshIOImpl);
}

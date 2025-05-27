#include "extendmeshplugin.h"

#include "data/meshiocenter.h"
#include "inout/cxopenandsavefilemanager.h"
#include "interface/modelinterface.h"

using namespace creative_kernel;
PluginExtendMesh::PluginExtendMesh(QObject* parent)
	:QObject(parent)
{

	QStringList typelist;

	typelist << "bmp";
	typelist << "jpg";
	typelist << "jpeg";
	typelist << "png";

	typelist << "gtl";
	typelist << "gbl";
	typelist << "gbo";
	typelist << "gbp";
	typelist << "gbs";
	typelist << "gko";
	typelist << "gml";
	typelist << "gpb";
	typelist << "gpt";
	typelist << "gto";
	typelist << "gtp";
	typelist << "gts";

	m_extenLoaderWrapper = new ExtendLoaderWrapper(this);

	CXFILE.registerOpenHandler(typelist, m_extenLoaderWrapper);
}

PluginExtendMesh::~PluginExtendMesh()
{
}

QString PluginExtendMesh::name()
{
	return "extend mesh io";
}

QString PluginExtendMesh::info()
{
	return "extend mesh gen";
}

void PluginExtendMesh::initialize()
{
	//MESHIO.addMeshIOInterface(m_photoImpl);
}

void PluginExtendMesh::uninitialize()
{
	//MESHIO.removeMeshIOInterface(m_photoImpl);
}

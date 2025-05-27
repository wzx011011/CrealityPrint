#ifndef _NULLSPACE_PluginExtendMesh_1589849922902_H
#define _NULLSPACE_PluginExtendMesh_1589849922902_H
#include "creativeinterface.h"
#include "extendloaderwrapper.h"

class PluginExtendMesh: public QObject, public CreativeInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "creative.ExtendMeshGen")
	Q_INTERFACES(CreativeInterface)
public:
	PluginExtendMesh(QObject* parent = nullptr);
	virtual ~PluginExtendMesh();

protected:
	QString name() override;
	QString info() override;

	void initialize() override;
	void uninitialize() override;

protected:

	ExtendLoaderWrapper* m_extenLoaderWrapper;
};
#endif // _NULLSPACE_PluginExtendMesh_1589849922902_H

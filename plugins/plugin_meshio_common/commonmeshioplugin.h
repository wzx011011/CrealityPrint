#ifndef _NULLSPACE_COMMONMESHIOPLUGIN_1589960315948_H
#define _NULLSPACE_COMMONMESHIOPLUGIN_1589960315948_H
#include "creativeinterface.h"
#include "meshioimpl.h"

class CommonMeshIOPlugin: public QObject, public CreativeInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "creative.CommonMeshIOPlugin")
	Q_INTERFACES(CreativeInterface)
public:
	CommonMeshIOPlugin(QObject* parent = nullptr);
	virtual ~CommonMeshIOPlugin();

	QString name() override;
	QString info() override;

	void initialize() override;
	void uninitialize() override;

protected:
	MeshIOImpl* m_meshIOImpl;
};
#endif // _NULLSPACE_COMMONMESHIOPLUGIN_1589960315948_H

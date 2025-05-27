#ifndef _NULLSPACE_MESHIOIMPL_1589960655092_H
#define _NULLSPACE_MESHIOIMPL_1589960655092_H
#include <QtCore/QObject>
#include "meshiointerface.h"

class MeshIOImpl: public QObject, public MeshIOInterface
{
public:
	MeshIOImpl(QObject* parent = nullptr);
	virtual ~MeshIOImpl();

protected:
	QString description() override;

	QStringList saveSupportExtension() override;
	QStringList loadSupportExtension() override;

	trimesh::TriMesh* load(const QString& name, qtuser_core::Progressor* progressor) override;
	void save(trimesh::TriMesh* mesh, const QString& name, qtuser_core::Progressor* progressor) override;

protected:
	QStringList m_saveSupportExtensions;
	QStringList m_loadSupportExtensions;
};
#endif // _NULLSPACE_MESHIOIMPL_1589960655092_H

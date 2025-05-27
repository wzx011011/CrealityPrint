#ifndef _NULLSPACE_MESHIOCENTER_1589533079831_H
#define _NULLSPACE_MESHIOCENTER_1589533079831_H
#include "basickernelexport.h"
#include <QtCore/QObject>
#include <QtCore/QMap>
#include "meshiointerface.h"
#include "qtusercore/module/progressor.h"

namespace creative_kernel
{
	class BASIC_KERNEL_API MeshIOCenter : public QObject
	{
		Q_OBJECT
			Q_PROPERTY(QStringList saveNameFilters READ saveNameFilters NOTIFY saveNameFiltersChanged)
			Q_PROPERTY(QStringList loadNameFilters READ loadNameFilters NOTIFY loadNameFiltersChanged)

			MeshIOCenter(QObject* parent = nullptr);

		static MeshIOCenter s_meshIOCenter;
	public:
		static MeshIOCenter& instance();
	public:
		virtual ~MeshIOCenter();

		trimesh::TriMesh* load(const QString& name);
		void save(trimesh::TriMesh* mesh, const QString& name);

		trimesh::TriMesh* asyncLoad(const QString& name, qtuser_core::Progressor* progressor);

		QStringList loadExtensionList();
		QStringList saveExtensionList();

		void addMeshIOInterface(MeshIOInterface* meshIOInterface);
		void removeMeshIOInterface(MeshIOInterface* meshIOInterface);
		void clearAll();

		QStringList saveNameFilters();
		QStringList loadNameFilters();
	signals:
		void saveExtensionFiltersChanged(QStringList newExtensions);
		void loadExtensionFiltersChanged(QStringList newExtensions);
		void saveNameFiltersChanged(QStringList newNames);
		void loadNameFiltersChanged(QStringList newNames);

	private:
		QStringList generateFilters(const QStringList& extensions);
	protected:
		QMap<QString, MeshIOInterface*> m_loadMap;
		QMap<QString, MeshIOInterface*> m_saveMap;
		QList<MeshIOInterface*> m_IOInterfaces;
		QStringList m_loadExtensionList;
		QStringList m_saveExtensionList;
	};
}

#define MESHIO creative_kernel::MeshIOCenter::instance()
#endif // _NULLSPACE_MESHIOCENTER_1589533079831_H

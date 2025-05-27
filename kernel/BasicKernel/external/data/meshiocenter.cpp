#include "meshiocenter.h"
#include "trimesh2/TriMesh.h"
#include <qdebug.h>


namespace creative_kernel
{
	MeshIOCenter MeshIOCenter::s_meshIOCenter;
	MeshIOCenter::MeshIOCenter(QObject* parent)
		:QObject(parent)
	{
		m_loadExtensionList.clear();
		m_saveExtensionList.clear();
	}

	MeshIOCenter::~MeshIOCenter()
	{
	}

	MeshIOCenter& MeshIOCenter::instance()
	{
		return s_meshIOCenter;
	}

	trimesh::TriMesh* MeshIOCenter::load(const QString& name)
	{
		QStringList lists = name.split('.');
		QMap<QString, MeshIOInterface*>::iterator it = m_loadMap.find(lists.back().toLower());
		if (it != m_loadMap.end())
		{
			trimesh::TriMesh* mesh = (*it)->load(name, nullptr);
			if (mesh)
			{
				mesh->need_bbox();
				if (mesh->normals.size() != mesh->vertices.size())
					mesh->need_normals();
			}

			return mesh;
		}
		return nullptr;
	}

	void MeshIOCenter::save(trimesh::TriMesh* mesh, const QString& name)
	{
	}

	trimesh::TriMesh* MeshIOCenter::asyncLoad(const QString& name, qtuser_core::Progressor* progressor)
	{
		QStringList lists = name.split('.');
		QString suffix = lists.back().toLower();
		QMap<QString, MeshIOInterface*>::iterator it = m_loadMap.find(suffix);
		if (it != m_loadMap.end())
		{
			trimesh::TriMesh* mesh = (*it)->load(name, progressor);
			//if (mesh)
			//{
			//	mesh->need_bbox();
			//	if (mesh->normals.size() != mesh->vertices.size())
			//		mesh->need_normals();
			//}

			return mesh;
		}

		if (progressor) progressor->failed("Can't find plugin to load this file");
		return nullptr;
	}

	void MeshIOCenter::addMeshIOInterface(MeshIOInterface* meshIOInterface)
	{
		QStringList newLoadExtensions;
		QStringList newSaveExtensions;
		QStringList loadExtensions = meshIOInterface->loadSupportExtension();
		QStringList saveExtensions = meshIOInterface->loadSupportExtension();

		for (const QString& extension : loadExtensions)
		{
			QMap<QString, MeshIOInterface*>::iterator it = m_loadMap.insert(extension, meshIOInterface);
			if (it != m_loadMap.end())
			{
				m_loadExtensionList << extension;
				newLoadExtensions << extension;
			}
		}
		for (const QString& extension : saveExtensions)
		{
			QMap<QString, MeshIOInterface*>::iterator it = m_saveMap.insert(extension, meshIOInterface);
			if (it != m_saveMap.end())
			{
				m_saveExtensionList << extension;
				newSaveExtensions << extension;
			}
		}

		emit loadExtensionFiltersChanged(newLoadExtensions);
		emit saveExtensionFiltersChanged(newSaveExtensions);
		emit loadNameFiltersChanged(generateFilters(newLoadExtensions));
		emit saveNameFiltersChanged(generateFilters(newSaveExtensions));
	}

	void MeshIOCenter::removeMeshIOInterface(MeshIOInterface* meshIOInterface)
	{

	}

	void MeshIOCenter::clearAll()
	{
		m_loadExtensionList.clear();
		m_saveExtensionList.clear();

		m_IOInterfaces.clear();
		m_loadMap.clear();
		m_saveMap.clear();
	}

	QStringList MeshIOCenter::loadExtensionList()
	{
		return m_loadExtensionList;
	}

	QStringList MeshIOCenter::saveExtensionList()
	{
		return m_saveExtensionList;
	}

	QStringList MeshIOCenter::saveNameFilters()
	{
		return generateFilters(m_saveExtensionList);
	}

	QStringList MeshIOCenter::loadNameFilters()
	{
		return generateFilters(m_loadExtensionList);
	}

	QStringList MeshIOCenter::generateFilters(const QStringList& extensions)
	{
		QStringList filters;
		QString filter;
		for (const QString& ext : extensions)
		{
			filter += QString("*.%1 ").arg(ext);
			filters << filter;
		}

		return filters;
	}

}
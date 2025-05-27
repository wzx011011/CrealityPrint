#include "gridcache.h"
#include "utils/matrixutil.h"
#include "data/modeln.h"

#include "mmesh/trimesh/box2dgrid.h"

using namespace mmesh;
namespace creative_kernel
{
	GridCache::GridCache(QObject* parent)
		:QObject(parent)
	{
	}

	GridCache::~GridCache()
	{
	}

	void GridCache::clear()
	{
		for (QMap<ModelN*, Box2DGrid*>::iterator it = m_grids.begin(); it != m_grids.end(); ++it)
		{
			delete (*it);
		}
		m_grids.clear();
	}

	int GridCache::size() const
	{
		return m_grids.size();
	}

	void GridCache::build(QList<ModelN*>& models, bool buildGlobal)
	{
		for (ModelN* m : models)
		{
			trimesh::fxform xf = qMatrix2Xform(m->globalMatrix());
			Box2DGrid* grid = new Box2DGrid();
			grid->build(m->mesh(), xf, m->isFanZhuan());

			if (buildGlobal) grid->buildGlobalProperties();
			m_grids.insert(m, grid);
		}
	}

	Box2DGrid* GridCache::grid(ModelN* model)
	{
		QMap<ModelN*, Box2DGrid*>::iterator it = m_grids.find(model);
		if (it != m_grids.end()) return (*it);
		return nullptr;
	}

	ModelN* GridCache::check(trimesh::vec2 xy)
	{
		ModelN* model = nullptr;
		for (QMap<ModelN*, Box2DGrid*>::iterator it = m_grids.begin(); it != m_grids.end(); ++it)
		{
			if (it.value()->contain(xy))
			{
				model = it.key();
				break;
			}
		}
		return model;
	}
}
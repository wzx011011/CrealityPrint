#include "cacheinterface.h"

namespace creative_kernel
{
	TrimeshCacheField* getCache(TriMeshPtr mesh)
	{
		return getTrimeshCache()->get(mesh);
	}

	void cache(TriMeshPtr mesh)
	{
		getTrimeshCache()->cache(mesh);
	}

	void clearCache(bool noReferrence)
	{
		getTrimeshCache()->clear(noReferrence);
	}
}
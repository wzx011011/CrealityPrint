#ifndef CREATIVE_KERNEL_CACHEINTERFACE_1593676766939_H
#define CREATIVE_KERNEL_CACHEINTERFACE_1593676766939_H
#include "basickernelexport.h"
#include "data/trimeshcache.h"

namespace creative_kernel
{
	BASIC_KERNEL_API TrimeshCacheField* getCache(TriMeshPtr mesh);
	BASIC_KERNEL_API void cache(TriMeshPtr mesh);
	BASIC_KERNEL_API void clearCache(bool noReferrence = true);
}
#endif // CREATIVE_KERNEL_CACHEINTERFACE_1593676766939_H
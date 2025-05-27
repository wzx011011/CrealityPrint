#ifndef CREATIVE_KERNEL_TRIMESHCACHE_1593675482914_H
#define CREATIVE_KERNEL_TRIMESHCACHE_1593675482914_H
#include "basickernelexport.h"
#include "qtusercore/module/singleton.h"
#include "trimeshheader.h"
#include <QtCore/QList>

#include <Qt3DRender/QGeometry>
#include "qtusercore/module/progressor.h"
#include "data/geometrydata.h"

namespace creative_kernel
{
	class TrimeshCacheField
	{
		friend class TrimeshCache;
	public:
		TrimeshCacheField();
		~TrimeshCacheField();

		Qt3DRender::QGeometry* getGeometry(int optype = 0, qtuser_core::Progressor *progressor=nullptr);

		Qt3DRender::QGeometry* getGeometryByData(int optype, GeometryData *geometryData);
	protected:
		TriMeshPtr mesh;
		Qt3DRender::QGeometry* m_faceGeometry;
		Qt3DRender::QGeometry* m_lineGeometry;
		Qt3DRender::QGeometry* m_edgeGeometry;
	};

	class BASIC_KERNEL_API TrimeshCache
	{
		SINGLETON_DECLARE(TrimeshCache)
	public:
		virtual ~TrimeshCache();

		TrimeshCacheField* get(TriMeshPtr mesh);
		TrimeshCacheField* cache(TriMeshPtr mesh);

		void clear(bool noReferrence = true);
	protected:
		QList<TrimeshCacheField*> m_fileds;
	};

	SINGLETON_EXPORT(BASIC_KERNEL_API, TrimeshCache)
}
#endif // CREATIVE_KERNEL_TRIMESHCACHE_1593675482914_H
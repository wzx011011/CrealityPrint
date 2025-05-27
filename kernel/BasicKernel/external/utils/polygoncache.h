#ifndef CREATIVE_KERNEL_POLYGONCACHE_1592554938580_H
#define CREATIVE_KERNEL_POLYGONCACHE_1592554938580_H
#include "basickernelexport.h"
#include "trimesh2/Vec.h"

namespace creative_kernel
{
	class BASIC_KERNEL_API PolygonCache : public QObject
	{
		Q_OBJECT
	public:
		PolygonCache(QObject* parent = nullptr);
		virtual ~PolygonCache();

		void save(const QString& file);
		void load(const QString& file);

		std::vector<trimesh::dvec2> m_points;
		std::vector<std::vector<int>> m_polygons;
	};
}
#endif // CREATIVE_KERNEL_POLYGONCACHE_1592554938580_H
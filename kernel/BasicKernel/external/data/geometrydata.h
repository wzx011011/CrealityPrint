#ifndef CREATIVE_KERNEL_GEOMETRY_DATA_H
#define CREATIVE_KERNEL_GEOMETRY_DATA_H

#include "basickernelexport.h"
#include <QtCore/qbytearray.h>

namespace creative_kernel
{
	class BASIC_KERNEL_API GeometryData
	{
	public:
		GeometryData();
		virtual ~GeometryData();

	public:
		QByteArray m_positionByteArray;
		QByteArray m_normalByteArray;
		QByteArray m_indexByteArray;
		QByteArray m_texcoordArray;

		int m_fcount;
		int m_vcount;
		int m_count;

	};
}
#endif // CREATIVE_KERNEL_GRIDCACHE_1594112048273_H
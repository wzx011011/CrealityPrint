#include "polygoncache.h"
#include <QtCore/QFile>
#include <QtCore/QDataStream>

namespace creative_kernel
{
	PolygonCache::PolygonCache(QObject* parent)
		:QObject(parent)
	{
	}
	
	PolygonCache::~PolygonCache()
	{
	}

	void PolygonCache::save(const QString& fileName)
	{
		QFile file(fileName);
		if (file.open(QIODevice::WriteOnly))
		{
			QDataStream stream(&file);
			int pointSize = (int)m_points.size();
			stream << pointSize;
			if (pointSize > 0)
				stream.writeBytes((const char*)&m_points.at(0), 2 * sizeof(double) * pointSize);

			int polygonSize = (int)m_polygons.size();
			stream << polygonSize;
			for (int i = 0; i < polygonSize; ++i)
			{
				std::vector<int>& polygon = m_polygons.at(i);
				int pSize = (int)polygon.size();
				stream << pSize;
				if(pSize > 0)
					stream.writeBytes((const char*)&polygon.at(0), sizeof(int) * pSize);
			}
		}
		file.close();
	}

	void PolygonCache::load(const QString& fileName)
	{
		QFile file(fileName);
		if (file.open(QIODevice::ReadOnly))
		{
			QDataStream stream(&file);
			int pointSize = 0;
			stream >> pointSize;
			if (pointSize > 0)
			{
				m_points.resize(pointSize);
				char* buffer = 0;
				uint size = 2 * sizeof(double) * pointSize;
				stream.readBytes(buffer, size);

				memcpy(&m_points.at(0), buffer, size);
			}
				
			int polygonSize = 0;
			stream >> polygonSize;
			if (polygonSize > 0) m_polygons.resize(polygonSize);
			for (int i = 0; i < polygonSize; ++i)
			{
				std::vector<int>& polygon = m_polygons.at(i);
				int pSize = 0;
				stream >> pSize;
				if (pSize > 0)
				{
					polygon.resize(pSize);
					char* buffer = 0;
					uint size = sizeof(int) * pSize;
					stream.readBytes(buffer, size);

					memcpy(&polygon.at(0), buffer, size);
				}
			}
		}
		file.close();
	}
}
#ifndef MESH_DATA_LIST_H
#define MESH_DATA_LIST_H
#include <vector>
#include "MeshDataListSettings.h"
#include <memory>

namespace cxsw
{
    class MeshDataListSettings;
    class MeshData;
    class  MeshDataList
    {
    public:
        MeshDataList();
        ~MeshDataList();

        bool addMeshData(std::shared_ptr<MeshData> mesh_data);
        std::vector<std::shared_ptr<MeshData>>& GetMesheData();
        void clearMesheData();
        MeshDataListSettings* GetSettings();
    private:
        std::vector<std::shared_ptr<MeshData>> m_mesheData;
        MeshDataListSettings* m_meshdatalistsettings;
    };
}

#endif

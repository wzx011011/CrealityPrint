#include "meshdatalist.h"

namespace cxsw
{
    std::vector<std::shared_ptr<MeshData>>& cxsw::MeshDataList::GetMesheData()
    {
        return m_mesheData;
    }

    MeshDataListSettings* cxsw::MeshDataList::GetSettings()
    {
        return m_meshdatalistsettings;
    }

    MeshDataList::MeshDataList()
    {
        m_meshdatalistsettings = new MeshDataListSettings;
        m_meshdatalistsettings->LoadDefault();
        m_mesheData.clear();
    }

    MeshDataList::~MeshDataList()
    {
        if (m_meshdatalistsettings)
        {
            delete m_meshdatalistsettings;
            m_meshdatalistsettings = nullptr;
        }
    }

    bool MeshDataList::addMeshData(std::shared_ptr<MeshData> mesh_data)
    {
        m_mesheData.push_back(mesh_data);

        return true;
    }

    void MeshDataList::clearMesheData()
    {
        m_mesheData.clear();
    }

}


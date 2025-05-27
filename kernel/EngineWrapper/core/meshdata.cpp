#include "meshdata.h"

namespace cxsw
{

    MeshData::MeshData() : m_idMesh(0), m_strModeName("model")
    {
        m_modesettings = new ModeSettings;
        m_modesettings->LoadDefault();
    }

    MeshData::~MeshData()
    {
        if (m_modesettings)
        {
            delete m_modesettings;
            m_modesettings = nullptr;
        }
    }

    int MeshData::getId() const
    {
        return this->m_idMesh;
    }

    void MeshData::setId(const int idMesh)
    {
        if (idMesh != this->m_idMesh)
        {
            this->m_idMesh = idMesh;
        }
    }

    std::string MeshData::getModeName() const
    {
        return this->m_strModeName;
    }

    void MeshData::setModeName(const std::string& strModeName)
    {
        if (this->m_strModeName.compare(strModeName))
        {
            this->m_strModeName = strModeName;
        }
    }

    void MeshData::setMesh(std::shared_ptr<trimesh::TriMesh> mesh)
    {
        m_mesh = mesh;
    }

    void MeshData::setMatrix(const trimesh::fxform* matrix)
    {
        m_matrix = *matrix;
    }

    std::shared_ptr<trimesh::TriMesh> MeshData::GetMesh()const
    {
        return m_mesh;
    }

    trimesh::fxform MeshData::GetMatrix()const
    {
        return m_matrix;
    }

    ModeSettings* MeshData::GetSettings() const
    {
        return m_modesettings;
    }

    void MeshData::setSettings(ModeSettings* setting)
    {
        m_modesettings = setting;
    }
}




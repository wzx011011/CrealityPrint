#ifndef MESH_DATA_H
#define MESH_DATA_H

#include "trimesh2/TriMesh.h"
#include "trimesh2/XForm.h"
#include "modeSettings.h"
#include <memory>
namespace cxsw
{
    //class Settings;
    class  MeshData
    {
    public:
        MeshData();
        ~MeshData();

        int getId() const;
        void setId(const int idMesh);

        std::string getModeName() const;
        void setModeName(const std::string& strModeName);

        void setMesh(std::shared_ptr<trimesh::TriMesh>);
        void setMatrix(const trimesh::fxform* matrix);
        std::shared_ptr<trimesh::TriMesh> GetMesh()const;
        trimesh::fxform GetMatrix()const;
        ModeSettings* GetSettings()const;
        void setSettings(ModeSettings* modemettings);

    private:
        int m_idMesh;
        std::shared_ptr<trimesh::TriMesh> m_mesh;
        //trimesh::TriMesh* m_mesh ;
        trimesh::fxform m_matrix;
        ModeSettings* m_modesettings;
        std::string m_strModeName;


    };
}

#endif





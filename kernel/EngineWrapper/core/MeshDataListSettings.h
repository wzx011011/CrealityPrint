#ifndef MESHDATALISTSETTINGS_H
#define MESHDATALISTSETTINGS_H
#include <map>
#include <string>
//#include <unordered_map>
namespace cxsw {
    class MeshDataListSettings
    {
    public:
        MeshDataListSettings();
        ~MeshDataListSettings();
        //����Ĭ�ϲ���
        void LoadDefault();
        std::map<std::string, std::string>& GetSetting();
    private:
        std::map<std::string, std::string> m_DefaultSettings;
    };
}

#endif
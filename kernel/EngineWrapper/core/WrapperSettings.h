#ifndef WRAPPER_SETTINGS_H
#define WRAPPER_SETTINGS_H
#include "Settings.h"

namespace cxsw {
    struct GlobalSettings {
        std::unordered_map<std::string, std::string> m_EditSettings;
        std::unordered_map<std::string, std::string> DefaultSettings;
    };

    //struct MeshDataListSettings {
    //	std::unordered_map<std::string, std::string> m_EditSettings;
    //	std::unordered_map<std::string, std::string> m_DefaultSettings;
    //};


    class WrapperSettings
    {
    public:
        WrapperSettings();
        ~WrapperSettings();

        //��ʼ�� WrapperSettings
        void Init(Settings* settings);

        //����Ĭ�ϲ���
        void LoadDefault();

        //��Ӳ���������Ѿ������򸲸�
        void add(const std::string& key, const std::string& value);

        //��Ӳ���������Ѿ������򸲸�
        void add(std::pair<std::string, std::string> pair_element);

    public:
        std::unordered_map<std::string, std::string> m_DefaultSettings;
        GlobalSettings m_GlobalSettings;
        //MeshDataListSettings m_MeshDataListSettings;
    };

}


#endif //WRAPPER_SETTINGS_H


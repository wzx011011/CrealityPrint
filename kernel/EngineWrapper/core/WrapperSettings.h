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

        //初始化 WrapperSettings
        void Init(Settings* settings);

        //加载默认参数
        void LoadDefault();

        //添加参数，如果已经有了则覆盖
        void add(const std::string& key, const std::string& value);

        //添加参数，如果已经有了则覆盖
        void add(std::pair<std::string, std::string> pair_element);

    public:
        std::unordered_map<std::string, std::string> m_DefaultSettings;
        GlobalSettings m_GlobalSettings;
        //MeshDataListSettings m_MeshDataListSettings;
    };

}


#endif //WRAPPER_SETTINGS_H


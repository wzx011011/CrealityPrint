#ifndef MODE_SETTINGS_H
#define MODE_SETTINGS_H
#include <string>
#include <vector>
#include <unordered_map>
namespace cxsw {
    class ModeSettings
    {
    public:
        ModeSettings();
        ~ModeSettings();
        std::unordered_map<std::string, std::string>& GetSetting();
        //加载默认参数
        void LoadDefault();

        //添加参数，如果已经有了则覆盖
        void add(const std::string& key, const std::string value);

    private:
        std::unordered_map<std::string, std::string> defaultsettings;

    };
}


#endif //MODE_SETTINGS_H

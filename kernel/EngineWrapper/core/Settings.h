#ifndef SETTINGS_H
#define SETTINGS_H
#include "enginewrapperexport.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#ifdef WIN32
#include <io.h>
#include <direct.h>
#endif
#include "slicesettings.h"
#include "machineSettings.h"
#include "materialSettings.h"
#include "modeSettings.h"


namespace cxsw {

#define DEFAULTCONFIG "DefaultConfig.ini"
#define MachineType "MachineType.ini"

    struct BasicData {
        std::string	name;	//	名字  	Creality Slicer Base Description	
        std::string  time;	//	时间	    2020-04-15
        std::string  author;	//	作者	    Creality3D
        float	version;//	版本号   1.0
        std::string	sub_name;//子配置名字  "Creality CR-5",
        std::string  sub_time; //子配置时间  "2020-04-15",
        std::string  sub_author; //子配置作者"Creality3D",
        float   sub_version;//子配置版本号 1.0
        std::string sub_inherits; //子配置继承文件："creality_base"
    };

    //Struct for writing a string to a stream in an escaped form
    struct Escaped
    {
        const char* str;

        /*!
         * Streaming function which replaces escape sequences with extra slashes
         */
        friend inline std::ostream& operator<<(std::ostream& os, const Escaped& e)
        {
            for (const char* char_p = e.str; *char_p != '\0'; char_p++)
            {
                switch (*char_p)
                {
                case '\a':  os << "\\a"; break;
                case '\b':  os << "\\b"; break;
                case '\f':  os << "\\f"; break;
                case '\n':  os << "\\n"; break;
                case '\r':  os << "\\r"; break;
                case '\t':  os << "\\t"; break;
                case '\v':  os << "\\v"; break;
                case '\\':  os << "\\\\"; break;
                case '\'':  os << "\\'"; break;
                case '\"':  os << "\\\""; break;
                case '\?':  os << "\\\?"; break;
                default: os << *char_p;
                }
            }
            return os;
        }
    };

    class ENGINE_WRAPPER_API Settings :public SliceSettings, public machineSettings, public materialSettings
    {
    public:

        Settings();

        //添加参数，如果已经有了则覆盖
        void add(const std::string& key, const std::string value);

        //添加参数，如果已经有了则覆盖
        void add(std::pair<std::string, std::string> pair_element);

        //保存Setting配置
        void Save(const char* filename, char* suffix = ".profile");

        //加载Setting配置
        void Load(const char* filename, char* suffix=".profile");

        //获取属性key 的 对应值
        template<typename A> A get(const std::string& key) const;

        //获取所有的Setting配置参数
        const std::string getAllSettingsString() const;

        //判断是否有该参数
        bool has(const std::string& key) const;

        //清空当前Setting类
        void ClearSettings();

        //switch Member类成员判断
        void assignment(const char* str, const std::string value);

        //map数据赋值到Member类成员中
        void Settings2Member();

        //更新界面参数
        void UpDataSetting();

        //加载机型列表
        void LoadMachineType();

        //设置打印 质量高中低的3个配置：
        void SetQualityType(const char* filename="middle");

        std::unordered_map<std::string, std::string>& GetEditSettings();
        BasicData   m_BasicData;
    private:
        //settings参数格式为：    属性：值
        std::unordered_map<std::string, std::string> EditSettings;
        std::unordered_map<std::string, std::string> m_MachineType;
    };
}



#endif //SETTINGS_H

#include "json.h"
#include <algorithm>
#include <QtCore/QFile>
#define MAX_PATH          260


namespace cxsw {

    JsonFile::JsonFile()
    {
    }

    JsonFile::~JsonFile()
    {

    }

    int JsonFile::loadJSON(const std::string& json_filename, Settings& settings)
    {
        QFile file(json_filename.c_str());
        bool opened = file.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray barray;
        if (opened == true)
        {
            barray = file.readAll();
            //std::cout << barray.data() << std::endl;
        }
        rapidjson::Document json_document;
        json_document.ParseInsitu(barray.data());
        if (json_document.HasParseError())
        {
            //logError("Error parsing JSON (offset %u): %s\n", static_cast<unsigned int>(json_document.GetErrorOffset()), GetParseError_En(json_document.GetParseError()));
            return 2;
        }

        if (json_document.HasMember("BasicData") && json_document["BasicData"].IsObject())
        {
            loadJSONBasicData(json_document["BasicData"], settings, json_filename);
        }
        if (json_document.HasMember("machine_settings") && json_document["machine_settings"].IsObject())
        {
            loadJSONSettings(json_document["machine_settings"], settings);
        }
        if (json_document.HasMember("resolution") && json_document["resolution"].IsObject())
        {
            loadJSONSettings(json_document["resolution"], settings);
        }

        if (json_document.HasMember("material") && json_document["material"].IsObject())
        {
            loadJSONSettings(json_document["material"], settings);
        }
        if (json_document.HasMember("infill") && json_document["infill"].IsObject())
        {
            loadJSONSettings(json_document["infill"], settings);
        }
        if (json_document.HasMember("travel") && json_document["travel"].IsObject())
        {
            loadJSONSettings(json_document["travel"], settings);
        }
        if (json_document.HasMember("support") && json_document["support"].IsObject())
        {
            loadJSONSettings(json_document["support"], settings);
        }
        if (json_document.HasMember("platform_adhesion") && json_document["platform_adhesion"].IsObject())
        {
            loadJSONSettings(json_document["platform_adhesion"], settings);
        }
        if (json_document.HasMember("dual") && json_document["dual"].IsObject())
        {
            loadJSONSettings(json_document["dual"], settings);
        }
        if (json_document.HasMember("meshfix") && json_document["meshfix"].IsObject())
        {
            loadJSONSettings(json_document["meshfix"], settings);
        }
        if (json_document.HasMember("SpecialFunction") && json_document["SpecialFunction"].IsObject())
        {
            loadJSONSettings(json_document["SpecialFunction"], settings);
        }

        if (json_document.HasMember("overrides") && json_document["overrides"].IsObject())
        {
            loadJSONSettings(json_document["overrides"], settings);
        }
        return 0;
    }

    void JsonFile::loadJSONSettings(const rapidjson::Value& element, Settings& settings)
    {
        if (element.HasMember("children"))
        {
            for (rapidjson::Value::ConstMemberIterator setting = element["children"].MemberBegin(); setting != element["children"].MemberEnd(); setting++)
            {
                const std::string name = setting->name.GetString();
                const rapidjson::Value& setting_object = setting->value;
                if (!setting_object.IsObject())
                {
                    //logError("JSON setting %s is not an object!\n", name.c_str());
                    continue;
                }
                if (setting_object.HasMember("children"))
                {
                    loadJSONSettings(setting_object, settings);
                }

                if (!setting_object.HasMember("default_value"))
                {
                    //logWarning("JSON setting %s has no default_value!\n", name.c_str());
                    continue;
                }
                const rapidjson::Value& default_value = setting_object["default_value"];
                std::string value_string;
                if (default_value.IsString())
                {
                    value_string = default_value.GetString();
                }
                else if (default_value.IsTrue())
                {
                    value_string = "true";
                }
                else if (default_value.IsFalse())
                {
                    value_string = "false";
                }
                else if (default_value.IsNumber())
                {
                    std::ostringstream ss;
                    ss << default_value.GetDouble();
                    value_string = ss.str();
                }
                else
                {
                    //logWarning("Unrecognized data type in JSON setting %s\n", name.c_str());
                    continue;
                }
                settings.add(name, value_string);
            }
        }
        else
        {
            return;
            //logError("this element no children);
        }
    }

    void JsonFile::loadJSONBasicData(const rapidjson::Value& element, Settings& settings, const std::string& json_filename)
    {
        for (rapidjson::Value::ConstMemberIterator setting = element.MemberBegin(); setting != element.MemberEnd(); setting++)
        {
            const std::string name = setting->name.GetString();
            if (name == "sub_inherits")
            {
                const static QString prefix = QString(":/config/config/");
                std::string parent_file = prefix.toStdString()+ setting->value.GetString() + ".json";
                if (parent_file == "")
                {
                    //logError("Inherited JSON file \"%s\" not found.\n", document["inherits"].GetString());

                }
                int error_code = loadJSON(parent_file, settings); //Head-recursively load the settings file that we inherit from.
                if (error_code)
                {
                    //logError("Inherited JSON file \"%s\" not found.\n", document["inherits"].GetString());
                }
            }
            else
            {
                std::string value_string;
                if (setting->value.IsString())
                {
                    value_string = setting->value.GetString();
                }
                else if (setting->value.IsTrue())
                {
                    value_string = "true";
                }
                else if (setting->value.IsFalse())
                {
                    value_string = "false";
                }
                else if (setting->value.IsNumber())
                {
                    std::ostringstream ss;
                    ss << setting->value.GetDouble();
                    value_string = ss.str();
                }
                else
                {
                    //logWarning("Unrecognized data type in JSON setting %s\n", name.c_str());
                    continue;
                }
                settings.add(name, value_string);
            }
        }
    }

}



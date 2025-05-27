#pragma once
#include "rapidjson/document.h"//Loading JSON documents to get settings from them.
#include "rapidjson/rapidjson.h"
#include "rapidjson/error/en.h"
#include "rapidjson/filereadstream.h"
#include "Settings.h"
#include <unordered_set>
#include <sstream>
#include <fstream> //To check if files exist.

#ifdef _WIN32
#include <windows.h> // GetFullPathNameA
#else
#include <libgen.h> // dirname
#include <cstring> // std::strcpy
#endif

namespace cxsw {

    class JsonFile
    {
    public:
        JsonFile();
        ~JsonFile();

        int loadJSON(const std::string& json_filename, Settings& settings);
        void loadJSONSettings(const rapidjson::Value& element, Settings& settings);
        void loadJSONBasicData(const rapidjson::Value& element, Settings& settings, const std::string& json_filename);

    };

}


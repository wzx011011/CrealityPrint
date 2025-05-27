#include "Settings.h"
#include <string>
#include <QtCore/QFile>
#include <QDir>
#include <QtCore/QTextStream>
namespace cxsw {

    Settings::Settings()
    {

    }

    void Settings::add(const std::string& key, const std::string value)
    {
        if (EditSettings.find(key) != EditSettings.end()) //Already exists.
        {
            EditSettings[key] = value;
        }
        else //New setting.
        {
            EditSettings.emplace(key, value);
        }
    }

    void Settings::add(std::pair<std::string, std::string> pair_element)
    {
        if (EditSettings.find(pair_element.first) != EditSettings.end()) //Already exists.
        {
            EditSettings[pair_element.first] = pair_element.second;
        }
        else //New setting.
        {
            EditSettings.emplace(pair_element.first, pair_element.second);
        }
    }


    void Settings::Save(const char* filename, char* suffix)
    {
        QString qstr = filename;
        qstr += suffix;
        QFile file(qstr);
        bool opened = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QByteArray barray;
        if (opened == true)
        {
            for (const std::pair<std::string, std::string> pair : EditSettings)
            {
                char buffer[4096];
                snprintf(buffer, 4096, "\"%s\"=\"%s\"\r\n", Escaped{ pair.first.c_str() }.str, Escaped{ pair.second.c_str() }.str);
                file.write(buffer);
            }
            file.close();
        }
    }

    void Settings::Load(const char* filename, char* suffix)
    {
        QString qstr = filename;
        qstr += suffix;
        QFile file(qstr);
        bool opened = file.open(QIODevice::ReadOnly);
        QByteArray barray;
        if (opened == true)
        {
            char buffer[1];
            qint64 length = 0;
            int icount = 0;
            QByteArray b;
            std::string strkey;
            std::string Temp;
            std::string strvalue;
            //bool  isture=false;
            while ((length = file.read(buffer, 1) != 0))
            {
                if (buffer[0] != '\"' && buffer[0] != '=' && icount>0)
                {
                    Temp += buffer[0];
                }
                if (buffer[0] == '\"')
                {
                    icount++;
                }
                if (icount==2 && buffer[0] == '=')
                {
                    strkey = Temp;
                    Temp.clear();
                }
                if (4 == icount)
                {
                    strvalue = Temp;
                    icount = 0;
                    Temp.clear();
                    if (EditSettings.find(strkey) != EditSettings.end()) //Already exists.
                    {
                        EditSettings[strkey] = strvalue;
                    }
                    else //New setting.
                    {
                        EditSettings.emplace(strkey, strvalue);
                    }
                }
            }
        }
        file.close();
    }


    template<> std::string Settings::get<std::string>(const std::string& key) const
    {
        //If this settings base has a setting value for it, look that up.
        if (EditSettings.find(key) != EditSettings.end())
        {
            return EditSettings.at(key);
        }

        //logError("Trying to retrieve setting with no value given: '%s'\n", key.c_str());
        std::exit(2);
    }

    template<> double Settings::get<double>(const std::string& key) const
    {
        return atof(get<std::string>(key).c_str());
    }

    template<> size_t Settings::get<size_t>(const std::string& key) const
    {
        return std::stoul(get<std::string>(key).c_str());
    }

    template<> bool Settings::get<bool>(const std::string& key) const
    {
        const std::string& value = get<std::string>(key);
        if (value == "on" || value == "yes" || value == "true" || value == "True")
        {
            return true;
        }
        const int num = atoi(value.c_str());
        return num != 0;
    }

    const std::string Settings::getAllSettingsString() const
    {
        std::stringstream sstream;
        for (const std::pair<std::string, std::string> pair : EditSettings)
        {
            char buffer[4096];
            // snprintf(buffer, 4096, "%s=\"%s\"\r\n", pair.first.c_str(), Escaped{ pair.second.c_str() }.str);
            snprintf(buffer, 4096, "\"%s\"=\"%s\"\r\n", Escaped{ pair.first.c_str() }.str, Escaped{ pair.second.c_str() }.str);
            sstream << buffer;
        }
        return sstream.str();
    }

    bool Settings::has(const std::string& key) const
    {
        return EditSettings.find(key) != EditSettings.end();
    }

    void Settings::ClearSettings()
    {
        EditSettings.clear();
    }



    typedef std::uint64_t hash_t;

    constexpr hash_t prime = 0x100000001B3ull;
    constexpr hash_t basis = 0xCBF29CE484222325ull;
    hash_t hash_(char const* str)
    {
        hash_t ret{ basis };

        while (*str) {
            ret ^= *str;
            ret *= prime;
            str++;
        }

        return ret;
    }

    constexpr hash_t hash_compile_time(char const* str, hash_t last_value = basis)
    {
        return *str ? hash_compile_time(str + 1, (*str ^ last_value) * prime) : last_value;
    }

    constexpr unsigned long long operator "" _hash(char const* p, size_t)
    {
        return hash_compile_time(p);
    }


    void Settings::assignment(const char* str, const std::string value)
    {
    }

    void Settings::Settings2Member()
    {
        for (const std::pair<std::string, std::string> pair : EditSettings)
        {
            assignment(pair.first.c_str(), pair.second);
        }
    }

    void Settings::UpDataSetting()
    {
        add(export_prime_tower_enable());
        add(export_prime_tower_enable());
        add(export_prime_tower_size());
        add(export_prime_tower_min_volume());
        add(export_prime_tower_position_x());
        add(export_prime_tower_position_y());

        add(export_infill_extruder_nr());
        add(export_infill_sparse_density());
        add(export_infill_pattern());
        add(export_infill_sparse_thickness());

        add(export_meshfix_union_all());
        add(export_meshfix_union_all_remove_holes());
        add(export_meshfix_extensive_stitching());
        add(export_meshfix_keep_open_polygons());
        add(export_meshfix_maximum_resolution());
        add(export_meshfix_maximum_travel_resolution());
        add(export_meshfix_maximum_deviation());
        add(export_multiple_mesh_overlap());
        add(export_carve_multiple_volumes());
        add(export_alternate_carve_order());
        add(export_remove_empty_first_layers());

        add(export_adhesion_type());
        add(export_adhesion_extruder_nr());
        add(export_skirt_line_count());
        add(export_skirt_gap());
        add(export_brim_line_count());
        add(export_raft_margin());
        add(export_raft_surface_thickness());
        add(export_raft_interface_thickness());
        add(export_raft_base_thickness());

        add(export_layer_height());
        add(export_layer_height_0());
        add(export_line_width());
        add(export_infill_line_width());
        add(export_support_line_width());
        add(export_initial_layer_line_width_factor());
        add(export_wall_extruder_nr());
        add(export_wall_line_count());
        add(export_top_bottom_thickness());
        add(export_top_layers());
        add(export_bottom_layers());
        add(export_top_bottom_pattern());
        add(export_skin_angles());
        add(export_outer_inset_first());
        add(export_alternate_extra_perimeter());
        add(export_fill_outline_gaps());
        add(export_xy_offset_layer_0());
        add(export_z_seam_type());
        add(export_z_seam_x());
        add(export_z_seam_y());
        add(export_z_seam_corner());
        add(export_ironing_enabled());
        add(export_ironing_pattern());
        add(export_cool_fan_enabled());

        add(export_print_sequence());
        add(export_magic_mesh_surface_mode());
        add(export_magic_spiralize());
        add(export_conical_overhang_enabled());
        add(export_conical_overhang_angle());
        add(export_coasting_enable());
        add(export_coasting_volume());
        add(export_coasting_speed());

        add(export_support_enable());
        add(export_support_extruder_nr());
        add(export_support_type());
        add(export_support_angle());
        add(export_support_pattern());
        add(export_support_infill_sparse_thickness());
        add(export_minimum_support_area());
        add(export_support_roof_height());
        add(export_support_bottom_height());
        add(export_support_roof_pattern());
        add(export_support_bottom_pattern());
        add(export_support_use_towers());
        add(export_support_tower_diameter());

        add(export_speed_print());
        add(export_speed_infill());
        add(export_speed_wall());
        add(export_speed_support());
        add(export_speed_prime_tower());
        add(export_speed_travel());
        add(export_speed_layer_0());
        add(export_skirt_brim_speed());
        add(export_speed_equalize_flow_enabled());
        add(export_retraction_combing());
        add(export_travel_retract_before_outer_wall());
        add(export_travel_avoid_other_parts());
        add(export_retraction_enable());
        add(export_retraction_amount());
        add(export_retraction_speed());
        add(export_retraction_extra_prime_amount());
        add(export_retraction_hop_enabled());
        add(export_retraction_hop_only_when_collides());
        add(export_retraction_hop());

        add(export_machine_name());
        add(export_machine_width());
        add(export_machine_depth());
        add(export_machine_height());
        add(export_machine_center_is_zero());
        add(export_machine_extruder_count());
        add(export_machine_nozzle_size());
        add(export_machine_start_gcode());
        add(export_machine_end_gcode());
        add(export_machine_gcode_flavor());
        add(export_machine_head_with_fans_polygon());

        add(export_material_diameter());
        add(export_material_type());
        add(export_material_print_temperature());
        add(export_material_bed_temperature());
        add(export_material_bed_temperature_layer_0());
    }

    void Settings::SetQualityType(const char* filename)
    {
        Load(filename, ".ini");
    }

    void Settings::LoadMachineType()
    {
        const static QString prefix = QString(":/config/config/");
        std::string DefaultPathName = prefix.toStdString() + MachineType;
        QFile file(DefaultPathName.c_str());
        bool opened = file.open(QIODevice::ReadOnly);
        QByteArray barray;
        if (opened == true)
        {
            char buffer[1];
            qint64 length = 0;
            int icount = 0;
            QByteArray b;
            std::string strkey;
            std::string Temp;
            std::string strvalue;
            //bool  isture=false;
            while ((length = file.read(buffer, 1) != 0))
            {
                if (buffer[0] != '\"' && buffer[0] != '=' && icount > 0)
                {
                    Temp += buffer[0];
                }
                if (buffer[0] == '\"')
                {
                    icount++;
                }
                if (icount == 2 && buffer[0] == '=')
                {
                    strkey = Temp;
                    Temp.clear();
                }
                if (4 == icount)
                {
                    strvalue = Temp;
                    icount = 0;
                    Temp.clear();
                    if (m_MachineType.find(strkey) != m_MachineType.end()) //Already exists.
                    {
                        m_MachineType[strkey] = strvalue;
                    }
                    else //New setting.
                    {
                        m_MachineType.emplace(strkey, strvalue);
                    }
                }
            }
        }
        file.close();
    }

    std::unordered_map<std::string, std::string>& Settings::GetEditSettings()
	{
		return EditSettings;
	}

}

















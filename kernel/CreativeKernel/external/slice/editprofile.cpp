#include "editprofile.h"

#include <QDebug>
#include "us/slicermanager.h"

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlProperty>

Editprofile::Editprofile()
{
	//m_layerconfg = new Layerconfig;
    //std::shared_ptr<Layerconfig> m_layerconfg(new Layerconfig);
}

Editprofile::~Editprofile()
{

}

void Editprofile::print_quality(int index)
{
	QString qualityName = "middle";
	switch (index)
	{
	case 0:
        qualityName = "high";
		SlicerManager::instance().setCurrentQuality(qualityName);
		break;
    case 1:
		qualityName = "middle";
		SlicerManager::instance().setCurrentQuality(qualityName);
        break;
    case 2:
        qualityName = "low";
        SlicerManager::instance().setCurrentQuality(qualityName);
        break;
	default:
        SlicerManager::instance().setCurrentQuality(qualityName);
		break;
	}
}

void Editprofile::settingconfig(QString type, QString name, QString value)
{
	if ("layer" == type)
	{
        //m_layerconfg->setsettingvalue(name, value);
	}
	else
	{
		SlicerManager::instance().insertSettingCache(name, value);
	}

}

void Editprofile::settingProfile(QString name, QString value)
{
    if ("infill_sparse_density" == name)
    {
        //setInfill_sparse_density(name, value);
    }
    SlicerManager::instance().insertSettingCache(name, value);
}

QString Editprofile::getValue(QString key)
{
    auto vh = SlicerManager::instance().globalsettings_modify()->settings().find(key);
    if (vh == SlicerManager::instance().globalsettings_modify()->settings().end())
    {
        qDebug() << "getSetVal key = " + key + " failed !";
        return "";
    }
	return SlicerManager::instance().globalsettings_modify()->settings().value(key)->value().toString();
}

void Editprofile::editProfiled()
{
//     QObject *pRightPanel = m_qmlUI->findChild<QObject *>("RightInfoPanel");
//     if(pRightPanel)
//     {
//         QObject *pSelectMachine = pRightPanel->findChild<QObject *>("editprofileobj");
//         if(pSelectMachine)
//         {
//             //layerHeight
//             QQmlProperty::write(m_qmlUI, "layerHeight", "2.0");
// 
//         }
//     }
}

void Editprofile::setqmlUI(QObject* qmlUI)
{
   // m_qmlUI = qmlUI;
}

void Editprofile::getALLprofile(std::map<QString,QString>& value)
{
    value.clear();

    QString strPara1 = "layer_height,line_width,wall_line_count,infill_sparse_density";
    QString strPara2 = ",material_print_temperature,material_bed_temperature,support_enable,support_angle,adhesion_type";
    QString strPara = strPara1 + strPara2;
    QStringList list = strPara.split(",");

    for (auto val : list)
    {
        //float 
        if ("layer_height" == val
            || "line_width" == val || "min_skin_width_for_expansion" == val)
        {
            value.insert(std::make_pair(val, QString("%1").arg(getValue(val).toDouble())));
        }
        else
        {
            value.insert(std::make_pair(val, getValue(val)));
        }
    }
}

void Editprofile::getALLprofile_dlp(std::map<QString, QString>& value_basic, std::map<QString, QString>& value_gcode)
{
    QString strPara1 = "layer_height";
    QString strPara2 = ",first_exposure_time,first_lift_distance,lifting_speed,first_layer_count,light_off_delay,exposure_time,retract_speed,first_lift_speed,first_lightoff_delay,anti_aliasing,gcode_start,inter_layer,gcode_end";
    QString strPara3 = ",xy_offset_enable,xy_offset,z_offset,zdeep_enable,zthrough_deep_value,minimum_grayscale,maximum_grayscale,lifting_distance";
    QString strPara = strPara1 + strPara2 + strPara3;
    QStringList list = strPara.split(",");

    for (auto val : list)
    {
        //float 
        if ("layer_height" == val)
        {
            value_basic.insert(std::make_pair(val, QString("%1").arg(getValue(val).toDouble())));
        }
        else if ("gcode_start" == val
            || "inter_layer" == val
            || "gcode_end" == val)
        {
            QString str = getValue(val);
            value_basic.insert(std::make_pair(val, str.replace("\\n", "\n")));
        }
        else
        {
            value_basic.insert(std::make_pair(val, getValue(val)));
        }
    }
    //gcode add
}
//FDM machine manager
void Editprofile::getALLMachineprofile(std::map<QString,QString>& value)
{
    value.clear();
    QString strPara1 = "machine_shape,machine_height,machine_width,machine_heated_bed,machine_extruder_type,preview_img_type,machine_gcode_flavor,machine_center_is_zero";
    QString strPara2 = ",machine_extruder_count,machine_head_with_fans_polygon,machine_depth,machine_nozzle_size,machine_start_gcode,machine_end_gcode,has_preview_img,screen_size";
    QString strPara = strPara1 + strPara2;
    QStringList list = strPara.split(",");
    for(auto val: list)
    {
        if ("machine_nozzle_size" == val)
        {
            value.insert(std::make_pair(val, QString("%1").arg(getValue(val).toDouble())));
        }
        else
        {
            value.insert(std::make_pair(val,getValue(val)));
        }
    }
}

//FDM machine manager
void Editprofile::getALLMachineprofile_DLP(std::map<QString,QString>& value)
{
    value.clear();

    QString strPara1 = "resolution_x,resolution_y,machine_width";
    QString strPara2 = ",machine_depth,machine_height,mirror_type";
    QString strPara = strPara1 + strPara2;
    QStringList list = strPara.split(",");

    for(auto val: list)
    {
        if ("machine_depth" == val
            || "machine_height" == val
            || "machine_width" == val)
        {
            value.insert(std::make_pair(val, QString("%1").arg(getValue(val).toDouble())));
        }
        else
        {
            value.insert(std::make_pair(val, getValue(val)));
        }
    }
}

void Editprofile::setInfill_sparse_density(QString name, QString value)
{
	//infill_sparse_density=20
//infill_line_distance = 6
//cross_infill_pocket_size = 6

    QString infill_pattern= getValue(name);
    int infill_pattern_cubic = 3;//default
    if ("grid" == infill_pattern
        || "tetrahedral" == infill_pattern
        || "quarter_cubic" == infill_pattern)
    {
        infill_pattern_cubic = 2;
    } 
    else if ("lines" == infill_pattern
        || "concentric" == infill_pattern
        || "zigzag" == infill_pattern
        || "cross" == infill_pattern
        || "cross_3d" == infill_pattern
        || "gyroid" == infill_pattern)
    {
        infill_pattern_cubic = 1;
    }
// 	else if ("triangles" == infill_pattern
//         || "trihexagon" == infill_pattern
//         || "cubic" == infill_pattern
//         || "cubicsubdiv" == infill_pattern)
// 	{
//         infill_pattern_cubic = 3;
// 	}
	else
	{
		infill_pattern_cubic = 3;
	}
	const double infill_line_width = getValue("line_width").toDouble();
	const double infill_sparse_density = value.toDouble();
	//const int infill_pattern_cubic= 3;//(和选择填充图案有关 此处默认直线 值为3)
	const double infill_line_distance = infill_sparse_density == 0 ? 0 : infill_line_width * 100 / infill_sparse_density * infill_pattern_cubic;
	const double cross_infill_pocket_size = infill_line_distance;

	SlicerManager::instance().insertSettingCache("infill_line_distance", QString::number(infill_line_distance, 10, 6));
	SlicerManager::instance().insertSettingCache("cross_infill_pocket_size", QString::number(cross_infill_pocket_size, 10, 6));
}

void Editprofile::getProfile_AdvancedSetting_Comprehensive(std::map<QString,QString>& value)
{
    value.clear();

    QString strPara_Quality = "layer_height,layer_height_0,line_width,infill_line_width,support_line_width,initial_layer_line_width_factor,infill_wipe_dist";
    QString strPara_Infill = ",infill_extruder_nr,infill_sparse_density,infill_line_distance,infill_pattern,infill_sparse_thickness,zig_zaggify_infill,min_infill_area";
    QString strPara_Adhesion = ",adhesion_type,brim_line_count,brim_gap,skirt_line_count,skirt_gap,raft_margin,raft_surface_thickness,raft_interface_thickness,raft_base_thickness";
    QString strPara_Material = ",material_print_temperature,material_bed_temperature,material_bed_temperature_layer_0,prime_tower_enable,prime_tower_size,prime_tower_min_volume,prime_tower_position_x,prime_tower_position_y";
    QString strPara = strPara_Quality + strPara_Infill + strPara_Adhesion + strPara_Material;
    QStringList list = strPara.split(",");

    for (auto val : list)
    {
        //float
        if ("layer_height" == val || "layer_height_0" == val || "infill_line_width" == val || "support_line_width" == val || "line_width" == val
                || "infill_line_distance" == val  || "infill_sparse_thickness" == val || "brim_line_count" == val
                || "skirt_gap" == val || "raft_margin" == val || "raft_surface_thickness" == val || "raft_interface_thickness" == val|| "raft_base_thickness" == val
                || "material_print_temperature" == val || "material_bed_temperature" == val || " material_bed_temperature_layer_0" == val || "prime_tower_size" == val
                || " prime_tower_position_x" == val || "prime_tower_position_y" == val)
        {
            value.insert(std::make_pair(val, QString("%1").arg(getValue(val).toDouble())));
        }
        else
        {
            value.insert(std::make_pair(val, getValue(val)));
        }
    }
}

void Editprofile::getProfile_AdvancedSetting_Shell(std::map<QString, QString> &value)
{
    value.clear();

    QString strPara_Part1 = "wall_thickness,wall_line_count,wall_0_wipe_dist,roofing_layer_count,top_bottom_thickness,top_layers,bottom_layers,infill_before_walls";
    QString strPara_Part2 = ",top_bottom_pattern,top_bottom_pattern_0,skin_angles,optimize_wall_printing_order,outer_inset_first,fill_outline_gaps";
    QString strPara_Part3 = ",xy_offset_layer_0,z_seam_type,z_seam_x,z_seam_y,z_seam_corner,z_seam_position,ironing_enabled,ironing_pattern,fill_perimeter_gaps,xy_offset_enable,xy_offset,z_offset";
    QString  strPara_Shell = strPara_Part1 + strPara_Part2 + strPara_Part3;
    QStringList list = strPara_Shell.split(",");

    for (auto val : list)
    {
        //float
        if ("wall_0_wipe_dist" == val || "top_bottom_thickness" == val ||"wall_thickness" == val)
        {
            value.insert(std::make_pair(val, QString("%1").arg(getValue(val).toDouble())));
        }
        else
        {
            value.insert(std::make_pair(val, getValue(val)));
        }
    }
}

void Editprofile::getProfile_AdvancedSetting_Support(std::map<QString, QString> &value)
{
    value.clear();

    QString strPara_Part1 = "support_enable,support_extruder_nr,support_type,support_angle,support_pattern";
    QString strPara_Part2 = ",support_infill_sparse_thickness,support_use_towers,support_tower_diameter,minimum_support_area,support_roof_enable";
    QString strPara_Part3 = ",support_roof_height,support_roof_pattern,support_bottom_enable,support_bottom_height,support_bottom_pattern,support_join_distance,support_xy_distance";
    QString strPara_Support = strPara_Part1 + strPara_Part2 + strPara_Part3;
    QStringList list = strPara_Support.split(",");

    for (auto val : list)
    {
        //float
        if ("support_angle" == val || "support_infill_sparse_thickness" == val ||"support_tower_diameter" == val ||"support_tower_diameter" == val
                ||"support_bottom_height" == val ||"support_join_distance" == val ||"support_xy_distance" == val ||"support_roof_height" == val)
        {
            value.insert(std::make_pair(val, QString("%1").arg(getValue(val).toDouble())));
        }
        else
        {
            value.insert(std::make_pair(val, getValue(val)));
        }
    }
}

void Editprofile::getProfile_AdvancedSetting_Speed(std::map<QString, QString> &value)
{
    value.clear();

    QString strPara_Part1 = "cool_fan_enabled,travel_avoid_other_parts,speed_print,speed_infill,speed_prime_tower,speed_travel";
    QString strPara_Part2 = ",speed_print_layer_0,skirt_brim_speed,speed_equalize_flow_enabled,retraction_enable,retraction_amount";
    QString strPara_Part3 = ",retraction_retract_speed,retraction_prime_speed,retraction_extra_prime_amount,retraction_count_max,travel_retract_before_outer_wall";
    QString strPara_Part4 = ",retraction_combing,retraction_hop_enabled,retraction_hop_only_when_collides,retraction_hop,retraction_extrusion_window";
    QString strPara_Speed = strPara_Part1 + strPara_Part2 + strPara_Part3 +  strPara_Part4;
    QStringList list = strPara_Speed.split(",");

    for (auto val : list)
    {
        //float
        if ("speed_print" == val || "speed_infill" == val ||"speed_prime_tower" == val || "retraction_hop" == val)
        {
            value.insert(std::make_pair(val, QString("%1").arg(getValue(val).toDouble())));
        }
        else
        {
            //if ("retraction_hop_enabled" == val)
            //{
            //    if (getValue(val) == "false")
            //    {
            //        value["retraction_combing"] = "infill";
            //    }
            //}

            value.insert(std::make_pair(val, getValue(val)));
        }
    }

}

void Editprofile::getProfile_AdvancedSetting_SpecialMode(std::map<QString, QString> &value)
{
    value.clear();

    QString strPara_Part1 = "meshfix_union_all,meshfix_union_all_remove_holes,meshfix_extensive_stitching,meshfix_keep_open_polygons,multiple_mesh_overlap";
    QString strPara_Part2 = ",carve_multiple_volumes,alternate_carve_order,remove_empty_first_layers,meshfix_maximum_resolution,meshfix_maximum_travel_resolution";
    QString strPara_Part3 = ",meshfix_maximum_deviation,print_sequence,mold_enabled,mold_roof_height,mold_angle,magic_mesh_surface_mode,magic_spiralize";
    QString strPara_Part4 = ",conical_overhang_enabled,conical_overhang_angle,coasting_enable,coasting_volume,coasting_speed,smooth_spiralized_contours";
    QString strPara_SpecialMode = strPara_Part1 + strPara_Part2 + strPara_Part3 + strPara_Part4;
    QStringList list = strPara_SpecialMode.split(",");

    for (auto val : list)
    {
        //float
        if ("multiple_mesh_overlap" == val || "meshfix_maximum_resolution" == val ||"meshfix_maximum_travel_resolution" == val ||"meshfix_maximum_deviation" == val
                 ||"mold_roof_height" == val || "coasting_volume" == val )
        {
            value.insert(std::make_pair(val, QString("%1").arg(getValue(val).toDouble())));
        }
        else
        {
            value.insert(std::make_pair(val, getValue(val)));
        }
    }

}

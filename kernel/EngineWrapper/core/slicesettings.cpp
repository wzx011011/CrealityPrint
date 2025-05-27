#include "slicesettings.h"

namespace cxsw
{
	SliceSettings::SliceSettings()
	{
	}

	SliceSettings::~SliceSettings()
	{
	}

	std::pair<std::string, std::string> SliceSettings::export_prime_tower_enable()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_prime_tower_size()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_prime_tower_min_volume()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_prime_tower_position_x()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_prime_tower_position_y()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_infill_extruder_nr()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_infill_sparse_density()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_infill_pattern()
	{
		// grid, lines, triangles, trihexagon, cubic, cubicsubdiv, tetrahedral,
		//quarter_cubic, concentric, zigzag, cross, cross_3d, gyroid
		std::string value;
		switch (m_SliceInfill.infill_pattern)
		{
		case 0:
		{
			value = "grid";
			break;
		}
		case 1:
		{
			value = "lines";
			break;
		}
		case 2:
		{
			value = "triangles";
			break;
		}
		case 3:
		{
			value = "trihexagon";
			break;
		}
		case 4:
		{
			value = "cubic";
			break;
		}
		case 5:
		{
			value = "cubicsubdiv";
			break;
		}
		case 6:
		{
			value = "tetrahedral";
			break;
		}
		case 7:
		{
			value = "quarter_cubic";
			break;
		}
		case 8:
		{
			value = "concentric";
			break;
		}
		case 9:
		{
			value = "zigzag";
			break;
		}
		case 10:
		{
			value = "cross";
			break;
		}
		case 11:
		{
			value = "cross_3d";
			break;
		}
		case 12:
		{
			value = "gyroid";
			break;
		}
		default:
			value = "grid";
		}
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_infill_sparse_thickness()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_meshfix_union_all()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_meshfix_union_all_remove_holes()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_meshfix_extensive_stitching()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_meshfix_keep_open_polygons()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_meshfix_maximum_resolution()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_meshfix_maximum_travel_resolution()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_meshfix_maximum_deviation()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_multiple_mesh_overlap()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_carve_multiple_volumes()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_alternate_carve_order()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_remove_empty_first_layers()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_adhesion_type()
	{
		//skirt, brim, raft, none
		std::string value;
		switch (m_SlicePlatform.adhesion_type)
		{
		case 0:
		{
			value = "skirt";
			break;
		}
		case 1:
		{
			value = "brim";
			break;
		}
		case 2:
		{
			value = "raft";
			break;
		}
		case 3:
		{
			value = "none";
			break;
		}
		default:
			value = "skirt";
		}
		return std::pair<std::string, std::string>("adhesion_type", value);
	}

	std::pair<std::string, std::string> SliceSettings::export_adhesion_extruder_nr()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_skirt_line_count()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_skirt_gap()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_brim_line_count()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_raft_margin()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_raft_surface_thickness()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_raft_interface_thickness()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_raft_base_thickness()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_layer_height()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_layer_height_0()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_line_width()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_infill_line_width()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_support_line_width()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_initial_layer_line_width_factor()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_wall_extruder_nr()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_wall_line_count()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_top_bottom_thickness()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_top_layers()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_bottom_layers()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_top_bottom_pattern()
	{
		//TOP_BOTTOM_lines, TOP_BOTTOM_concentric, TOP_BOTTOM_zigzag
		std::string value;
		switch (m_SliceResolution.top_bottom_pattern)
		{
		case 0:
		{
			value = "TOP_BOTTOM_lines";
			break;
		}
		case 1:
		{
			value = "TOP_BOTTOM_concentric";
			break;
		}
		case 2:
		{
			value = "TOP_BOTTOM_zigzag";
			break;
		}
		default:
			value = "TOP_BOTTOM_lines";
		}

		return std::pair<std::string, std::string>("top_bottom_pattern", value);
	}

	std::pair<std::string, std::string> SliceSettings::export_skin_angles()
	{
		return std::pair<std::string, std::string>("skin_angles", m_SliceResolution.skin_angles);
	}

	std::pair<std::string, std::string> SliceSettings::export_outer_inset_first()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_alternate_extra_perimeter()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_fill_outline_gaps()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_xy_offset_layer_0()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_z_seam_type()
	{
		//back, shortest, random, sharpest_corner 
		std::string value;
		switch (m_SliceResolution.z_seam_type)
		{
		case 0:
		{
			value = "back";
			break;
		}
		case 1:
		{
			value = "shortest";
			break;
		}
		case 2:
		{
			value = "random";
			break;
		}
		case 3:
		{
			value = "sharpest_corner";
			break;
		}
		default:
			value = "sharpest_corner";
		}

		return std::pair<std::string, std::string>("z_seam_type", value);
	}

	std::pair<std::string, std::string> SliceSettings::export_z_seam_x()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_z_seam_y()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_z_seam_corner()
	{
		//z_seam_corner_none, z_seam_corner_inner, z_seam_corner_outer, z_seam_corner_any, z_seam_corner_weighted
		std::string value;
		switch (m_SliceResolution.z_seam_corner)
		{
		case 0:
		{
			value = "z_seam_corner_none";
			break;
		}
		case 1:
		{
			value = "z_seam_corner_inner";
			break;
		}
		case 2:
		{
			value = "z_seam_corner_outer";
			break;
		}
		case 3:
		{
			value = "z_seam_corner_any";
			break;
		}
		case 4:
		{
			value = "z_seam_corner_weighted";
			break;
		}
		default:
			value = "z_seam_corner_inner";
		}
		return std::pair<std::string, std::string>("z_seam_corner", value);
	}

	std::pair<std::string, std::string> SliceSettings::export_ironing_enabled()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_ironing_pattern()
	{
		//IRONING_concentric, IRONING_zigzag
		std::string value;
		switch (m_SliceResolution.ironing_pattern)
		{
		case 0:
		{
			value = "IRONING_concentric";
			break;
		}
		case 1:
		{
			value = "IRONING_zigzag";
			break;
		}
		default:
			value = "IRONING_concentric";
		}
		return std::pair<std::string, std::string>("ironing_pattern", value);
	}

	std::pair<std::string, std::string> SliceSettings::export_cool_fan_enabled()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_print_sequence()
	{
		//all_at_once, one_at_a_time
		std::string value;
		switch (m_SliceSpecialFunction.print_sequence)
		{
		case 0:
		{
			value = "all_at_once";
			break;
		}
		case 1:
		{
			value = "one_at_a_time";
			break;
		}
		default:
			value = "all_at_once";
		}
		return std::pair<std::string, std::string>("print_sequence", value);
	}

	std::pair<std::string, std::string> SliceSettings::export_magic_mesh_surface_mode()
	{
		//normal, surface, both
		std::string value;
		switch (m_SliceSpecialFunction.magic_mesh_surface_mode)
		{
		case 0:
		{
			value = "normal";
			break;
		}
		case 1:
		{
			value = "surface";
			break;
		}
		case 2:
		{
			value = "both";
			break;
		}
		default:
			value = "normal";
		}
		return std::pair<std::string, std::string>("magic_mesh_surface_mode", value);
	}

	std::pair<std::string, std::string> SliceSettings::export_magic_spiralize()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_conical_overhang_enabled()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_conical_overhang_angle()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_coasting_enable()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_coasting_volume()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_coasting_speed()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_support_enable()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_support_extruder_nr()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_support_type()
	{
		//buildplate, everywhere
		std::string value;
		switch (m_SliceSupport.support_type)
		{
		case 0:
		{
			value = "buildplate";
			break;
		}
		case 1:
		{
			value = "everywhere";
			break;
		}
		default:
			value = "everywhere";
		}
		return std::pair<std::string, std::string>("support_type", value);
	}

	std::pair<std::string, std::string> SliceSettings::export_support_angle()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_support_pattern()
	{
		//SUPPORT_lines, SUPPORT_grid, SUPPORT_triangles, SUPPORT_concentric, SUPPORT_zigzag, SUPPORT_cross, SUPPORT_gyroid
		std::string value;
		switch (m_SliceSupport.support_pattern)
		{
		case 0:
		{
			value = "SUPPORT_lines";
			break;
		}
		case 1:
		{
			value = "SUPPORT_grid";
			break;
		}
		case 2:
		{
			value = "SUPPORT_triangles";
			break;
		}
		case 3:
		{
			value = "SUPPORT_concentric";
			break;
		}
		case 4:
		{
			value = "SUPPORT_zigzag";
			break;
		}
		case 5:
		{
			value = "SUPPORT_cross";
			break;
		}
		case 6:
		{
			value = "SUPPORT_gyroid";
			break;
		}
		default:
			value = "SUPPORT_grid";
		}
		return std::pair<std::string, std::string>("support_pattern", value);
	}

	std::pair<std::string, std::string> SliceSettings::export_support_infill_sparse_thickness()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_minimum_support_area()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_support_roof_height()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_support_bottom_height()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_support_roof_pattern()
	{
		//ROOF_BOTTOM_lines, ROOF_BOTTOM_grid, ROOF_BOTTOM_triangles, ROOF_BOTTOM_concentric, ROOF_BOTTOM_zigzag
		std::string value;
		switch (m_SliceSupport.support_roof_pattern)
		{
		case 0:
		{
			value = "ROOF_BOTTOM_lines";
			break;
		}
		case 1:
		{
			value = "ROOF_BOTTOM_grid";
			break;
		}
		case 2:
		{
			value = "ROOF_BOTTOM_triangles";
			break;
		}
		case 3:
		{
			value = "ROOF_BOTTOM_concentric";
			break;
		}
		case 4:
		{
			value = "ROOF_BOTTOM_zigzag";
			break;
		}
		default:
			value = "ROOF_BOTTOM_zigzag";
		}
		return std::pair<std::string, std::string>("support_roof_pattern", value);
	}

	std::pair<std::string, std::string> SliceSettings::export_support_bottom_pattern()
	{
		//ROOF_BOTTOM_lines, ROOF_BOTTOM_grid, ROOF_BOTTOM_triangles, ROOF_BOTTOM_concentric, ROOF_BOTTOM_zigzag
		std::string value;
		switch (m_SliceSupport.support_bottom_pattern)
		{
		case 0:
		{
			value = "ROOF_BOTTOM_lines";
			break;
		}
		case 1:
		{
			value = "ROOF_BOTTOM_grid";
			break;
		}
		case 2:
		{
			value = "ROOF_BOTTOM_triangles";
			break;
		}
		case 3:
		{
			value = "ROOF_BOTTOM_concentric";
			break;
		}
		case 4:
		{
			value = "ROOF_BOTTOM_zigzag";
			break;
		}
		default:
			value = "ROOF_BOTTOM_zigzag";
		}
		return std::pair<std::string, std::string>("support_bottom_pattern", value);
	}

	std::pair<std::string, std::string> SliceSettings::export_support_use_towers()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_support_tower_diameter()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_speed_print()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_speed_infill()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_speed_wall()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_speed_support()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_speed_prime_tower()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_speed_travel()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_speed_layer_0()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_skirt_brim_speed()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_speed_equalize_flow_enabled()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_retraction_combing()
	{
		//off, all, noskin, infill
		std::string value;
		switch (m_SliceTravel.retraction_combing)
		{
		case 0:
		{
			value = "off";
			break;
		}
		case 1:
		{
			value = "all";
			break;
		}
		case 2:
		{
			value = "noskin";
			break;
		}
		case 3:
		{
			value = "infill";
			break;
		}
		default:
			value = "infill";
		}
		return std::pair<std::string, std::string>("retraction_combing", value);
	}

	std::pair<std::string, std::string> SliceSettings::export_travel_retract_before_outer_wall()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_travel_avoid_other_parts()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_retraction_enable()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_retraction_amount()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_retraction_speed()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_retraction_extra_prime_amount()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_retraction_hop_enabled()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_retraction_hop_only_when_collides()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> SliceSettings::export_retraction_hop()
	{
		return std::pair<std::string, std::string>();
	}

}




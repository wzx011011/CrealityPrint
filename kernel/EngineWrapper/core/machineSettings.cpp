#include "machineSettings.h"

namespace cxsw {

	machineSettings::machineSettings()
	{

	}

	machineSettings::~machineSettings()
	{
	}

	std::pair<std::string, std::string> machineSettings::export_machine_name()
	{
		return std::pair<std::string, std::string>("machine_name", machine_name);
	}

	std::pair<std::string, std::string> machineSettings::export_machine_width()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> machineSettings::export_machine_depth()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> machineSettings::export_machine_height()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> machineSettings::export_machine_center_is_zero()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> machineSettings::export_machine_extruder_count()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> machineSettings::export_machine_nozzle_size()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> machineSettings::export_machine_start_gcode()
	{
		return std::pair<std::string, std::string>("machine_start_gcode", machine_start_gcode);
	}

	std::pair<std::string, std::string> machineSettings::export_machine_end_gcode()
	{
		return std::pair<std::string, std::string>("machine_end_gcode", machine_end_gcode);
	}

	std::pair<std::string, std::string> machineSettings::export_machine_gcode_flavor()
	{
		// Marlin, MarlinVolumetric, RepRap, Makerbot, Ultimaker2, Griffin, BFB, MACH3, Repetier 
		std::string value;
		switch (machine_gcode_flavor)
		{
		case 0:
		{
			value = "Marlin";
			break;
		}
		case 1:
		{
			value = "MarlinVolumetric";
			break;
		}
		case 2:
		{
			value = "RepRap";
			break;
		}
		case 3:
		{
			value = "Makerbot";
			break;
		}
		case 4:
		{
			value = "Ultimaker2";
			break;
		}
		case 5:
		{
			value = "Griffin";
			break;
		}
		case 6:
		{
			value = "BFB";
			break;
		}
		case 7:
		{
			value = "MACH3";
			break;
		}
		case 8:
		{
			value = "Repetier";
			break;
		}
		default:
			value = "Marlin";
		}
		return std::pair<std::string, std::string>("machine_gcode_flavor", value);
	}

	std::pair<std::string, std::string> machineSettings::export_machine_head_with_fans_polygon()
	{
		return std::pair<std::string, std::string>("machine_head_with_fans_polygon", machine_head_with_fans_polygon);
	}
}




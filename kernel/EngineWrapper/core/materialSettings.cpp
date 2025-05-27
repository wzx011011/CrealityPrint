#include "materialSettings.h"

using namespace std;
namespace cxsw {

    materialSettings::materialSettings()
        : material_diameter(0.0)
        , material_type()
        , material_print_temperature(0)
        , material_bed_temperature(0)
        , material_bed_temperature_layer_0(0)
    {
    }

    materialSettings::~materialSettings()
    {
    }

	std::pair<std::string, std::string> materialSettings::export_material_diameter()
	{
		return std::pair<std::string, std::string>();
	}

    std::pair<std::string, std::string> materialSettings::export_material_type()
    {
        // PLA, ABS, CPE, GFFCPE, HIPS, PC, PETG, PP, PVA, TPU 
        std::string value;
        switch (material_type)
        {
        case 0:
        {
            value = "PLA";
            break;
        }
        case 1:
        {
            value = "ABS";
            break;
        }
        case 2:
        {
            value = "CPE";
            break;
        }
        case 3:
        {
            value = "GFFCPE";
            break;
        }
        case 4:
        {
            value = "HIPS";
            break;
        }
        case 5:
        {
            value = "PC";
            break;
        }
        case 6:
        {
            value = "PETG";
            break;
        }
        case 7:
        {
            value = "PP";
            break;
        }
        case 8:
        {
            value = "PVA";
            break;
        }
        case 9:
        {
            value = "TPU";
            break;
        }
        default:
            value = "PLA";
        }
        return std::pair<std::string, std::string>("material_type", value);
    }

	std::pair<std::string, std::string> materialSettings::export_material_print_temperature()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> materialSettings::export_material_bed_temperature()
	{
		return std::pair<std::string, std::string>();
	}

	std::pair<std::string, std::string> materialSettings::export_material_bed_temperature_layer_0()
	{
		return std::pair<std::string, std::string>();
	}
}



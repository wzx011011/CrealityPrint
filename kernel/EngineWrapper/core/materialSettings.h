#ifndef MATERIAL_SETTINGS_H
#define MATERIAL_SETTINGS_H
#include <map>
#include <string>


namespace cxsw {
    enum	MATERIAL_PYPE { PLA, ABS, CPE, GFFCPE, HIPS, PC, PETG, PP, PVA, TPU };
    class materialSettings
    {
    public:
        materialSettings();
        virtual ~materialSettings();
        std::pair<std::string, std::string> export_material_diameter();
        std::pair<std::string, std::string> export_material_type();
        std::pair<std::string, std::string> export_material_print_temperature();
        std::pair<std::string, std::string> export_material_bed_temperature();
        std::pair<std::string, std::string> export_material_bed_temperature_layer_0();

    public:
        float    material_diameter;
        MATERIAL_PYPE    material_type;
        int    material_print_temperature;
        int    material_bed_temperature;
        int    material_bed_temperature_layer_0;
    };
}


#endif //MATERIALSETTINGS_H


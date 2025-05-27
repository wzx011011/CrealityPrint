#ifndef MACHINE_SETTINGS_H
#define MACHINE_SETTINGS_H
#include <string>
#include <list>

namespace cxsw {

    enum   MACHINE_GCODE_FLAVOR { Marlin, MarlinVolumetric, RepRap, Makerbot, Ultimaker2, Griffin, BFB, MACH3, Repetier };	//	g浠ｇ爜鐨勭被鍨?enum
    class machineSettings
    {
    public:
        machineSettings();
        virtual ~machineSettings();

        std::pair<std::string, std::string> export_machine_name();
        std::pair<std::string, std::string> export_machine_width();
        std::pair<std::string, std::string> export_machine_depth();
        std::pair<std::string, std::string> export_machine_height();
        std::pair<std::string, std::string> export_machine_center_is_zero();
        std::pair<std::string, std::string> export_machine_extruder_count();
        std::pair<std::string, std::string> export_machine_nozzle_size();
        std::pair<std::string, std::string> export_machine_start_gcode();
        std::pair<std::string, std::string> export_machine_end_gcode();
        std::pair<std::string, std::string> export_machine_gcode_flavor();
        std::pair<std::string, std::string> export_machine_head_with_fans_polygon();


    public:
        std::string  machine_name;//机型名字string
        float   machine_width;//机型宽度float
        float   machine_depth;//机型深度float
        float   machine_height;//机型高度float
        bool    machine_center_is_zero;//机器中心是否在原点	bool
        int     machine_extruder_count;//挤出机的数量int
        float   machine_nozzle_size;//喷嘴孔径float
        std::string    machine_start_gcode;//开始的G代码string
        std::string    machine_end_gcode;//结束的G代码string
        MACHINE_GCODE_FLAVOR     machine_gcode_flavor;//g代码的类型enum
        std::string    machine_head_with_fans_polygon;//机器喷嘴部件大小string
    };
}




#endif // MACHINE_SETTINGS_H
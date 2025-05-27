#ifndef _US_MATERIALSETTING_1589252221535_H
#define _US_MATERIALSETTING_1589252221535_H
#include "enginewrapperexport.h"
#include <string>

namespace cxsw
{
    class SliceResult;
}


typedef struct StuSliceResult
{
    unsigned long int print_time; // 预估打印耗时，单位：秒
    double filament_len; // 预估材料消耗，单位：米
    unsigned long int layer_count;  // 切片层数
    double x;   // 切片x尺寸
    double y;   // 切片y尺寸
    double z;   // 切片z尺寸
    //std::string strgcode;
} stu_slice_result;

//设置参数
enum MODIFYTYPE
{
    GlobalSetting,
    ExtruderSetting,
    ModelListSetting,
    ModelSetting
};

//是否带裙边
enum ADHESIONTYPE
{
    Skirt,
    Brim,
    Raft,
    None
};

//支撑范围
enum SUPPORTTYPE
{
    Everywhere,
    Buildplate
};

//支撑类型
enum SUPPORTPATTERN
{
    Lines,
    Grid,
    Triangles,
    Concentric,
    ZigZag,
    Cross,
    Gyroid
};

//打印平台类型
enum MACHINESHAPE
{
    Rectangular,
    Elliptic
};

//填充图案
enum INFILLPATTERN
{
    INFILL_Grid,
    INFILL_Lines,
    INFILL_Triangles,
    INFILL_TriHexagon,
    INFILL_Cubic,
    INFILL_CubicSubdivisio,
    INFILL_Octet,
    INFILL_QuarterCubic,
    INFILL_Concentric,
    INFILL_ZigZag,
    INFILL_Cross,
    INFILL_Cross3D,
    INFILL_Gyroid
};

/*
 * 功能:初始化任务
 * machine_name:机器型号 example: Ender3
 *
 * Return: job_id: 保存任务标识
 */
ENGINE_WRAPPER_API int prepare_slice_job();

/*
 * 功能:添加模型到任务
 * job_id:任务标识
 * stl:模型文件路径
 * matrixs: float[16] 数组 4x4的矩阵数据
 *
 * RETURN:
 */
ENGINE_WRAPPER_API bool add_mesh(int job_id, const std::string& stl_path, float matrixs[16]);

/*
 * 功能:初始化任务
 * job_id:任务标识
  */
ENGINE_WRAPPER_API void slice(stu_slice_result& _stu_slice_result, int job_id,
                              const std::string& gcode_path, const std::string& engine_path, const std::string & engine_output_pat,
                              const std::string& conversion_stl);

/*
 * 功能:加载配置文件
 * fileName：文件名称
 * Return：参数是否设置成功
 */
ENGINE_WRAPPER_API bool load_settings_file(int job_id, const std::string& file_name);

/*
 * 功能:gcode转STL
 * fileName：保存STL文件的名字
 * m_result：切片数据
 * Return：参数是否设置成功
 */
ENGINE_WRAPPER_API void Gcode2Stl(cxsw::SliceResult* result, const std::string& file_name);

/*
 * 功能:gcode转STL
 * stl_path：保存STL文件
 * gcode_path：gcode数据
 * bool：保存stl 是否成功
 */
ENGINE_WRAPPER_API bool Gcode2Stl(const std::string& stl_path, const std::string& gcode_path);


/*
 * 功能:修改设置参数值
 * modify_type：修改的类型   枚举类型  GlobalSetting,ExtruderSetting,ModelListSetting, ModelSetting
 * key:参数名
 * value：参数值
 * mesh_id：修改的模型编号，默认缺省，需要修改单独的模型参数时给定
 * Return：是否设置成功
 */
ENGINE_WRAPPER_API bool set_value(int job_id, int modify_type, const std::string& key, const std::string& value, int mesh_id = 0);

ENGINE_WRAPPER_API std::string get_value(int job_id,const std::string& key, int mesh_id = 0);

/*
 * 功能:释放任务资源
 * job_id:任务标识
 * 返回: gcode string
 */
ENGINE_WRAPPER_API void destroy(int job_id);

#endif

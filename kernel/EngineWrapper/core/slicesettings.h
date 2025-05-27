#ifndef SLICE_SETTINGS_H
#define SLICE_SETTINGS_H
#include <string>

namespace cxsw {

    enum	INFILL_PATTERN {
        grid, lines, triangles, trihexagon, cubic, cubicsubdiv, tetrahedral,
        quarter_cubic, concentric, zigzag, cross, cross_3d, gyroid
    };
    struct SliceDual {
        bool	prime_tower_enable;	//	启用装填塔	False	
        float	prime_tower_size;	//	装填塔尺寸	20	mm
        float	prime_tower_min_volume;	//	装填塔最小体积	6	mm3
        float	prime_tower_position_x;	//	装填塔 X 位置	294.3	mm
        float	prime_tower_position_y;	//	装填塔 Y 位置	274.3	mm
    };

    struct SliceInfill {
        int		infill_extruder_nr;	//填充挤出机
        float	infill_sparse_density;//填充密度%
        INFILL_PATTERN	infill_pattern;//填充图案
        float	infill_sparse_thickness;//填充层厚度	mm
    };

    struct SliceMeshfix {
        bool	meshfix_union_all;	//	联合覆盖体积	True	
        bool	meshfix_union_all_remove_holes;	//	移除所有孔洞	False	
        bool	meshfix_extensive_stitching;	//	广泛缝合	False	
        bool	meshfix_keep_open_polygons;	//	保留断开连接的面	False	
        float	meshfix_maximum_resolution;	//	最大分辨率	0.05	mm
        float	meshfix_maximum_travel_resolution;	//	空走的最大分辨率	0.05	mm
        float	meshfix_maximum_deviation;	//	最大偏移量	0.05	mm
        float	multiple_mesh_overlap;	//	合并网格重叠	multiple_mesh_overlap	mm
        bool	carve_multiple_volumes;	//	移除网格交叉	False	
        bool	alternate_carve_order;	//	交替网格移除	True	
        bool	remove_empty_first_layers;	//	移除空白第一层	True	
    };
    enum ADHESION_TYPE { skirt, brim, raft, none };
    struct SlicePlatform {
        ADHESION_TYPE adhesion_type; 	//	打印平台附着类型	skirt	
        int		adhesion_extruder_nr;	//	打印平台附着挤出机	0	
        int		skirt_line_count;	//	裙摆 走线计数	1	
        float	skirt_gap;	//	裙摆 距离	3.0	mm
        int		brim_line_count;	//	边缘 走线计数	5	
        float	raft_margin;	//	底座 留白	5	mm
        float	raft_surface_thickness;	//	底座 顶层厚度	0.5	mm
        float	raft_interface_thickness;	//	底座 中间厚度	0.5	mm
        float	raft_base_thickness;	//	底座 基础厚度	0.384	mm
    };

    enum 		TOP_BOTTOM_PATTERN { TOP_BOTTOM_lines, TOP_BOTTOM_concentric, TOP_BOTTOM_zigzag };	//	顶部 / 底部走线图案	
    enum		Z_SEAM_TYPE { back, shortest, random, sharpest_corner }; //Z 缝对齐 sharpest_corner	
    enum Z_SEAM_CORNER { z_seam_corner_none, z_seam_corner_inner, z_seam_corner_outer, z_seam_corner_any, z_seam_corner_weighted };
    enum		IRONING_PATTERN { IRONING_concentric, IRONING_zigzag };	//	熨平图案	zigzag	
    struct SliceResolution {
        float		layer_height;	//	层高	0.2	mm
        float		layer_height_0;	//	起始层高层高	0.2	mm
        float		line_width;	//	走线宽度	0.4	mm
        float		infill_line_width;	//	走线宽度（填充）	0.4	mm
        float		support_line_width;	//	走线宽度（支撑结构）	0.4	mm
        float		initial_layer_line_width_factor;	//	起始层走线宽度	100.0	%
        int			wall_extruder_nr;	//	壁挤出机	0	
        int			wall_line_count;	//	壁走线次数	2	
        float		top_bottom_thickness;	//	顶层 / 底层厚度	1.82	mm
        int			top_layers;	//	顶部层数	4	
        int			bottom_layers;	//	底部层数	4	
        TOP_BOTTOM_PATTERN 		top_bottom_pattern;	//	顶部 / 底部走线图案	
        std::string		skin_angles;	//	顶层/底层走线方向	[ ]	
        bool		outer_inset_first;	//	先外壁后内壁	False	
        bool		alternate_extra_perimeter;	//	交替备用壁	False	
        bool		fill_outline_gaps;	//	打印薄壁	False	
        float		xy_offset_layer_0;	//	起始层水平扩展	0	mm
        Z_SEAM_TYPE		z_seam_type; //Z 缝对齐 sharpest_corner	
        float		z_seam_x;	//	Z 缝 X	100	mm
        float		z_seam_y;	//	Z 缝 Y	100	mm
        Z_SEAM_CORNER z_seam_corner;	//	缝隙角偏好设置	z_seam_corner_inner	
        bool		ironing_enabled;	//	启用熨平	False	
        IRONING_PATTERN		ironing_pattern;	//	熨平图案	zigzag	
        bool		cool_fan_enabled;	//	开启打印冷却	True	
    };

    enum PRINT_SEQUENCE { all_at_once, one_at_a_time };
    enum MAGIC_MESH_SURFACE_MODE { normal, surface, both };
    struct SliceSpecialFunction {
        PRINT_SEQUENCE print_sequence; 	//	打印序列	all_at_once	
        MAGIC_MESH_SURFACE_MODE	magic_mesh_surface_mode;//	表面模式	normal	
        bool	magic_spiralize;	//	螺旋打印外轮廓	False	
        bool	conical_overhang_enabled;	//	使悬垂可打印	False	
        float	conical_overhang_angle;	//	最大模型角度	50	°
        bool	coasting_enable;	//	启用滑行	False	
        float	coasting_volume;	//	滑行体积	0.128	mm3
        float	coasting_speed;	//	滑行速度	90	%
    };

    enum	SUPPORT_TYPE { buildplate, everywhere };
    enum	SUPPORT_PATTERN { SUPPORT_lines, SUPPORT_grid, SUPPORT_triangles, SUPPORT_concentric, SUPPORT_zigzag, SUPPORT_cross, SUPPORT_gyroid };
    enum	ROOF_BOTTOM_PATTERN { ROOF_BOTTOM_lines, ROOF_BOTTOM_grid, ROOF_BOTTOM_triangles, ROOF_BOTTOM_concentric, ROOF_BOTTOM_zigzag };
    struct SliceSupport {
        bool	support_enable;	//	生成支撑	False	
        int		support_extruder_nr;	//	支撑用挤出机	0	
        SUPPORT_TYPE	support_type;	//	支撑放置	buildplate	
        float	support_angle;	//	支撑悬垂角度	60	°
        SUPPORT_PATTERN	support_pattern; //支撑图案 zigzag	
        float	support_infill_sparse_thickness;	//	支撑填充层厚度	0.5	mm
        float	minimum_support_area;	//	最小支撑面积	5	mm²
        float	support_roof_height;	//	支撑顶板厚度	2.0	mm
        float	support_bottom_height;	//	支撑底板厚度	2.0	mm
        ROOF_BOTTOM_PATTERN	support_roof_pattern;	//	支撑顶板图案	grid	
        ROOF_BOTTOM_PATTERN	support_bottom_pattern;	//	支撑底板图案	grid	
        bool	support_use_towers;	//	使用塔	False	
        float	support_tower_diameter;	//	塔直径	3.0	mm
    };

    enum	RETRACTION_COMBING { off, all, noskin, infill };
    struct SliceTravel {
        float	speed_print;	//	打印速度	50.0	mm/s
        float	speed_infill;	//	填充速度	50	mm/s
        float	speed_wall;	//	速度（壁）	25.0	mm/s
        float	speed_support;	//	速度（支撑结构）	25.0	mm/s
        float	speed_prime_tower;	//	装填塔速度	25.0	mm/s
        float	speed_travel;	//	空驶速度	120.0	mm/s
        float	speed_layer_0;	//	起始层速度	20.0	mm/s
        float	skirt_brim_speed;	//	裙摆/Brim 速度	20.0	mm/s
        bool	speed_equalize_flow_enabled;//	均衡耗材流量	False	
        RETRACTION_COMBING	retraction_combing;	//	梳理模式	infill	
        bool	travel_retract_before_outer_wall;	//	在外壁前回抽	True	
        bool	travel_avoid_other_parts;	//	空驶时避开已打印部分	True	
        bool	retraction_enable;	//	启用回抽	True	
        float	retraction_amount;	//	回抽距离	5.0	mm
        float	retraction_speed;	//	回抽速度	45	mm/s
        float	retraction_extra_prime_amount;	//	回抽额外装填量	0	mm3
        bool	retraction_hop_enabled;	//	回抽时 Z 抬升	False	
        bool	retraction_hop_only_when_collides;	//	仅在已打印部分上 Z 抬升	False	
        float	retraction_hop;	//	Z 抬升高度	0.2	mm
    };


    class SliceSettings
    {
    public:
        SliceSettings();
        virtual ~SliceSettings();
        //SliceDual
        std::pair<std::string, std::string> export_prime_tower_enable();
        std::pair<std::string, std::string> export_prime_tower_size();
        std::pair<std::string, std::string> export_prime_tower_min_volume();
        std::pair<std::string, std::string> export_prime_tower_position_x();
        std::pair<std::string, std::string> export_prime_tower_position_y();
        //SliceInfill
        std::pair<std::string, std::string> export_infill_extruder_nr();
        std::pair<std::string, std::string> export_infill_sparse_density();
        std::pair<std::string, std::string> export_infill_pattern();
        std::pair<std::string, std::string> export_infill_sparse_thickness();
        //SliceMeshfix
        std::pair<std::string, std::string> export_meshfix_union_all();
        std::pair<std::string, std::string> export_meshfix_union_all_remove_holes();
        std::pair<std::string, std::string> export_meshfix_extensive_stitching();
        std::pair<std::string, std::string> export_meshfix_keep_open_polygons();
        std::pair<std::string, std::string> export_meshfix_maximum_resolution();
        std::pair<std::string, std::string> export_meshfix_maximum_travel_resolution();
        std::pair<std::string, std::string> export_meshfix_maximum_deviation();
        std::pair<std::string, std::string> export_multiple_mesh_overlap();
        std::pair<std::string, std::string> export_carve_multiple_volumes();
        std::pair<std::string, std::string> export_alternate_carve_order();
        std::pair<std::string, std::string> export_remove_empty_first_layers();
        //SlicePlatform
        std::pair<std::string, std::string> export_adhesion_type();
        std::pair<std::string, std::string> export_adhesion_extruder_nr();
        std::pair<std::string, std::string> export_skirt_line_count();
        std::pair<std::string, std::string> export_skirt_gap();
        std::pair<std::string, std::string> export_brim_line_count();
        std::pair<std::string, std::string> export_raft_margin();
        std::pair<std::string, std::string> export_raft_surface_thickness();
        std::pair<std::string, std::string> export_raft_interface_thickness();
        std::pair<std::string, std::string> export_raft_base_thickness();
        //SliceResolution
        std::pair<std::string, std::string> export_layer_height();
        std::pair<std::string, std::string> export_layer_height_0();
        std::pair<std::string, std::string> export_line_width();
        std::pair<std::string, std::string> export_infill_line_width();
        std::pair<std::string, std::string> export_support_line_width();
        std::pair<std::string, std::string> export_initial_layer_line_width_factor();
        std::pair<std::string, std::string> export_wall_extruder_nr();
        std::pair<std::string, std::string> export_wall_line_count();
        std::pair<std::string, std::string> export_top_bottom_thickness();
        std::pair<std::string, std::string> export_top_layers();
        std::pair<std::string, std::string> export_bottom_layers();
        std::pair<std::string, std::string> export_top_bottom_pattern();
        std::pair<std::string, std::string> export_skin_angles();
        std::pair<std::string, std::string> export_outer_inset_first();
        std::pair<std::string, std::string> export_alternate_extra_perimeter();
        std::pair<std::string, std::string> export_fill_outline_gaps();
        std::pair<std::string, std::string> export_xy_offset_layer_0();
        std::pair<std::string, std::string> export_z_seam_type();
        std::pair<std::string, std::string> export_z_seam_x();
        std::pair<std::string, std::string> export_z_seam_y();
        std::pair<std::string, std::string> export_z_seam_corner();
        std::pair<std::string, std::string> export_ironing_enabled();
        std::pair<std::string, std::string> export_ironing_pattern();
        std::pair<std::string, std::string> export_cool_fan_enabled();
        //SliceSpecialFunction
        std::pair<std::string, std::string> export_print_sequence();
        std::pair<std::string, std::string> export_magic_mesh_surface_mode();
        std::pair<std::string, std::string> export_magic_spiralize();
        std::pair<std::string, std::string> export_conical_overhang_enabled();
        std::pair<std::string, std::string> export_conical_overhang_angle();
        std::pair<std::string, std::string> export_coasting_enable();
        std::pair<std::string, std::string> export_coasting_volume();
        std::pair<std::string, std::string> export_coasting_speed();
        //SliceSupport
        std::pair<std::string, std::string> export_support_enable();
        std::pair<std::string, std::string> export_support_extruder_nr();
        std::pair<std::string, std::string> export_support_type();
        std::pair<std::string, std::string> export_support_angle();
        std::pair<std::string, std::string> export_support_pattern();
        std::pair<std::string, std::string> export_support_infill_sparse_thickness();
        std::pair<std::string, std::string> export_minimum_support_area();
        std::pair<std::string, std::string> export_support_roof_height();
        std::pair<std::string, std::string> export_support_bottom_height();
        std::pair<std::string, std::string> export_support_roof_pattern();
        std::pair<std::string, std::string> export_support_bottom_pattern();
        std::pair<std::string, std::string> export_support_use_towers();
        std::pair<std::string, std::string> export_support_tower_diameter();
        //SliceTravel
        std::pair<std::string, std::string> export_speed_print();
        std::pair<std::string, std::string> export_speed_infill();
        std::pair<std::string, std::string> export_speed_wall();
        std::pair<std::string, std::string> export_speed_support();
        std::pair<std::string, std::string> export_speed_prime_tower();
        std::pair<std::string, std::string> export_speed_travel();
        std::pair<std::string, std::string> export_speed_layer_0();
        std::pair<std::string, std::string> export_skirt_brim_speed();
        std::pair<std::string, std::string> export_speed_equalize_flow_enabled();
        std::pair<std::string, std::string> export_retraction_combing();
        std::pair<std::string, std::string> export_travel_retract_before_outer_wall();
        std::pair<std::string, std::string> export_travel_avoid_other_parts();
        std::pair<std::string, std::string> export_retraction_enable();
        std::pair<std::string, std::string> export_retraction_amount();
        std::pair<std::string, std::string> export_retraction_speed();
        std::pair<std::string, std::string> export_retraction_extra_prime_amount();
        std::pair<std::string, std::string> export_retraction_hop_enabled();
        std::pair<std::string, std::string> export_retraction_hop_only_when_collides();
        std::pair<std::string, std::string> export_retraction_hop();

    public:
        SliceDual m_SliceDual;
        SliceInfill m_SliceInfill;
        SliceMeshfix m_SliceMeshfix;
        SlicePlatform m_SlicePlatform;
        SliceResolution m_SliceResolution;
        SliceSpecialFunction m_SliceSpecialFunction;
        SliceSupport m_SliceSupport;
        SliceTravel m_SliceTravel;
    };
}

#endif // SLICE_SETTINGS_H
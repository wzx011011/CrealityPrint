#include "gcoderendergenerator.h"
#include "libslic3r/libslic3r.h"
#include "libslic3r/Print.hpp"

#include <array>
#include <algorithm>
#include <chrono>

namespace Slic3r {
namespace GUI {

    static ColorRGBA decode_color_to_float_array(const std::string color)
    {
        ColorRGBA ret = ColorRGBA::BLACK();
        decode_color(color, ret);
        return ret;
    }

    static std::vector<ColorRGBA> Extrusion_Role_Colors{ {
        { 0.90f, 0.70f, 0.70f, 1.0f },   // erNone
        { 1.00f, 0.90f, 0.30f, 1.0f },   // erPerimeter
        { 1.00f, 0.49f, 0.22f, 1.0f },   // erExternalPerimeter
        { 0.12f, 0.12f, 1.00f, 1.0f },   // erOverhangPerimeter
        { 0.69f, 0.19f, 0.16f, 1.0f },   // erInternalInfill
        { 0.59f, 0.33f, 0.80f, 1.0f },   // erSolidInfill
        { 0.94f, 0.25f, 0.25f, 1.0f },   // erTopSolidInfill
        { 0.40f, 0.36f, 0.78f, 1.0f },   // erBottomSurface
        { 1.00f, 0.55f, 0.41f, 1.0f },   // erIroning
        { 0.30f, 0.40f, 0.63f, 1.0f },   // erBridgeInfill
        { 0.30f, 0.50f, 0.73f, 1.0f },   // erInternalBridgeInfill
        { 1.00f, 1.00f, 1.00f, 1.0f },   // erGapFill
        { 0.00f, 0.53f, 0.43f, 1.0f },   // erSkirt
        { 0.00f, 0.23f, 0.43f, 1.0f },   // erBrim
        { 0.00f, 1.00f, 0.00f, 1.0f },   // erSupportMaterial
        { 0.00f, 0.50f, 0.00f, 1.0f },   // erSupportMaterialInterface
        { 0.00f, 0.25f, 0.00f, 1.0f },   // erSupportTransition
        { 0.70f, 0.89f, 0.67f, 1.0f },   // erWipeTower
        { 0.37f, 0.82f, 0.58f, 1.0f }    // erCustom
    } };

    static std::vector<ColorRGBA> Options_Colors{ {
        { 0.803f, 0.135f, 0.839f, 1.0f },   // Retractions
        { 0.287f, 0.679f, 0.810f, 1.0f },   // Unretractions
        { 0.900f, 0.900f, 0.900f, 1.0f },   // Seams
        { 0.758f, 0.744f, 0.389f, 1.0f },   // ToolChanges
        { 0.856f, 0.582f, 0.546f, 1.0f },   // ColorChanges
        { 0.322f, 0.942f, 0.512f, 1.0f },   // PausePrints
        { 0.886f, 0.825f, 0.262f, 1.0f }    // CustomGCodes
    } };

    static std::vector<ColorRGBA> Travel_Colors{ {
        { 0.219f, 0.282f, 0.609f, 1.0f }, // Move
        { 0.112f, 0.422f, 0.103f, 1.0f }, // Extrude
        { 0.505f, 0.064f, 0.028f, 1.0f }  // Retract
    } };

    static std::vector<ColorRGBA> Range_Colors{ {
        decode_color_to_float_array("#0b2c7a"),  // bluish
        decode_color_to_float_array("#135985"),
        decode_color_to_float_array("#1c8891"),
        decode_color_to_float_array("#04d60f"),
        decode_color_to_float_array("#aaf200"),
        decode_color_to_float_array("#fcf903"),
        decode_color_to_float_array("#f5ce0a"),
        //decode_color_to_float_array("#e38820"),
        decode_color_to_float_array("#d16830"),
        decode_color_to_float_array("#c2523c"),
        decode_color_to_float_array("#942616")    // reddish
    } };

    static ColorRGBA Wipe_Color = ColorRGBA::YELLOW();
    static ColorRGBA Neutral_Color = ColorRGBA::DARK_GRAY();


static unsigned char buffer_id(EMoveType type) {
    return static_cast<unsigned char>(type) - static_cast<unsigned char>(EMoveType::Retract);
}

static EMoveType buffer_type(unsigned char id) {
    return static_cast<EMoveType>(static_cast<unsigned char>(EMoveType::Retract) + id);
}

// Round to a bin with minimum two digits resolution.
// Equivalent to conversion to string with sprintf(buf, "%.2g", value) and conversion back to float, but faster.
static float round_to_bin(const float value)
{
//    assert(value > 0);
    constexpr float const scale    [5] = { 100.f,  1000.f,  10000.f,  100000.f,  1000000.f };
    constexpr float const invscale [5] = { 0.01f,  0.001f,  0.0001f,  0.00001f,  0.000001f };
    constexpr float const threshold[5] = { 0.095f, 0.0095f, 0.00095f, 0.000095f, 0.0000095f };
    // Scaling factor, pointer to the tables above.
    int                   i            = 0;
    // While the scaling factor is not yet large enough to get two integer digits after scaling and rounding:
    for (; value < threshold[i] && i < 4; ++ i) ;
    return std::round(value * scale[i]) * invscale[i];
}

void GCodeRenderGenerator::InstanceVBuffer::Ranges::reset()
{
    ranges.clear();
}

void GCodeRenderGenerator::InstanceVBuffer::reset()
{
    s_ids.clear();
    s_ids.shrink_to_fit();
    buffer.clear();
    buffer.shrink_to_fit();
    render_ranges.reset();
}

bool GCodeRenderGenerator::Path::matches(const GCodeProcessorResult::MoveVertex& move) const
{
    auto matches_percent = [](float value1, float value2, float max_percent) {
        return std::abs(value2 - value1) / value1 <= max_percent;
    };

    switch (move.type)
    {
    case EMoveType::Tool_change:
    case EMoveType::Color_change:
    case EMoveType::Pause_Print:
    case EMoveType::Custom_GCode:
    case EMoveType::Retract:
    case EMoveType::Unretract:
    case EMoveType::Seam:
    case EMoveType::Extrude: {
        // use rounding to reduce the number of generated paths
        return type == move.type && extruder_id == move.extruder_id && cp_color_id == move.cp_color_id && role == move.extrusion_role &&
            move.position.z() <= sub_paths.front().first.position.z() && feedrate == move.feedrate && fan_speed == move.fan_speed &&
            height == round_to_bin(move.height) && width == round_to_bin(move.width) &&
            matches_percent(volumetric_rate, move.volumetric_rate(), 0.05f) && layer_time == move.layer_duration;
    }
    case EMoveType::Travel: {
        return type == move.type && feedrate == move.feedrate && extruder_id == move.extruder_id && cp_color_id == move.cp_color_id;
    }
    default: { return false; }
    }
}

void GCodeRenderGenerator::TBuffer::Model::reset()
{
    instances.reset();
}

void GCodeRenderGenerator::TBuffer::reset()
{
    indices.clear();
    paths.clear();
    render_paths.clear();
    model.reset();
}

void GCodeRenderGenerator::TBuffer::add_path(const GCodeProcessorResult::MoveVertex& move, unsigned int b_id, size_t i_id, size_t s_id)
{
    Path::Endpoint endpoint = { b_id, i_id, s_id, move.position };
    // use rounding to reduce the number of generated paths
    paths.push_back({ move.type, move.extrusion_role, move.delta_extruder,
        round_to_bin(move.height), round_to_bin(move.width),
        move.feedrate, move.fan_speed, move.temperature,
        move.volumetric_rate(), move.layer_duration, move.extruder_id, move.cp_color_id, { { endpoint, endpoint } } });
}

unsigned int GCodeRenderGenerator::TBuffer::max_vertices_per_segment() const
{
    switch (render_primitive_type)
    {
    case ERenderPrimitiveType::Line: { return 2; }
    case ERenderPrimitiveType::Triangle: { return 8; }
    default: { return 0; }
    }
}

size_t GCodeRenderGenerator::TBuffer::max_vertices_per_segment_size_floats() const
{
    return vertices.vertex_size_floats() * static_cast<size_t>(max_vertices_per_segment());
}

size_t GCodeRenderGenerator::TBuffer::max_vertices_per_segment_size_bytes() const
{
    return max_vertices_per_segment_size_floats() * sizeof(float);
}

unsigned int GCodeRenderGenerator::TBuffer::indices_per_segment() const
{
    switch (render_primitive_type)
    {
    case ERenderPrimitiveType::Line: { return 2; }
    case ERenderPrimitiveType::Triangle: { return 30; } // 3 indices x 10 triangles
    default: { return 0; }
    }
}

size_t GCodeRenderGenerator::TBuffer::indices_per_segment_size_bytes() const
{
    return static_cast<size_t>(indices_per_segment() * sizeof(IBufferType));
}

unsigned int GCodeRenderGenerator::TBuffer::max_indices_per_segment() const
{
    switch (render_primitive_type)
    {
    case ERenderPrimitiveType::Line: { return 2; }
    case ERenderPrimitiveType::Triangle: { return 36; } // 3 indices x 12 triangles
    default: { return 0; }
    }
}

size_t GCodeRenderGenerator::TBuffer::max_indices_per_segment_size_bytes() const
{
    return max_indices_per_segment() * sizeof(IBufferType);
}

bool GCodeRenderGenerator::TBuffer::has_data() const
{
    switch (render_primitive_type)
    {
    case ERenderPrimitiveType::Line:
    case ERenderPrimitiveType::Triangle: {
        return !vertices.vbos.empty() && vertices.vbos.front() != 0 && !indices.empty() && indices.front().ibo != 0;
    }
    case ERenderPrimitiveType::InstancedModel: {
        return true;
    }
    case ERenderPrimitiveType::BatchedModel: {
        return !model.data.vertices.empty() && !model.data.indices.empty() &&
            !vertices.vbos.empty() && vertices.vbos.front() != 0 && !indices.empty() && indices.front().ibo != 0;
    }
    default: { return false; }
    }
}

ColorRGBA GCodeRenderGenerator::Extrusions::Range::get_color_at(float value) const
{
    // Input value scaled to the colors range
    const float step = step_size();
    float _min = min;
    if(log_scale) {
        value = std::log(value);
        _min = std::log(min);
    }
    const float global_t = (step != 0.0f) ? std::max(0.0f, value - _min) / step : 0.0f; // lower limit of 0.0f

    const size_t color_max_idx = Range_Colors.size() - 1;

    // Compute the two colors just below (low) and above (high) the input value
    const size_t color_low_idx = std::clamp<size_t>(static_cast<size_t>(global_t), 0, color_max_idx);
    const size_t color_high_idx = std::clamp<size_t>(color_low_idx + 1, 0, color_max_idx);

    // Interpolate between the low and high colors to find exactly which color the input value should get
    return lerp(Range_Colors[color_low_idx], Range_Colors[color_high_idx], global_t - static_cast<float>(color_low_idx));
}

float GCodeRenderGenerator::Extrusions::Range::step_size() const {
if (log_scale)
    {
        float min_range = min;
        if (min_range == 0)
            min_range = 0.001f;
        return (std::log(max / min_range) / (static_cast<float>(Range_Colors.size()) - 1.0f));
    } else
    return (max - min) / (static_cast<float>(Range_Colors.size()) - 1.0f);
}

float GCodeRenderGenerator::Extrusions::Range::get_value_at_step(int step) const {
    if (!log_scale)
        return min + static_cast<float>(step) * step_size();
    else
    return std::exp(std::log(min) + static_cast<float>(step) * step_size());
    
}
GCodeRenderGenerator::SequentialRangeCap::~SequentialRangeCap() {

}

void GCodeRenderGenerator::SequentialRangeCap::reset() {
    buffer = nullptr;
    ibo = 0;
    vbo = 0;
    color = { 0.0f, 0.0f, 0.0f, 1.0f };
}

GCodeRenderGenerator::GCodeRenderGenerator()
{
    m_extrusions.reset_role_visibility_flags();
}

GCodeRenderGenerator::~GCodeRenderGenerator()
{
    //reset();
}

void GCodeRenderGenerator::init(ConfigOptionMode mode ,bool isgcode)
{
    if (m_gl_data_initialized)
        return;

    if (isgcode)
    {
        BOOST_LOG_TRIVIAL(info) << __FUNCTION__ << boost::format(": enter, m_buffers.size=%1%") % m_buffers.size();
        // initializes opengl data of TBuffers
        for (size_t i = 0; i < m_buffers.size(); ++i) {
            TBuffer&  buffer = m_buffers[i];
            EMoveType type   = buffer_type(i);
            switch (type) {
            default: {
                break;
            }
            case EMoveType::Tool_change:
            case EMoveType::Color_change:
            case EMoveType::Pause_Print:
            case EMoveType::Custom_GCode:
            case EMoveType::Retract:
            case EMoveType::Unretract:
            case EMoveType::Seam: {
                if (type == EMoveType::Seam)
                    buffer.visible = true;

                buffer.render_primitive_type = TBuffer::ERenderPrimitiveType::BatchedModel;
                buffer.vertices.format       = VBuffer::EFormat::PositionNormal3;
                buffer.shader                = "gouraud_light";

                buffer.model.data             = diamond(16);
                buffer.model.color            = option_color(type);
                buffer.model.instances.format = InstanceVBuffer::EFormat::BatchedModel;
                //            }
                break;
            }
            case EMoveType::Wipe:
            case EMoveType::Extrude: {
                buffer.render_primitive_type = TBuffer::ERenderPrimitiveType::Triangle;
                buffer.vertices.format       = VBuffer::EFormat::PositionNormal3;
                buffer.shader                = "gouraud_light";
                break;
            }
            case EMoveType::Travel: {
                buffer.render_primitive_type = TBuffer::ERenderPrimitiveType::Line;
                buffer.vertices.format       = VBuffer::EFormat::Position;
                buffer.shader                = "flat";
                break;
            }
            }

            set_toolpath_move_type_visible(EMoveType::Extrude, true);
        }

    }
    m_gl_data_initialized = true;
    BOOST_LOG_TRIVIAL(info) << __FUNCTION__ << boost::format(": finished");
}

void GCodeRenderGenerator::calculateVolume(const GCodeProcessorResult& gcode_result)
{
    auto& ps = gcode_result.print_statistics;
    m_gcodePath.materialLength = 0;
    m_gcodePath.materialWeight = 0;

    auto& modelLengths = m_gcodePath.perFilamentUsedLength;
    auto& modelWeights = m_gcodePath.perFilamentUsedWeight;
    auto& flushLengths = m_gcodePath.flushPerFilamentUsedLength;
    auto& flushWeights = m_gcodePath.flushPerFilamentUsedWeight;
    auto& towerLengths = m_gcodePath.towerPerFilamentUsedLength;
    auto& towerWeights = m_gcodePath.towerPerFilamentUsedWeight;

    int toolCount = gcode_result.tool_change_path.size();
    for (int i = 0; i < toolCount; ++i)
    {
        if (gcode_result.tool_change_path[i].empty())
            continue;

        double   koef = 0.001;
        float filament_radius = 0.5f * gcode_result.filament_diameters[i];
        float filament_density = gcode_result.filament_densities[i];
        double s = M_PI * sqr(filament_radius) > 0.0f ? M_PI * sqr(filament_radius) : 1.0f;
        double lengthFactor = s / koef;
        double weightFactor = filament_density * koef;

        for (int newTool : gcode_result.tool_change_path[i])
        {
            double volume = gcode_result.tool_change_volumes_map[i][newTool];
            flushLengths[newTool] += volume / lengthFactor;
            flushWeights[newTool] += volume * weightFactor;
        }
	}
    for (int i = 0, count = flushWeights.size(); i < count; ++i)
	{
		m_gcodePath.materialLength += flushLengths[i];
		m_gcodePath.materialWeight += flushWeights[i];
    }

    for (const auto& pair : ps.volumes_per_extruder) {
        int extruderId = pair.first;
        double volume = pair.second;
        double   koef = 0.001;

        double length = (koef * volume / (PI * sqr(0.5 * gcode_result.filament_diameters[extruderId])));
        modelLengths.insert(std::pair<int, float>(extruderId, length));
        double weight = (volume * gcode_result.filament_densities[extruderId] * koef);
        modelWeights.insert(std::pair<int, float>(extruderId, weight));
        m_gcodePath.materialLength += length;
        m_gcodePath.materialWeight += weight;

        if (ps.volumes_per_extruder.size() > 1 && ps.wipe_tower_volumes_per_extruder.find(extruderId) != ps.wipe_tower_volumes_per_extruder.end())
        {   // multi color
           volume = ps.wipe_tower_volumes_per_extruder.at(extruderId);

           length = (koef * volume / (PI * sqr(0.5 * gcode_result.filament_diameters[extruderId])));
           towerLengths.insert(std::pair<int, float>(extruderId, length));
           weight = (volume * gcode_result.filament_densities[extruderId] * koef);
           towerWeights.insert(std::pair<int, float>(extruderId, weight));
           m_gcodePath.materialLength += length;
           m_gcodePath.materialWeight += weight;
        }
    }

    if (ps.total_filament_cost == 0)
    {
        float orcaEstimatedWeight = ps.total_weight;
        m_gcodePath.materialCost = ps.total_cost * m_gcodePath.materialWeight / orcaEstimatedWeight;
    }
    else 
        m_gcodePath.materialCost = ps.total_filament_cost;
}

bool GCodeRenderGenerator::loadImpl(const GCodeProcessorResult& gcode_result)
{
    if (m_last_result_id == gcode_result.id) {
        return false;
    }

    //gcode_result.lock();
    if (gcode_result.moves.size() == 0) {
        //gcode_result.unlock();
        return false;
    }

    reset();

    //BBS: move the id to the end of reset
    m_last_result_id = gcode_result.id;
    m_gcode_result = &gcode_result;

    std::vector<std::string> str_tool_colors;
    refresh(gcode_result, str_tool_colors);
    load_toolpaths(gcode_result);
    //gcode_result.unlock();
    refresh_render_paths(false, false);

    m_gcodePath.gcodeMap = m_sequential_view.gcode_ids;
    m_gcodePath.gcodeLinesEnd = gcode_result.lines_ends;

    m_gcodePath.imageData = gcode_result.image_data;
    m_gcodePath.fliamentTypes = gcode_result.creality_fliament_types;
    m_gcodePath.defaultFliamentColors = gcode_result.creality_extruder_colors;
    m_gcodePath.fliamentColors = gcode_result.creality_complete_extruder_colors;
    m_gcodePath.defaultFliamentColorsWithUnused = gcode_result.creality_extruder_colors_with_unused;

    /* calculate volumn */
    calculateVolume(gcode_result);

    auto& ps = gcode_result.print_statistics;
    auto& mode = ps.modes[0];
    for (auto& role : mode.roles_times)
        m_gcodePath.rolesTime.push_back(std::pair<int, float>(role.first, role.second));

    m_gcodePath.timeParts = m_gcodePath.rolesTime;
    for (auto& move : mode.moves_times)
    {
        m_gcodePath.movesTime.push_back(std::pair<int, float>((int)move.first, move.second));
        m_gcodePath.timeParts.push_back(std::pair<int, float>((int)move.first + (int)erCount + 1, move.second));
    }

    m_gcodePath.layersTime = mode.layers_times;

    if (gcode_result.printable_area.size() >= 4)
    {
        m_gcodePath.printableWidth = gcode_result.printable_area[2][0];
        m_gcodePath.printableLength = gcode_result.printable_area[2][1];
    }
    m_gcodePath.printableHeight = gcode_result.printable_height;

    m_gcodePath.printerModel = gcode_result.printer_model;
    m_gcodePath.nozzleDiameter = gcode_result.nozzle_diameter;

    return true;
}

//BBS: always load shell at preview
void GCodeRenderGenerator::load(const GCodeProcessorResult& gcode_result, Print& print)
{
    if (!loadImpl(gcode_result))
        return;

    auto& ps = gcode_result.print_statistics;
     m_gcodePath.printTime = ps.modes[0].time + print.config().creality_flush_time * ps.total_filamentchanges;
    auto hexColors = print.config().default_filament_colour;
    for (std::string hexColor : hexColors.values)
    {
        if (!hexColor.empty())
        {
            int r = std::stoi(hexColor.substr(1, 2), nullptr, 16);
            int g = std::stoi(hexColor.substr(3, 2), nullptr, 16);
            int b = std::stoi(hexColor.substr(5, 2), nullptr, 16);
            m_gcodePath.tools.push_back(crslice2::GCodePath::ExtruderColor(r, g, b));
        }
        else
        {
            m_gcodePath.tools.push_back(crslice2::GCodePath::ExtruderColor(0, 0, 0));
        }

    }
}

void GCodeRenderGenerator::load(const GCodeProcessorResult& gcode_result)
{
    if (!loadImpl(gcode_result)) {
        return;
    }

    auto& ps = gcode_result.print_statistics;
    m_gcodePath.printTime = ps.total_estimated_time;
    auto hexColors = gcode_result.creality_complete_extruder_colors;
    for (std::string hexColor : hexColors)
    {
        if (!hexColor.empty() && hexColor != "\"\"")
        {
            int r = std::stoi(hexColor.substr(1, 2), nullptr, 16);
            int g = std::stoi(hexColor.substr(3, 2), nullptr, 16);
            int b = std::stoi(hexColor.substr(5, 2), nullptr, 16);
            m_gcodePath.tools.push_back(crslice2::GCodePath::ExtruderColor(r, g, b));
        }
        else
        {
            m_gcodePath.tools.push_back(crslice2::GCodePath::ExtruderColor(0, 0, 0));
        }
    }
}

void GCodeRenderGenerator::refresh(const GCodeProcessorResult& gcode_result, const std::vector<std::string>& str_tool_colors)
{
    m_moves_count = gcode_result.moves.size();
    if (m_moves_count == 0) {
        return;
    }

    //wxBusyCursor busy;

    if (m_view_type == EViewType::Tool && !gcode_result.extruder_colors.empty()) {
        // update tool colors from config stored in the gcode
        decode_colors(gcode_result.extruder_colors, m_tools.m_tool_colors);
        m_tools.m_tool_visibles = std::vector<bool>(m_tools.m_tool_colors.size());
        for (auto item: m_tools.m_tool_visibles) item = true;
    }
    else {
        // update tool colors
        decode_colors(str_tool_colors, m_tools.m_tool_colors);
        m_tools.m_tool_visibles = std::vector<bool>(m_tools.m_tool_colors.size());
        for (auto item : m_tools.m_tool_visibles) item = true;
    }

    //for (int i = 0; i < m_tools.m_tool_colors.size(); i++) {
    //    m_tools.m_tool_colors[i] = adjust_color_for_rendering(m_tools.m_tool_colors[i]);
    //}
    ColorRGBA default_color;
    decode_color("#FF8000", default_color);
	// ensure there are enough colors defined
    while (m_tools.m_tool_colors.size() < std::max(size_t(1), gcode_result.extruders_count)) {
        m_tools.m_tool_colors.push_back(default_color);
        m_tools.m_tool_visibles.push_back(true);
    }

    // update ranges for coloring / legend
    m_extrusions.reset_ranges();
    for (size_t i = 0; i < m_moves_count; ++i) {
        // skip first vertex
        if (i == 0)
            continue;

        const GCodeProcessorResult::MoveVertex& curr = gcode_result.moves[i];

        switch (curr.type)
        {
        case EMoveType::Extrude:
        {
            m_extrusions.ranges.height.update_from(round_to_bin(curr.height));
            m_extrusions.ranges.width.update_from(round_to_bin(curr.width));
            m_extrusions.ranges.fan_speed.update_from(curr.fan_speed);
            m_extrusions.ranges.temperature.update_from(curr.temperature);
            m_extrusions.ranges.feedrate.update_from(curr.feedrate);

            if (curr.acc == 0)
                m_extrusions.ranges.acc.update_from(m_gcode_result->defaultAcc);
            else 
                m_extrusions.ranges.acc.update_from(curr.acc);

            if (curr.extrusion_role != erCustom)
                m_extrusions.ranges.volumetric_rate.update_from(round_to_bin(curr.volumetric_rate()));

            if (curr.layer_duration > 0.f) {
                m_extrusions.ranges.layer_duration.update_from(curr.layer_duration);
                m_extrusions.ranges.layer_duration_log.update_from(curr.layer_duration);
            }
            [[fallthrough]];
        }
        case EMoveType::Travel:
        {
           //m_extrusions.ranges.feedrate.update_from(curr.feedrate);

            break;
        }
        default: { break; }
        }
    }

    Extrusions::Range& feedrate = m_extrusions.ranges.feedrate;
    Extrusions::Range& temperature = m_extrusions.ranges.temperature;
    Extrusions::Range& height = m_extrusions.ranges.height;
    Extrusions::Range& width = m_extrusions.ranges.width;
    Extrusions::Range& fan_speed = m_extrusions.ranges.fan_speed;
    Extrusions::Range& volumetric_rate = m_extrusions.ranges.volumetric_rate;
    Extrusions::Range& layer_duration = m_extrusions.ranges.layer_duration;
    Extrusions::Range& acc = m_extrusions.ranges.acc;

    if (feedrate.count != 0)
        m_gcodePath.speedRange = crslice2::GCodeRange(feedrate.min, feedrate.max);
    else
        m_gcodePath.speedRange = crslice2::GCodeRange(0.0, 0.0);

    if (temperature.count != 0)
        m_gcodePath.temperatureRange = crslice2::GCodeRange(temperature.min, temperature.max);
    else
        m_gcodePath.temperatureRange = crslice2::GCodeRange(0.0, 0.0);

    if (height.count != 0)
        m_gcodePath.heightRange = crslice2::GCodeRange(height.min, height.max);
    else
        m_gcodePath.heightRange = crslice2::GCodeRange(0.0, 0.0);

    if (width.count != 0)
        m_gcodePath.widthRange = crslice2::GCodeRange(width.min, width.max);
    else
        m_gcodePath.widthRange = crslice2::GCodeRange(0.0, 0.0);

    if (fan_speed.count != 0)
        m_gcodePath.fanSpeedRange = crslice2::GCodeRange(fan_speed.min, fan_speed.max);
    else
        m_gcodePath.fanSpeedRange = crslice2::GCodeRange(0.0, 0.0);

    if (volumetric_rate.count != 0)
        m_gcodePath.volumetricRateRange = crslice2::GCodeRange(volumetric_rate.min, volumetric_rate.max);
    else
        m_gcodePath.volumetricRateRange = crslice2::GCodeRange(0.0, 0.0);

    if (layer_duration.count != 0)
        m_gcodePath.layerDurationRange = crslice2::GCodeRange(layer_duration.min, layer_duration.max);
    else
        m_gcodePath.layerDurationRange = crslice2::GCodeRange(0.0, 0.0);

    if (acc.count != 0)
        m_gcodePath.accRange = crslice2::GCodeRange(acc.min, acc.max);
    else
        m_gcodePath.accRange = crslice2::GCodeRange(0.0, 0.0);

}

void GCodeRenderGenerator::reset()
{
    //BBS: should also reset the result id
    BOOST_LOG_TRIVIAL(info) << __FUNCTION__ << boost::format(": current result id %1% ")%m_last_result_id;
    m_last_result_id = -1;

    m_moves_count = 0;
    m_ssid_to_moveid_map.clear();
    m_ssid_to_moveid_map.shrink_to_fit();
    m_gcodePath.blocks.clear();
    for (TBuffer& buffer : m_buffers) {
        buffer.reset();
    }
    m_paths_bounding_box = BoundingBoxf3();
    m_tools.m_tool_colors = std::vector<ColorRGBA>();
    m_tools.m_tool_visibles = std::vector<bool>();
    m_extruder_ids = std::vector<unsigned char>();
    m_extrusions.reset_ranges();
    //BBS: always load shell at preview

    m_layers.reset();
    m_layers_z_range = { 0, 0 };
    m_roles = std::vector<ExtrusionRole>();
    m_print_statistics.reset();
    m_contained_in_bed = true;
}

bool GCodeRenderGenerator::can_export_toolpaths() const
{
    return has_data() && m_buffers[buffer_id(EMoveType::Extrude)].render_primitive_type == TBuffer::ERenderPrimitiveType::Triangle;
}

void GCodeRenderGenerator::set_toolpath_move_type_visible(EMoveType type, bool visible)
{
    size_t id = static_cast<size_t>(buffer_id(type));
    if (id < m_buffers.size())
        m_buffers[id].visible = visible;
}

void GCodeRenderGenerator::add_step(const GCodeProcessorResult::MoveVertex& curr, const Vec3f& position, StepBuffer& steps, int move_id) 
{
    if (curr.type == EMoveType::Seam)
    {
        if (steps.empty() || steps.back().moveId != move_id)
        {
            crslice2::StepInfoPtr ptr;
            ptr.moveId = move_id;
            ptr.span = 1;
            steps.push_back(ptr);
        }
        else
        {
            crslice2::StepInfoPtr& ptr = steps.back();
            ptr.span++;
        }
        return;
    }

    if (steps.empty() || steps.back().moveId != move_id)
    {
        crslice2::StepInfoPtr ptr;
        ptr.moveId = move_id;
        ptr.span = 1;
        steps.push_back(ptr);
    }
    crslice2::StepInfoPtr& stepPtr = steps.back();

    float role = curr.type == EMoveType::Extrude ? (float)curr.extrusion_role : ((float)curr.type + (float)ExtrusionRole::erCount + 2);
    if (!m_gcodePath.stepMap[stepPtr.moveId].valid())
    {
        crslice2::StepInfo step;
        step.moveId = move_id;
        step.role = role;
        step.extruder = curr.extruder_id;
        step.speed = (curr.feedrate);
        step.temperature = (curr.temperature);
        step.height = round_to_bin(curr.height);
        step.width = round_to_bin(curr.width);
        step.fanSpeed = (curr.fan_speed);
        step.volumetricRate = round_to_bin(curr.volumetric_rate());
        step.layerDuration = (curr.layer_duration);
        step.acc = curr.acc == 0 ? m_gcode_result->defaultAcc : curr.acc;	//加速度
        step.position[0] = position.x();
        step.position[1] = position.y();
        step.position[2] = position.z();
        m_gcodePath.stepMap[move_id] = step;
    }
    else
    {
        stepPtr.span++;
    }
};
void GCodeRenderGenerator::load_toolpaths(const GCodeProcessorResult& gcode_result)
{
    // max index buffer size, in bytes
    static const size_t IBUFFER_THRESHOLD_BYTES = 64 * 1024 * 1024;

    // format data into the buffers to be rendered as lines
    auto add_vertices_as_line = [=](const GCodeProcessorResult::MoveVertex& prev, const GCodeProcessorResult::MoveVertex& curr,
        VertexBuffer& vertices, StepBuffer& steps, int move_id) {
            auto add_vertex = [&vertices](const Vec3f& position) {
                // add position
                vertices.push_back(position.x());
                vertices.push_back(position.y());
                vertices.push_back(position.z());
            };
            // x component of the normal to the current segment (the normal is parallel to the XY plane)
            //BBS: Has modified a lot for this function to support arc move
            size_t loop_num = curr.is_arc_move_with_interpolation_points() ? curr.interpolation_points.size() : 0;
            for (size_t i = 0; i < loop_num + 1; i++) {
                const Vec3f& previous = (i == 0 ? prev.position : curr.interpolation_points[i - 1]);
                const Vec3f& current = (i == loop_num ? curr.position : curr.interpolation_points[i]);
                // add previous vertex
                add_vertex(previous);
                add_step(curr, curr.position, steps, move_id);

                // add current vertex
                add_vertex(current);
                add_step(curr, curr.position, steps, move_id);

            }
    };
    //BBS: modify a lot to support arc travel
    auto add_indices_as_line = [](const GCodeProcessorResult::MoveVertex& prev, const GCodeProcessorResult::MoveVertex& curr, TBuffer& buffer,
        size_t& vbuffer_size, unsigned int ibuffer_id, IndexBuffer& indices, size_t move_id) {

            if (buffer.paths.empty() || prev.type != curr.type || !buffer.paths.back().matches(curr)) {
                buffer.add_path(curr, ibuffer_id, indices.size(), move_id - 1);
                buffer.paths.back().sub_paths.front().first.position = prev.position;
            }

            Path& last_path = buffer.paths.back();
            size_t loop_num = curr.is_arc_move_with_interpolation_points() ? curr.interpolation_points.size() : 0;
            for (size_t i = 0; i < loop_num + 1; i++) {
                //BBS: add previous index
                indices.push_back(static_cast<IBufferType>(indices.size()));
                //BBS: add current index
                indices.push_back(static_cast<IBufferType>(indices.size()));
                vbuffer_size += buffer.max_vertices_per_segment();
            }
            last_path.sub_paths.back().last = { ibuffer_id, indices.size() - 1, move_id, curr.position };
    };

    // format data into the buffers to be rendered as solid.
    auto add_vertices_as_solid = [=](const GCodeProcessorResult::MoveVertex& prev, const GCodeProcessorResult::MoveVertex& curr,
        TBuffer& buffer, unsigned int vbuffer_id, VertexBuffer& vertices,
        StepBuffer& steps, size_t move_id) {

            auto store_vertex = [=](VertexBuffer& vertices, const Vec3f& position, const Vec3f& normal) {
                // append position
                vertices.push_back(position.x());
                vertices.push_back(position.y());
                vertices.push_back(position.z());
                // append normal
                vertices.push_back(normal.x());
                vertices.push_back(normal.y());
                vertices.push_back(normal.z());

            };

            if (buffer.paths.empty() || prev.type != curr.type || !buffer.paths.back().matches(curr)) {
                buffer.add_path(curr, vbuffer_id, vertices.size(), move_id - 1);
                buffer.paths.back().sub_paths.back().first.position = prev.position;
            }

            Path& last_path = buffer.paths.back();
            //BBS: Has modified a lot for this function to support arc move
            size_t loop_num = curr.is_arc_move_with_interpolation_points() ? curr.interpolation_points.size() : 0;
            for (size_t i = 0; i < loop_num + 1; i++) {
                const Vec3f& prev_position = (i == 0 ? prev.position : curr.interpolation_points[i - 1]);
                const Vec3f& curr_position = (i == loop_num ? curr.position : curr.interpolation_points[i]);

                const Vec3f dir = (curr_position - prev_position).normalized();
                const Vec3f right = Vec3f(dir.y(), -dir.x(), 0.0f).normalized();
                const Vec3f left = -right;
                const Vec3f up = right.cross(dir);
                const Vec3f down = -up;
                const float half_width = 0.5f * last_path.width;
                const float half_height = 0.5f * last_path.height;
                const Vec3f prev_pos = prev_position - half_height * up;
                const Vec3f curr_pos = curr_position - half_height * up;
                const Vec3f d_up = half_height * up;
                const Vec3f d_down = -half_height * up;
                const Vec3f d_right = half_width * right;
                const Vec3f d_left = -half_width * right;

                if ((last_path.vertices_count() == 1 || vertices.empty()) && i == 0) {
                    store_vertex(vertices, prev_pos + d_up, up);
                    add_step(curr, curr_position, steps, move_id);
                    store_vertex(vertices, prev_pos + d_right, right);
                    add_step(curr, curr_position, steps, move_id);
                    store_vertex(vertices, prev_pos + d_down, down);
                    add_step(curr, curr_position, steps, move_id);
                    store_vertex(vertices, prev_pos + d_left, left);
                    add_step(curr, curr_position, steps, move_id);
                }
                else {
                    store_vertex(vertices, prev_pos + d_right, right);
                    add_step(curr, curr_position, steps, move_id);
                    store_vertex(vertices, prev_pos + d_left, left);
                    add_step(curr, curr_position, steps, move_id);
                }

                store_vertex(vertices, curr_pos + d_up, up);
                add_step(curr, curr_position, steps, move_id);
                store_vertex(vertices, curr_pos + d_right, right);
                add_step(curr, curr_position, steps, move_id);
                store_vertex(vertices, curr_pos + d_down, down);
                add_step(curr, curr_position, steps, move_id);
                store_vertex(vertices, curr_pos + d_left, left);
                add_step(curr, curr_position, steps, move_id);
            }

            last_path.sub_paths.back().last = { vbuffer_id, vertices.size(), move_id, curr.position };
    };
    auto add_indices_as_solid = [&](const GCodeProcessorResult::MoveVertex& prev, const GCodeProcessorResult::MoveVertex& curr, const GCodeProcessorResult::MoveVertex* next,
        TBuffer& buffer, size_t& vbuffer_size, unsigned int ibuffer_id, IndexBuffer& indices, size_t move_id) {
            static Vec3f prev_dir;
            static Vec3f prev_up;
            static float sq_prev_length;
            auto store_triangle = [](IndexBuffer& indices, IBufferType i1, IBufferType i2, IBufferType i3) {
                indices.push_back(i1);
                indices.push_back(i2);
                indices.push_back(i3);
            };
            auto append_dummy_cap = [store_triangle](IndexBuffer& indices, IBufferType id) {
                store_triangle(indices, id, id, id);
                store_triangle(indices, id, id, id);
            };
            auto convert_vertices_offset = [](size_t vbuffer_size, const std::array<int, 8>& v_offsets) {
                std::array<IBufferType, 8> ret = {
                    static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[0]),
                    static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[1]),
                    static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[2]),
                    static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[3]),
                    static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[4]),
                    static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[5]),
                    static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[6]),
                    static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[7])
                };
                return ret;
            };
            auto append_starting_cap_triangles = [&](IndexBuffer& indices, const std::array<IBufferType, 8>& v_offsets) {
                store_triangle(indices, v_offsets[0], v_offsets[2], v_offsets[1]);
                store_triangle(indices, v_offsets[0], v_offsets[3], v_offsets[2]);
            };
            auto append_stem_triangles = [&](IndexBuffer& indices, const std::array<IBufferType, 8>& v_offsets) {
                store_triangle(indices, v_offsets[0], v_offsets[1], v_offsets[4]);
                store_triangle(indices, v_offsets[1], v_offsets[5], v_offsets[4]);
                store_triangle(indices, v_offsets[1], v_offsets[2], v_offsets[5]);
                store_triangle(indices, v_offsets[2], v_offsets[6], v_offsets[5]);
                store_triangle(indices, v_offsets[2], v_offsets[3], v_offsets[6]);
                store_triangle(indices, v_offsets[3], v_offsets[7], v_offsets[6]);
                store_triangle(indices, v_offsets[3], v_offsets[0], v_offsets[7]);
                store_triangle(indices, v_offsets[0], v_offsets[4], v_offsets[7]);
            };
            auto append_ending_cap_triangles = [&](IndexBuffer& indices, const std::array<IBufferType, 8>& v_offsets) {
                store_triangle(indices, v_offsets[4], v_offsets[6], v_offsets[7]);
                store_triangle(indices, v_offsets[4], v_offsets[5], v_offsets[6]);
            };

            if (buffer.paths.empty() || prev.type != curr.type || !buffer.paths.back().matches(curr)) {
                buffer.add_path(curr, ibuffer_id, indices.size(), move_id - 1);
                buffer.paths.back().sub_paths.back().first.position = prev.position;
            }

            Path& last_path = buffer.paths.back();
            bool is_first_segment = (last_path.vertices_count() == 1);
            //BBS: has modified a lot for this function to support arc move
            std::array<IBufferType, 8> first_seg_v_offsets = convert_vertices_offset(vbuffer_size, { 0, 1, 2, 3, 4, 5, 6, 7 });
            std::array<IBufferType, 8> non_first_seg_v_offsets = convert_vertices_offset(vbuffer_size, { -4, 0, -2, 1, 2, 3, 4, 5 });

            size_t loop_num = curr.is_arc_move_with_interpolation_points() ? curr.interpolation_points.size() : 0;
            for (size_t i = 0; i < loop_num + 1; i++) {
                const Vec3f& prev_position = (i == 0 ? prev.position : curr.interpolation_points[i - 1]);
                const Vec3f& curr_position = (i == loop_num ? curr.position : curr.interpolation_points[i]);

                const Vec3f dir = (curr_position - prev_position).normalized();
                const Vec3f right = Vec3f(dir.y(), -dir.x(), 0.0f).normalized();
                const Vec3f up = right.cross(dir);
                const float sq_length = (curr_position - prev_position).squaredNorm();

                if ((is_first_segment || vbuffer_size == 0) && i == 0) {
                    if (is_first_segment && i == 0)
                        // starting cap triangles
                        append_starting_cap_triangles(indices, first_seg_v_offsets);
                    // dummy triangles outer corner cap
                    append_dummy_cap(indices, vbuffer_size);
                    // stem triangles
                    append_stem_triangles(indices, first_seg_v_offsets);

                    vbuffer_size += 8;
                }
                else {
                    float displacement = 0.0f;
                    float cos_dir = prev_dir.dot(dir);
                    if (cos_dir > -0.9998477f) {
                        // if the angle between adjacent segments is smaller than 179 degrees
                        const Vec3f med_dir = (prev_dir + dir).normalized();
                        const float half_width = 0.5f * last_path.width;
                        displacement = half_width * ::tan(::acos(std::clamp(dir.dot(med_dir), -1.0f, 1.0f)));
                    }

                    float sq_displacement = sqr(displacement);
                    bool can_displace = displacement > 0.0f && sq_displacement < sq_prev_length&& sq_displacement < sq_length;

                    bool is_right_turn = prev_up.dot(prev_dir.cross(dir)) <= 0.0f;
                    // whether the angle between adjacent segments is greater than 45 degrees
                    bool is_sharp = cos_dir < 0.7071068f;

                    bool right_displaced = false;
                    bool left_displaced = false;

                    if (!is_sharp && can_displace) {
                        if (is_right_turn)
                            left_displaced = true;
                        else
                            right_displaced = true;
                    }

                    // triangles outer corner cap
                    if (is_right_turn) {
                        if (left_displaced)
                            // dummy triangles
                            append_dummy_cap(indices, vbuffer_size);
                        else {
                            store_triangle(indices, vbuffer_size - 4, vbuffer_size + 1, vbuffer_size - 1);
                            store_triangle(indices, vbuffer_size + 1, vbuffer_size - 2, vbuffer_size - 1);
                        }
                    }
                    else {
                        if (right_displaced)
                            // dummy triangles
                            append_dummy_cap(indices, vbuffer_size);
                        else {
                            store_triangle(indices, vbuffer_size - 4, vbuffer_size - 3, vbuffer_size + 0);
                            store_triangle(indices, vbuffer_size - 3, vbuffer_size - 2, vbuffer_size + 0);
                        }
                    }
                    // stem triangles
                    non_first_seg_v_offsets = convert_vertices_offset(vbuffer_size, { -4, 0, -2, 1, 2, 3, 4, 5 });
                    append_stem_triangles(indices, non_first_seg_v_offsets);
                    vbuffer_size += 6;
                }
                prev_dir = dir;
                prev_up = up;
                sq_prev_length = sq_length;
            }

            if (next != nullptr && (curr.type != next->type || !last_path.matches(*next)))
                // ending cap triangles
                append_ending_cap_triangles(indices, (is_first_segment && !curr.is_arc_move_with_interpolation_points()) ? first_seg_v_offsets : non_first_seg_v_offsets);

            last_path.sub_paths.back().last = { ibuffer_id, indices.size() - 1, move_id, curr.position };
    };

    // format data into the buffers to be rendered as instanced model
    auto add_model_instance = [](const GCodeProcessorResult::MoveVertex& curr, InstanceBuffer& instances, InstanceIdBuffer& instances_ids, size_t move_id) {
        // append position
        instances.push_back(curr.position.x());
        instances.push_back(curr.position.y());
        instances.push_back(curr.position.z());
        // append width
        instances.push_back(curr.width);
        // append height
        instances.push_back(curr.height);

        // append id
        instances_ids.push_back(move_id);
    };


    // format data into the buffers to be rendered as batched model
    auto add_vertices_as_model_batch = [=](const GCodeProcessorResult::MoveVertex& curr, const GLModel::Geometry& data,
                                          VertexBuffer& vertices, InstanceBuffer& instances, InstanceIdBuffer& instances_ids,
                                          size_t move_id, StepBuffer& steps) {

       

        const double width = static_cast<double>(1.5f * curr.width);
        const double height = static_cast<double>(1.5f * curr.height);

        const Transform3d trafo = Geometry::assemble_transform((curr.position - 0.5f * curr.height * Vec3f::UnitZ()).cast<double>(),
                                                                        Vec3d::Zero(), {width, width, height});
        const Eigen::Matrix<double, 3, 3, Eigen::DontAlign> normal_matrix = trafo.matrix().template block<3, 3>(0, 0).inverse().transpose();

        // append vertices
        const size_t vertices_count = data.vertices_count();
        for (size_t i = 0; i < vertices_count; ++i) {
            // append position
            const Vec3d position = trafo * data.extract_position_3(i).cast<double>();
            vertices.push_back(float(position.x()));
            vertices.push_back(float(position.y()));
            vertices.push_back(float(position.z()));

            // append normal
            const Vec3d normal = normal_matrix * data.extract_normal_3(i).cast<double>();
            vertices.push_back(float(normal.x()));
            vertices.push_back(float(normal.y()));
            vertices.push_back(float(normal.z()));

            Vec3f p(position.x(), position.y(), position.z());
            add_step(curr, p, steps, move_id);
        }

        // append instance position
        instances.push_back(curr.position.x());
        instances.push_back(curr.position.y());
        instances.push_back(curr.position.z());
        // append instance id
        instances_ids.push_back(move_id);
    };

    auto add_indices_as_model_batch = [](const GLModel::Geometry& data, IndexBuffer& indices, IBufferType base_index) {
        const size_t indices_count = data.indices_count();
        for (size_t i = 0; i < indices_count; ++i) {
            indices.push_back(static_cast<IBufferType>(data.extract_index(i) + base_index));
        }
    };

    auto add_bounding_box = [](trimesh::box& box, trimesh::vec3& newPos)
    {
        trimesh::vec3& max = box.max;
        trimesh::vec3& min = box.min;
        max.x = newPos.x > max.x ? newPos.x : max.x;
        max.y = newPos.y > max.y ? newPos.y : max.y;
        max.z = newPos.z > max.z ? newPos.z : max.z;
        min.x = newPos.x < min.x ? newPos.x : min.x;
        min.y = newPos.y < min.y ? newPos.y : min.y;
        min.z = newPos.z < min.z ? newPos.z : min.z;
    };

    m_moves_count = gcode_result.moves.size();
    if (m_moves_count == 0)
        return;

    unsigned int progress_count = 0;
    static const unsigned int progress_threshold = 1000;

    //BBS: use convex_hull for toolpath outside check
    Points pts;

    // extract approximate paths bounding box from result
    //BBS: add only gcode mode
    for (const GCodeProcessorResult::MoveVertex& move : gcode_result.moves) {
            if (move.type == EMoveType::Extrude && move.extrusion_role != erCustom && move.width != 0.0f && move.height != 0.0f) {
                m_paths_bounding_box.merge(move.position.cast<double>());
                //BBS: use convex_hull for toolpath outside check
                pts.emplace_back(Point(scale_(move.position.x()), scale_(move.position.y())));
            }
        //}
    }

    // BBS: also merge the point on arc to bounding box
    for (const GCodeProcessorResult::MoveVertex& move : gcode_result.moves) {
        // continue if not arc path
        if (!move.is_arc_move_with_interpolation_points())
            continue;

            if (move.type == EMoveType::Extrude && move.width != 0.0f && move.height != 0.0f)
                for (int i = 0; i < move.interpolation_points.size(); i++) {
                    m_paths_bounding_box.merge(move.interpolation_points[i].cast<double>());
                    //BBS: use convex_hull for toolpath outside check
                    pts.emplace_back(Point(scale_(move.interpolation_points[i].x()), scale_(move.interpolation_points[i].y())));
                }
    }

    BOOST_LOG_TRIVIAL(info) << __FUNCTION__<< boost::format(",m_paths_bounding_box {%1%, %2%}-{%3%, %4%}\n")
        %m_paths_bounding_box.min.x() %m_paths_bounding_box.min.y() %m_paths_bounding_box.max.x() %m_paths_bounding_box.max.y();

    {
        m_contained_in_bed = true;
    }

    m_sequential_view.gcode_ids.clear();
    trimesh::vec3 firstMovePos(gcode_result.moves[0].position[0], gcode_result.moves[0].position[1], gcode_result.moves[0].position[2]);
    m_gcodePath.boundingBox.min = m_gcodePath.boundingBox.max = firstMovePos;
    for (size_t i = 0; i < gcode_result.moves.size(); ++i) {
        const GCodeProcessorResult::MoveVertex& move = gcode_result.moves[i];
        if (move.type != EMoveType::Seam)
            m_sequential_view.gcode_ids.push_back(move.gcode_id);
    }
    BOOST_LOG_TRIVIAL(info) << __FUNCTION__<< boost::format(",m_contained_in_bed %1%\n")%m_contained_in_bed;

    m_gcodePath.blocks.clear();
    m_gcodePath.blocks.resize(m_buffers.size());
    std::vector<MultiVertexBuffer> vertices(m_buffers.size());
    std::vector<MultiStepBuffer> steps(m_buffers.size());

    std::vector<MultiIndexBuffer> indices(m_buffers.size());
    std::vector<InstanceBuffer> instances(m_buffers.size());
    std::vector<InstanceIdBuffer> instances_ids(m_buffers.size());
    std::vector<InstancesOffsets> instances_offsets(m_buffers.size());
    std::vector<float> options_zs;
    m_gcodePath.stepMap.resize(m_moves_count + 1);

    size_t seams_count = 0;
    std::vector<size_t> biased_seams_ids;

    // toolpaths data -> extract vertices from result
    for (size_t i = 0; i < m_moves_count; ++i) {
        const GCodeProcessorResult::MoveVertex& curr = gcode_result.moves[i];
        if (curr.type == EMoveType::Seam) {
            ++seams_count;
            biased_seams_ids.push_back(i - biased_seams_ids.size() - 1);
        }

        size_t move_id11 = i - seams_count;

        if (curr.extrusion_role != ExtrusionRole::erCustom && curr.type != EMoveType::Travel)
        {
            trimesh::vec3 movePos(curr.position[0], curr.position[1], curr.position[2]);
            add_bounding_box(m_gcodePath.boundingBox, movePos);
        }
        //for (auto& vec : curr.interpolation_points)
        //{
        //    trimesh::vec3 pos(vec[0], vec[1], vec[2]);
        //    add_bounding_box(m_gcodePath.boundingBox, pos);
        //}

        // skip first vertex
        if (i == 0)
            continue;

        const GCodeProcessorResult::MoveVertex& prev = gcode_result.moves[i - 1];

        // update progress dialog
        //++progress_count;
        //if (progress_dialog != nullptr && progress_count % progress_threshold == 0) {
        //    progress_dialog->Update(int(100.0f * float(i) / (2.0f * float(m_moves_count))),
        //        _L("Generating geometry vertex data") + ": " + wxNumberFormatter::ToString(100.0 * double(i) / double(m_moves_count), 0, wxNumberFormatter::Style_None) + "%");
        //    progress_dialog->Fit();
        //    progress_count = 0;
        //}

        const unsigned char id = buffer_id(curr.type);
        TBuffer& t_buffer = m_buffers[id];
        MultiVertexBuffer& v_multibuffer = vertices[id];
        MultiStepBuffer& step_multiBuffer = steps[id];
     

        InstanceBuffer& inst_buffer = instances[id];
        InstanceIdBuffer& inst_id_buffer = instances_ids[id];
        InstancesOffsets& inst_offsets = instances_offsets[id];

        /*if (i%1000 == 1) {
            BOOST_LOG_TRIVIAL(info) << __FUNCTION__ << boost::format(":i=%1%, buffer_id %2% render_type %3%, gcode_id %4%\n")
                %i %(int)id %(int)t_buffer.render_primitive_type %curr.gcode_id;
        }*/

        // ensure there is at least one vertex buffer
        if (v_multibuffer.empty())
        {
            v_multibuffer.push_back(VertexBuffer());
            step_multiBuffer.push_back(StepBuffer());
        }

        // if adding the vertices for the current segment exceeds the threshold size of the current vertex buffer
        // add another vertex buffer
        // BBS: get the point number and then judge whether the remaining buffer is enough
        size_t points_num = curr.is_arc_move_with_interpolation_points() ? curr.interpolation_points.size() + 1 : 1;
        size_t vertices_size_to_add = (t_buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::BatchedModel) ? t_buffer.model.data.vertices_size_bytes() : points_num * t_buffer.max_vertices_per_segment_size_bytes();
        if (v_multibuffer.back().size() * sizeof(float) > t_buffer.vertices.max_size_bytes() - vertices_size_to_add) {
            v_multibuffer.push_back(VertexBuffer());
            step_multiBuffer.push_back(StepBuffer());
            if (t_buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::Triangle) {
                Path& last_path = t_buffer.paths.back();
                if (prev.type == curr.type && last_path.matches(curr))
                    last_path.add_sub_path(prev, static_cast<unsigned int>(v_multibuffer.size()) - 1, 0, move_id11 - 1);
            }
        }

        VertexBuffer& v_buffer = v_multibuffer.back();
        StepBuffer& sb = step_multiBuffer.back();

        switch (t_buffer.render_primitive_type)
        {
        case TBuffer::ERenderPrimitiveType::Line:     { add_vertices_as_line(prev, curr, v_buffer, sb, move_id11); break; }
        case TBuffer::ERenderPrimitiveType::Triangle: { add_vertices_as_solid(prev, curr, t_buffer, static_cast<unsigned int>(v_multibuffer.size()) - 1, v_buffer, sb, move_id11); break; }
        case TBuffer::ERenderPrimitiveType::InstancedModel:
        {
            add_model_instance(curr, inst_buffer, inst_id_buffer, move_id11);
            inst_offsets.push_back(prev.position - curr.position);
            break;
        }
        case TBuffer::ERenderPrimitiveType::BatchedModel:
        {
            add_vertices_as_model_batch(curr, t_buffer.model.data, v_buffer, inst_buffer, inst_id_buffer, move_id11, sb);
            inst_offsets.push_back(prev.position - curr.position);
            break;
        }
        }

        // collect options zs for later use
        if (curr.type == EMoveType::Pause_Print || curr.type == EMoveType::Custom_GCode) {
            const float* const last_z = options_zs.empty() ? nullptr : &options_zs.back();
            if (last_z == nullptr || curr.position[2] < *last_z - EPSILON || *last_z + EPSILON < curr.position[2])
                options_zs.emplace_back(curr.position[2]);
        }
    }

    /*for (size_t b = 0; b < vertices.size(); ++b) {
        MultiVertexBuffer& v_multibuffer = vertices[b];
        BOOST_LOG_TRIVIAL(info) << __FUNCTION__ << boost::format(":b=%1%, vertex buffer count %2%\n")
            %b %v_multibuffer.size();
    }*/
    auto extract_move_id = [&biased_seams_ids](size_t id) {
        size_t new_id = size_t(-1);
        auto it = std::lower_bound(biased_seams_ids.begin(), biased_seams_ids.end(), id);
        if (it == biased_seams_ids.end())
            new_id = id + biased_seams_ids.size();
        else {
            if (it == biased_seams_ids.begin() && *it < id)
                new_id = id;
            else if (it != biased_seams_ids.begin())
                new_id = id + std::distance(biased_seams_ids.begin(), it);
        }
        return (new_id == size_t(-1)) ? id : new_id;
    };
    //BBS: generate map from ssid to move id in advance to reduce computation
    m_ssid_to_moveid_map.clear();
    m_ssid_to_moveid_map.reserve( m_moves_count - biased_seams_ids.size());
    for (size_t i = 0; i < m_moves_count - biased_seams_ids.size(); i++)
        m_ssid_to_moveid_map.push_back(extract_move_id(i));

    //BBS: smooth toolpaths corners for the given TBuffer using triangles
    auto smooth_triangle_toolpaths_corners = [&gcode_result, this](
        const TBuffer& t_buffer, MultiVertexBuffer& v_multibuffer) {
        auto extract_position_at = [](const VertexBuffer& vertices, size_t offset) {
            return Vec3f(vertices[offset + 0], vertices[offset + 1], vertices[offset + 2]);
        };
        auto update_position_at = [](VertexBuffer& vertices, size_t offset, const Vec3f& position) {
            vertices[offset + 0] = position.x();
            vertices[offset + 1] = position.y();
            vertices[offset + 2] = position.z();
        };
        auto match_right_vertices_with_internal_point = [&](const Path::Sub_Path& prev_sub_path, const Path::Sub_Path& next_sub_path,
            size_t curr_s_id, bool is_internal_point, size_t interpolation_point_id, size_t vertex_size_floats, const Vec3f& displacement_vec) {
            if (&prev_sub_path == &next_sub_path || is_internal_point) { // previous and next segment are both contained into to the same vertex buffer
                VertexBuffer& vbuffer = v_multibuffer[prev_sub_path.first.b_id];
                // offset into the vertex buffer of the next segment 1st vertex
                size_t temp_offset = prev_sub_path.last.s_id - curr_s_id;
                for (size_t i = prev_sub_path.last.s_id; i > curr_s_id; i--) {
                    size_t move_id = m_ssid_to_moveid_map[i];
                    temp_offset += (gcode_result.moves[move_id].is_arc_move() ? gcode_result.moves[move_id].interpolation_points.size() : 0);
                }
                if (is_internal_point) {
                    size_t move_id = m_ssid_to_moveid_map[curr_s_id];
                    temp_offset += (gcode_result.moves[move_id].interpolation_points.size() - interpolation_point_id);
                }
                const size_t next_1st_offset = temp_offset * 6 * vertex_size_floats;
                // offset into the vertex buffer of the right vertex of the previous segment
                const size_t prev_right_offset = prev_sub_path.last.i_id - next_1st_offset - 3 * vertex_size_floats;
                // new position of the right vertices
                const Vec3f shared_vertex = extract_position_at(vbuffer, prev_right_offset) + displacement_vec;
                // update previous segment
                update_position_at(vbuffer, prev_right_offset, shared_vertex);
                // offset into the vertex buffer of the right vertex of the next segment
                const size_t next_right_offset = prev_sub_path.last.i_id - next_1st_offset;
                // update next segment
                update_position_at(vbuffer, next_right_offset, shared_vertex);
            }
            else { // previous and next segment are contained into different vertex buffers
                VertexBuffer& prev_vbuffer = v_multibuffer[prev_sub_path.first.b_id];
                VertexBuffer& next_vbuffer = v_multibuffer[next_sub_path.first.b_id];
                // offset into the previous vertex buffer of the right vertex of the previous segment
                const size_t prev_right_offset = prev_sub_path.last.i_id - 3 * vertex_size_floats;
                // new position of the right vertices
                const Vec3f shared_vertex = extract_position_at(prev_vbuffer, prev_right_offset) + displacement_vec;
                // update previous segment
                update_position_at(prev_vbuffer, prev_right_offset, shared_vertex);
                // offset into the next vertex buffer of the right vertex of the next segment
                const size_t next_right_offset = next_sub_path.first.i_id + 1 * vertex_size_floats;
                // update next segment
                update_position_at(next_vbuffer, next_right_offset, shared_vertex);
            }
        };
        //BBS: modify a lot of this function to support arc move
        auto match_left_vertices_with_internal_point = [&](const Path::Sub_Path& prev_sub_path, const Path::Sub_Path& next_sub_path,
            size_t curr_s_id, bool is_internal_point, size_t interpolation_point_id, size_t vertex_size_floats, const Vec3f& displacement_vec) {
            if (&prev_sub_path == &next_sub_path || is_internal_point) { // previous and next segment are both contained into to the same vertex buffer
                VertexBuffer& vbuffer = v_multibuffer[prev_sub_path.first.b_id];
                // offset into the vertex buffer of the next segment 1st vertex
                size_t temp_offset = prev_sub_path.last.s_id - curr_s_id;
                for (size_t i = prev_sub_path.last.s_id; i > curr_s_id; i--) {
                    size_t move_id = m_ssid_to_moveid_map[i];
                    temp_offset += (gcode_result.moves[move_id].is_arc_move() ? gcode_result.moves[move_id].interpolation_points.size() : 0);
                }
                if (is_internal_point) {
                    size_t move_id = m_ssid_to_moveid_map[curr_s_id];
                    temp_offset += (gcode_result.moves[move_id].interpolation_points.size() - interpolation_point_id);
                }
                const size_t next_1st_offset = temp_offset * 6 * vertex_size_floats;
                // offset into the vertex buffer of the left vertex of the previous segment
                const size_t prev_left_offset = prev_sub_path.last.i_id - next_1st_offset - 1 * vertex_size_floats;
                // new position of the left vertices
                const Vec3f shared_vertex = extract_position_at(vbuffer, prev_left_offset) + displacement_vec;
                // update previous segment
                update_position_at(vbuffer, prev_left_offset, shared_vertex);
                // offset into the vertex buffer of the left vertex of the next segment
                const size_t next_left_offset = prev_sub_path.last.i_id - next_1st_offset + 1 * vertex_size_floats;
                // update next segment
                update_position_at(vbuffer, next_left_offset, shared_vertex);
            }
            else { // previous and next segment are contained into different vertex buffers
                VertexBuffer& prev_vbuffer = v_multibuffer[prev_sub_path.first.b_id];
                VertexBuffer& next_vbuffer = v_multibuffer[next_sub_path.first.b_id];
                // offset into the previous vertex buffer of the left vertex of the previous segment
                const size_t prev_left_offset = prev_sub_path.last.i_id - 1 * vertex_size_floats;
                // new position of the left vertices
                const Vec3f shared_vertex = extract_position_at(prev_vbuffer, prev_left_offset) + displacement_vec;
                // update previous segment
                update_position_at(prev_vbuffer, prev_left_offset, shared_vertex);
                // offset into the next vertex buffer of the left vertex of the next segment
                const size_t next_left_offset = next_sub_path.first.i_id + 3 * vertex_size_floats;
                // update next segment
                update_position_at(next_vbuffer, next_left_offset, shared_vertex);
            }
        };

        size_t vertex_size_floats = t_buffer.vertices.vertex_size_floats();
        for (const Path& path : t_buffer.paths) {
            //BBS: the two segments of the path sharing the current vertex may belong
            //to two different vertex buffers
            size_t prev_sub_path_id = 0;
            size_t next_sub_path_id = 0;
            const size_t path_vertices_count = path.vertices_count();
            const float half_width = 0.5f * path.width;
            // BBS: modify a lot to support arc move which has internal points
            for (size_t j = 1; j < path_vertices_count; ++j) {
                size_t curr_s_id = path.sub_paths.front().first.s_id + j;
                size_t move_id = m_ssid_to_moveid_map[curr_s_id];
                int interpolation_points_num = gcode_result.moves[move_id].is_arc_move_with_interpolation_points()?
                                                    gcode_result.moves[move_id].interpolation_points.size() : 0;
                int loop_num = interpolation_points_num;
                //BBS: select the subpaths which contains the previous/next segments
                if (!path.sub_paths[prev_sub_path_id].contains(curr_s_id))
                    ++prev_sub_path_id;
                if (j == path_vertices_count - 1) {
                    if (!gcode_result.moves[move_id].is_arc_move_with_interpolation_points())
                        break;   // BBS: the last move has no internal point.
                    loop_num--;  //BBS: don't need to handle the endpoint of the last arc move of path
                    next_sub_path_id = prev_sub_path_id;
                } else {
                    if (!path.sub_paths[next_sub_path_id].contains(curr_s_id + 1))
                        ++next_sub_path_id;
                }
                const Path::Sub_Path& prev_sub_path = path.sub_paths[prev_sub_path_id];
                const Path::Sub_Path& next_sub_path = path.sub_paths[next_sub_path_id];

                // BBS: smooth triangle toolpaths corners including arc move which has internal interpolation point
                for (int k = 0; k <= loop_num; k++) {
                    const Vec3f& prev = k==0?
                                        gcode_result.moves[move_id - 1].position :
                                        gcode_result.moves[move_id].interpolation_points[k-1];
                    const Vec3f& curr = k==interpolation_points_num?
                                        gcode_result.moves[move_id].position :
                                        gcode_result.moves[move_id].interpolation_points[k];
                    const Vec3f& next = k < interpolation_points_num - 1?
                                        gcode_result.moves[move_id].interpolation_points[k+1]:
                                        (k == interpolation_points_num - 1? gcode_result.moves[move_id].position :
                                        (gcode_result.moves[move_id + 1].is_arc_move_with_interpolation_points()?
                                        gcode_result.moves[move_id + 1].interpolation_points[0] :
                                        gcode_result.moves[move_id + 1].position));

                    const Vec3f prev_dir = (curr - prev).normalized();
                    const Vec3f prev_right = Vec3f(prev_dir.y(), -prev_dir.x(), 0.0f).normalized();
                    const Vec3f prev_up = prev_right.cross(prev_dir);

                    const Vec3f next_dir = (next - curr).normalized();

                    const bool is_right_turn = prev_up.dot(prev_dir.cross(next_dir)) <= 0.0f;
                    const float cos_dir = prev_dir.dot(next_dir);
                    // whether the angle between adjacent segments is greater than 45 degrees
                    const bool is_sharp = cos_dir < 0.7071068f;

                    float displacement = 0.0f;
                    if (cos_dir > -0.9998477f) {
                        // if the angle between adjacent segments is smaller than 179 degrees
                        Vec3f med_dir = (prev_dir + next_dir).normalized();
                        displacement = half_width * ::tan(::acos(std::clamp(next_dir.dot(med_dir), -1.0f, 1.0f)));
                    }

                    const float sq_prev_length = (curr - prev).squaredNorm();
                    const float sq_next_length = (next - curr).squaredNorm();
                    const float sq_displacement = sqr(displacement);
                    const bool can_displace = displacement > 0.0f && sq_displacement < sq_prev_length&& sq_displacement < sq_next_length;
                    bool is_internal_point = interpolation_points_num > k;

                    if (can_displace) {
                        // displacement to apply to the vertices to match
                        Vec3f displacement_vec = displacement * prev_dir;
                        // matches inner corner vertices
                        if (is_right_turn)
                            match_right_vertices_with_internal_point(prev_sub_path, next_sub_path, curr_s_id, is_internal_point, k, vertex_size_floats, -displacement_vec);
                        else
                            match_left_vertices_with_internal_point(prev_sub_path, next_sub_path, curr_s_id, is_internal_point, k, vertex_size_floats, -displacement_vec);

                        if (!is_sharp) {
                            //BBS: matches outer corner vertices
                            if (is_right_turn)
                                match_left_vertices_with_internal_point(prev_sub_path, next_sub_path, curr_s_id, is_internal_point, k, vertex_size_floats, displacement_vec);
                            else
                                match_right_vertices_with_internal_point(prev_sub_path, next_sub_path, curr_s_id, is_internal_point, k, vertex_size_floats, displacement_vec);
                        }
                    }
                }
            }
        }
    };

    // smooth toolpaths corners for TBuffers using triangles
    for (size_t i = 0; i < m_buffers.size(); ++i) {
        const TBuffer& t_buffer = m_buffers[i];
        if (t_buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::Triangle) {
            smooth_triangle_toolpaths_corners(t_buffer, vertices[i]);
        }
    }

    // dismiss, no more needed
    std::vector<size_t>().swap(biased_seams_ids);

    for (MultiVertexBuffer& v_multibuffer : vertices) {
        for (VertexBuffer& v_buffer : v_multibuffer) {
            v_buffer.shrink_to_fit();
        }
    }

    // move the wipe toolpaths half height up to render them on proper position
    MultiVertexBuffer& wipe_vertices = vertices[buffer_id(EMoveType::Wipe)];
    for (VertexBuffer& v_buffer : wipe_vertices) {
        for (size_t i = 2; i < v_buffer.size(); i += 3) {
            v_buffer[i] += 0.5f * GCodeProcessor::Wipe_Height;
        }
    }

    // send vertices data to gpu, where needed
    for (size_t i = 0; i < m_buffers.size(); ++i) {
        TBuffer& t_buffer = m_buffers[i];
        if (t_buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::InstancedModel) {
            const InstanceBuffer& inst_buffer = instances[i];
            if (!inst_buffer.empty()) {
                t_buffer.model.instances.buffer = inst_buffer;
                t_buffer.model.instances.s_ids = instances_ids[i];
                t_buffer.model.instances.offsets = instances_offsets[i];
            }
        }
        else {
            if (t_buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::BatchedModel) {
                const InstanceBuffer& inst_buffer = instances[i];
                if (!inst_buffer.empty()) {
                    t_buffer.model.instances.buffer = inst_buffer;
                    t_buffer.model.instances.s_ids = instances_ids[i];
                    t_buffer.model.instances.offsets = instances_offsets[i];
                }
            }
            const MultiVertexBuffer& v_multibuffer = vertices[i];
            const MultiStepBuffer& step_multibuffer = steps[i];

            crslice2::GCodeBlockRenderData& blockRenderData = m_gcodePath.blocks[i];
            blockRenderData.verticeGroups = v_multibuffer;
            blockRenderData.stepInfoGroups = step_multibuffer;
            
            for (const VertexBuffer& v_buffer : v_multibuffer) {
                const size_t size_elements = v_buffer.size();
                const size_t size_bytes = size_elements * sizeof(float);
                const size_t vertices_count = size_elements / t_buffer.vertices.vertex_size_floats();
                t_buffer.vertices.count += vertices_count;

                int id = 0;
                t_buffer.vertices.vbos.push_back(static_cast<unsigned int>(id));
                t_buffer.vertices.sizes.push_back(size_bytes);
            }
        }
    }

    // dismiss vertices data, no more needed
    std::vector<MultiVertexBuffer>().swap(vertices);
    std::vector<InstanceBuffer>().swap(instances);
    std::vector<InstanceIdBuffer>().swap(instances_ids);

    // toolpaths data -> extract indices from result
    // paths may have been filled while extracting vertices,
    // so reset them, they will be filled again while extracting indices
    for (TBuffer& buffer : m_buffers) {
        buffer.paths.clear();
    }

    // variable used to keep track of the current vertex buffers index and size
    using CurrVertexBuffer = std::pair<unsigned int, size_t>;
    std::vector<CurrVertexBuffer> curr_vertex_buffers(m_buffers.size(), { 0, 0 });

    // variable used to keep track of the vertex buffers ids
    using VboIndexList = std::vector<unsigned int>;
    std::vector<VboIndexList> vbo_indices(m_buffers.size());

    seams_count = 0;

    for (size_t i = 0; i < m_moves_count; ++i) {
        const GCodeProcessorResult::MoveVertex& curr = gcode_result.moves[i];
        if (curr.type == EMoveType::Seam)
            ++seams_count;

        size_t move_id = i - seams_count;

        // skip first vertex
        if (i == 0)
            continue;

        const GCodeProcessorResult::MoveVertex& prev = gcode_result.moves[i - 1];
        const GCodeProcessorResult::MoveVertex* next = nullptr;
        if (i < m_moves_count - 1)
            next = &gcode_result.moves[i + 1];

        const unsigned char id = buffer_id(curr.type);
        TBuffer& t_buffer = m_buffers[id];
        MultiIndexBuffer& i_multibuffer = indices[id];
        CurrVertexBuffer& curr_vertex_buffer = curr_vertex_buffers[id];
        VboIndexList& vbo_index_list = vbo_indices[id];

        // ensure there is at least one index buffer
        if (i_multibuffer.empty()) {
            i_multibuffer.push_back(IndexBuffer());
            if (!t_buffer.vertices.vbos.empty())
                vbo_index_list.push_back(t_buffer.vertices.vbos[curr_vertex_buffer.first]);
        }

        // if adding the indices for the current segment exceeds the threshold size of the current index buffer
        // create another index buffer
        // BBS: get the point number and then judge whether the remaining buffer is enough
        size_t points_num = curr.is_arc_move_with_interpolation_points() ? curr.interpolation_points.size() + 1 : 1;
        size_t indiced_size_to_add = (t_buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::BatchedModel) ? t_buffer.model.data.indices_size_bytes() : points_num * t_buffer.max_indices_per_segment_size_bytes();
        if (i_multibuffer.back().size() * sizeof(IBufferType) >= IBUFFER_THRESHOLD_BYTES - indiced_size_to_add) {
            i_multibuffer.push_back(IndexBuffer());
            vbo_index_list.push_back(t_buffer.vertices.vbos[curr_vertex_buffer.first]);
            if (t_buffer.render_primitive_type != TBuffer::ERenderPrimitiveType::BatchedModel) {
                Path& last_path = t_buffer.paths.back();
                last_path.add_sub_path(prev, static_cast<unsigned int>(i_multibuffer.size()) - 1, 0, move_id - 1);
            }
        }

        // if adding the vertices for the current segment exceeds the threshold size of the current vertex buffer
        // create another index buffer
        // BBS: support multi points in one MoveVertice, should multiply point number
        size_t vertices_size_to_add = (t_buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::BatchedModel) ? t_buffer.model.data.vertices_size_bytes() : points_num * t_buffer.max_vertices_per_segment_size_bytes();
        if (curr_vertex_buffer.second * t_buffer.vertices.vertex_size_bytes() > t_buffer.vertices.max_size_bytes() - vertices_size_to_add) {
            i_multibuffer.push_back(IndexBuffer());

            ++curr_vertex_buffer.first;
            curr_vertex_buffer.second = 0;
            vbo_index_list.push_back(t_buffer.vertices.vbos[curr_vertex_buffer.first]);

            if (t_buffer.render_primitive_type != TBuffer::ERenderPrimitiveType::BatchedModel) {
                Path& last_path = t_buffer.paths.back();
                last_path.add_sub_path(prev, static_cast<unsigned int>(i_multibuffer.size()) - 1, 0, move_id - 1);
            }
        }

        IndexBuffer& i_buffer = i_multibuffer.back();

        switch (t_buffer.render_primitive_type)
        {
        case TBuffer::ERenderPrimitiveType::Line: {
            unsigned int bufferId = static_cast<unsigned int>(i_multibuffer.size()) - 1;
            add_indices_as_line(prev, curr, t_buffer, curr_vertex_buffer.second, static_cast<unsigned int>(i_multibuffer.size()) - 1, i_buffer, move_id);
            break;
        }
        case TBuffer::ERenderPrimitiveType::Triangle: {
            unsigned int bufferId = static_cast<unsigned int>(i_multibuffer.size()) - 1;
            add_indices_as_solid(prev, curr, next, t_buffer, curr_vertex_buffer.second, static_cast<unsigned int>(i_multibuffer.size()) - 1, i_buffer, move_id);
            break;
        }
        case TBuffer::ERenderPrimitiveType::BatchedModel: {
            add_indices_as_model_batch(t_buffer.model.data, i_buffer, curr_vertex_buffer.second);
            curr_vertex_buffer.second += t_buffer.model.data.vertices_count();
            break;
        }
        default: { break; }
        }
    }

    for (MultiIndexBuffer& i_multibuffer : indices) {
        for (IndexBuffer& i_buffer : i_multibuffer) {
            i_buffer.shrink_to_fit();
        }
    }

    // toolpaths data -> send indices data to gpu
    for (size_t i = 0; i < m_buffers.size(); ++i) {
        TBuffer& t_buffer = m_buffers[i];
        if (t_buffer.render_primitive_type != TBuffer::ERenderPrimitiveType::InstancedModel) {
            const MultiIndexBuffer& i_multibuffer = indices[i];

             crslice2::GCodeBlockRenderData& blockRenderData = m_gcodePath.blocks[i];
             blockRenderData.indexGroups = i_multibuffer;

            for (const IndexBuffer& i_buffer : i_multibuffer) {
                const size_t size_elements = i_buffer.size();
                const size_t size_bytes = size_elements * sizeof(IBufferType);

                // stores index buffer informations into TBuffer
                t_buffer.indices.push_back(IBuffer());
                IBuffer& ibuf = t_buffer.indices.back();
                ibuf.count = size_elements;
                ibuf.vbo = vbo_indices[i][t_buffer.indices.size() - 1];

            }
        }
    }

    // dismiss indices data, no more needed
    std::vector<MultiIndexBuffer>().swap(indices);

    // layers zs / roles / extruder ids -> extract from result
    size_t last_travel_s_id = 0;
    seams_count = 0;
    for (size_t i = 0; i < m_moves_count; ++i) {
        const GCodeProcessorResult::MoveVertex& move = gcode_result.moves[i];
        if (move.type == EMoveType::Seam)
            ++seams_count;

        size_t move_id = i - seams_count;

        if (move.type == EMoveType::Extrude) {
            // layers zs
            const double* const last_z = m_layers.empty() ? nullptr : &m_layers.get_zs().back();
            const double z = static_cast<double>(move.position.z());
            if (last_z == nullptr || z < *last_z - EPSILON || *last_z + EPSILON < z)
                m_layers.append(z, { last_travel_s_id, move_id });
            else
                m_layers.get_endpoints().back().last = move_id;
            // extruder ids
            m_extruder_ids.emplace_back(move.extruder_id);
            // roles
            if (i > 0)
                m_roles.emplace_back(move.extrusion_role);
        }
        else if (move.type == EMoveType::Travel) {
            if (move_id - last_travel_s_id > 1 && !m_layers.empty())
                m_layers.get_endpoints().back().last = move_id;

            last_travel_s_id = move_id;
        }
    }

    // roles -> remove duplicates
    sort_remove_duplicates(m_roles);
    m_roles.shrink_to_fit();

    // extruder ids -> remove duplicates
    sort_remove_duplicates(m_extruder_ids);
    m_extruder_ids.shrink_to_fit();

    std::vector<int> plater_extruder;
	for (auto mid : m_extruder_ids){
        int eid = mid;
        plater_extruder.push_back(++eid);
	}
    m_plater_extruder = plater_extruder;

    // replace layers for spiral vase mode
    if (!gcode_result.spiral_vase_layers.empty()) {
        m_layers.reset();
        for (const auto& layer : gcode_result.spiral_vase_layers) {
            m_layers.append(layer.first, { layer.second.first, layer.second.second});
        }
    }

    const std::vector<Layers::Endpoints>&layers = m_layers.get_endpoints();
    for (const Layers::Endpoints& layer : layers)
    {
        crslice2::GCodeLayerInfo layerInfo;

        layerInfo.first = layer.first;
        layerInfo.last = layer.last;
        layerInfo.stepCount = layer.last - layer.first + 1;
        
        m_gcodePath.layers.push_back(layerInfo);
    }

    // set layers z range
    if (!m_layers.empty())
        m_layers_z_range = { 0, static_cast<unsigned int>(m_layers.size() - 1) };

    // change color of paths whose layer contains option points
    if (!options_zs.empty()) {
        TBuffer& extrude_buffer = m_buffers[buffer_id(EMoveType::Extrude)];
        for (Path& path : extrude_buffer.paths) {
            const float z = path.sub_paths.front().first.position.z();
            if (std::find_if(options_zs.begin(), options_zs.end(), [z](float f) { return f - EPSILON <= z && z <= f + EPSILON; }) != options_zs.end())
                path.cp_color_id = 255 - path.cp_color_id;
        }
    }
}

void GCodeRenderGenerator::refresh_render_paths(bool keep_sequential_current_first, bool keep_sequential_current_last) const
{
    BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << boost::format(": enter, m_buffers size %1%!") % m_buffers.size();
    auto extrusion_color = [this](const Path& path) {
        ColorRGBA color;
        switch (m_view_type)
        {
        case EViewType::FeatureType: { color = Extrusion_Role_Colors[static_cast<unsigned int>(path.role)]; break; }
        case EViewType::Height: { color = m_extrusions.ranges.height.get_color_at(path.height); break; }
        case EViewType::Width: { color = m_extrusions.ranges.width.get_color_at(path.width); break; }
        case EViewType::Feedrate: { color = m_extrusions.ranges.feedrate.get_color_at(path.feedrate); break; }
        case EViewType::FanSpeed: { color = m_extrusions.ranges.fan_speed.get_color_at(path.fan_speed); break; }
        case EViewType::Temperature: { color = m_extrusions.ranges.temperature.get_color_at(path.temperature); break; }
        case EViewType::LayerTime: { color = m_extrusions.ranges.layer_duration.get_color_at(path.layer_time); break; }
        case EViewType::LayerTimeLog: { color = m_extrusions.ranges.layer_duration_log.get_color_at(path.layer_time); break; }
        case EViewType::VolumetricRate: { color = m_extrusions.ranges.volumetric_rate.get_color_at(path.volumetric_rate); break; }
        case EViewType::Tool: { color = m_tools.m_tool_colors[path.extruder_id]; break; }
        case EViewType::ColorPrint: {
            if (path.cp_color_id >= static_cast<unsigned char>(m_tools.m_tool_colors.size()))
                color = ColorRGBA::GRAY();
            else {
                color = m_tools.m_tool_colors[path.cp_color_id];
                //color = adjust_color_for_rendering(color);
            }
            break;
        }
        case EViewType::FilamentId: {
            float id = float(path.extruder_id) / 256;
            float role = float(path.role) / 256;
            color = { id, role, id, 1.0f };
            break;
        }
        default: { color = ColorRGBA::WHITE(); break; }
        }

        return color;
    };

    auto travel_color = [](const Path& path) {
        return (path.delta_extruder < 0.0f) ? Travel_Colors[2] /* Retract */ :
            ((path.delta_extruder > 0.0f) ? Travel_Colors[1] /* Extrude */ :
                Travel_Colors[0] /* Move */);
    };

    auto is_in_layers_range = [this](const Path& path, size_t min_id, size_t max_id) {
        auto in_layers_range = [this, min_id, max_id](size_t id) {
            return m_layers.get_endpoints_at(min_id).first <= id && id <= m_layers.get_endpoints_at(max_id).last;
        };

        return in_layers_range(path.sub_paths.front().first.s_id) && in_layers_range(path.sub_paths.back().last.s_id);
    };

    //BBS
    auto is_extruder_in_layer_range = [this](const Path& path, size_t extruder_id) {
        return path.extruder_id == extruder_id;
    };


    auto is_travel_in_layers_range = [this](size_t path_id, size_t min_id, size_t max_id) {
        const TBuffer& buffer = m_buffers[buffer_id(EMoveType::Travel)];
        if (path_id >= buffer.paths.size())
            return false;

        Path path = buffer.paths[path_id];
        size_t first = path_id;
        size_t last = path_id;

        // check adjacent paths
        while (first > 0 && path.sub_paths.front().first.position.isApprox(buffer.paths[first - 1].sub_paths.back().last.position)) {
            --first;
            path.sub_paths.front().first = buffer.paths[first].sub_paths.front().first;
        }
        while (last < buffer.paths.size() - 1 && path.sub_paths.back().last.position.isApprox(buffer.paths[last + 1].sub_paths.front().first.position)) {
            ++last;
            path.sub_paths.back().last = buffer.paths[last].sub_paths.back().last;
        }

        const size_t min_s_id = m_layers.get_endpoints_at(min_id).first;
        const size_t max_s_id = m_layers.get_endpoints_at(max_id).last;

        return (min_s_id <= path.sub_paths.front().first.s_id && path.sub_paths.front().first.s_id <= max_s_id) ||
            (min_s_id <= path.sub_paths.back().last.s_id && path.sub_paths.back().last.s_id <= max_s_id);
    };

    const bool top_layer_only = true;

    //BBS
    SequentialView::Endpoints global_endpoints = { m_sequential_view.gcode_ids.size() , 0 };
    SequentialView::Endpoints top_layer_endpoints = global_endpoints;
    SequentialView* sequential_view = const_cast<SequentialView*>(&m_sequential_view);
    if (top_layer_only || !keep_sequential_current_first) sequential_view->current.first = 0;
    //BBS
    if (!keep_sequential_current_last) sequential_view->current.last = m_sequential_view.gcode_ids.size();

    // first pass: collect visible paths and update sequential view data
    std::vector<std::tuple<unsigned char, unsigned int, unsigned int, unsigned int>> paths;

    for (size_t b = 0; b < m_buffers.size(); ++b) {
        TBuffer& buffer = const_cast<TBuffer&>(m_buffers[b]);
        buffer.visible = true;
        // reset render paths
        buffer.render_paths.clear();

        if (!buffer.visible)
            continue;

        if (buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::InstancedModel ||
            buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::BatchedModel) {
            for (size_t id : buffer.model.instances.s_ids) {
                if (id < m_layers.get_endpoints_at(m_layers_z_range[0]).first || m_layers.get_endpoints_at(m_layers_z_range[1]).last < id)
                    continue;

                global_endpoints.first = std::min(global_endpoints.first, id);
                global_endpoints.last = std::max(global_endpoints.last, id);

                if (top_layer_only) {
                    if (id < m_layers.get_endpoints_at(m_layers_z_range[1]).first || m_layers.get_endpoints_at(m_layers_z_range[1]).last < id)
                        continue;

                    top_layer_endpoints.first = std::min(top_layer_endpoints.first, id);
                    top_layer_endpoints.last = std::max(top_layer_endpoints.last, id);
                }
            }
        }
        else {
            for (size_t i = 0; i < buffer.paths.size(); ++i) {
                const Path& path = buffer.paths[i];
                if (path.type == EMoveType::Travel) {
                    if (!is_travel_in_layers_range(i, m_layers_z_range[0], m_layers_z_range[1]))
                        continue;
                }
                else if (!is_in_layers_range(path, m_layers_z_range[0], m_layers_z_range[1]))
                    continue;

                if (path.type == EMoveType::Extrude)
                    continue;

                if (m_view_type == EViewType::ColorPrint && !m_tools.m_tool_visibles[path.extruder_id])
                    continue;

                // store valid path
                for (size_t j = 0; j < path.sub_paths.size(); ++j) {
                    paths.push_back({ static_cast<unsigned char>(b), path.sub_paths[j].first.b_id, static_cast<unsigned int>(i), static_cast<unsigned int>(j) });
                }

                global_endpoints.first = std::min(global_endpoints.first, path.sub_paths.front().first.s_id);
                global_endpoints.last = std::max(global_endpoints.last, path.sub_paths.back().last.s_id);

                if (top_layer_only) {
                    if (path.type == EMoveType::Travel) {
                        if (is_travel_in_layers_range(i, m_layers_z_range[1], m_layers_z_range[1])) {
                            top_layer_endpoints.first = std::min(top_layer_endpoints.first, path.sub_paths.front().first.s_id);
                            top_layer_endpoints.last = std::max(top_layer_endpoints.last, path.sub_paths.back().last.s_id);
                        }
                    }
                    else if (is_in_layers_range(path, m_layers_z_range[1], m_layers_z_range[1])) {
                        top_layer_endpoints.first = std::min(top_layer_endpoints.first, path.sub_paths.front().first.s_id);
                        top_layer_endpoints.last = std::max(top_layer_endpoints.last, path.sub_paths.back().last.s_id);
                    }
                }
            }
        }
    }

    // update current sequential position
    sequential_view->current.first = !top_layer_only && keep_sequential_current_first ? std::clamp(sequential_view->current.first, global_endpoints.first, global_endpoints.last) : global_endpoints.first;
    if (global_endpoints.last == 0) {
        m_no_render_path = true;
        sequential_view->current.last = global_endpoints.last;
    }
    else {
        m_no_render_path = false;
        sequential_view->current.last = keep_sequential_current_last ? std::clamp(sequential_view->current.last, global_endpoints.first, global_endpoints.last) : global_endpoints.last;
    }

    // get the world position from the vertex buffer
    bool found = false;
    for (const TBuffer& buffer : m_buffers) {
        if (buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::InstancedModel ||
            buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::BatchedModel) {
            for (size_t i = 0; i < buffer.model.instances.s_ids.size(); ++i) {
                if (buffer.model.instances.s_ids[i] == m_sequential_view.current.last) {
                    size_t offset = i * buffer.model.instances.instance_size_floats();
                    sequential_view->current_position.x() = buffer.model.instances.buffer[offset + 0];
                    sequential_view->current_position.y() = buffer.model.instances.buffer[offset + 1];
                    sequential_view->current_position.z() = buffer.model.instances.buffer[offset + 2];
                    sequential_view->current_offset = buffer.model.instances.offsets[i];
                    found = true;
                    break;
                }
            }
        }
        else {
            // searches the path containing the current position
            for (const Path& path : buffer.paths) {
                if (path.contains(m_sequential_view.current.last)) {
                    const int sub_path_id = path.get_id_of_sub_path_containing(m_sequential_view.current.last);
                    if (sub_path_id != -1) {
                        const Path::Sub_Path& sub_path = path.sub_paths[sub_path_id];
                        unsigned int offset = static_cast<unsigned int>(m_sequential_view.current.last - sub_path.first.s_id);
                        if (offset > 0) {
                            if (buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::Line) {
                                for (size_t i = sub_path.first.s_id + 1; i < m_sequential_view.current.last + 1; i++) {
                                    size_t move_id = m_ssid_to_moveid_map[i];
                                    const GCodeProcessorResult::MoveVertex& curr = m_gcode_result->moves[move_id];
                                    if (curr.is_arc_move()) {
                                        offset += curr.interpolation_points.size();
                                    }
                                }
                                offset = 2 * offset - 1;
                            }
                            else if (buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::Triangle) {
                                unsigned int indices_count = buffer.indices_per_segment();
                                // BBS: modify to support moves which has internal point
                                for (size_t i = sub_path.first.s_id + 1; i < m_sequential_view.current.last + 1; i++) {
                                    size_t move_id = m_ssid_to_moveid_map[i];
                                    const GCodeProcessorResult::MoveVertex& curr = m_gcode_result->moves[move_id];
                                    if (curr.is_arc_move()) {
                                        offset += curr.interpolation_points.size();
                                    }
                                }
                                offset = indices_count * (offset - 1) + (indices_count - 2);
                                if (sub_path_id == 0)
                                    offset += 6; // add 2 triangles for starting cap
                            }
                        }
                        offset += static_cast<unsigned int>(sub_path.first.i_id);

                        // gets the vertex index from the index buffer on gpu
                        const IBuffer& i_buffer = buffer.indices[sub_path.first.b_id];
                        unsigned int index = 0;
                        sequential_view->current_offset = Vec3f::Zero();
                        found = true;
                        break;
                    }
                }
            }
        }

        if (found)
            break;
    }

    // second pass: filter paths by sequential data and collect them by color
    RenderPath* render_path = nullptr;
    for (const auto& [tbuffer_id, ibuffer_id, path_id, sub_path_id] : paths) {
        TBuffer& buffer = const_cast<TBuffer&>(m_buffers[tbuffer_id]);
        const Path& path = buffer.paths[path_id];
        const Path::Sub_Path& sub_path = path.sub_paths[sub_path_id];
        if (m_sequential_view.current.last < sub_path.first.s_id || sub_path.last.s_id < m_sequential_view.current.first)
            continue;

        ColorRGBA color;
        switch (path.type)
        {
        case EMoveType::Tool_change:
        case EMoveType::Color_change:
        case EMoveType::Pause_Print:
        case EMoveType::Custom_GCode:
        case EMoveType::Retract:
        case EMoveType::Unretract:
        case EMoveType::Seam: break;
        case EMoveType::Extrude: {
            if (!top_layer_only ||
                m_sequential_view.current.last == global_endpoints.last ||
                is_in_layers_range(path, m_layers_z_range[1], m_layers_z_range[1]))
                color = extrusion_color(path);
            else
                color = Neutral_Color;

            break;
        }
        case EMoveType::Travel: {
            if (!top_layer_only || m_sequential_view.current.last == global_endpoints.last || is_travel_in_layers_range(path_id, m_layers_z_range[1], m_layers_z_range[1]))
                color = (m_view_type == EViewType::Feedrate || m_view_type == EViewType::Tool) ? extrusion_color(path) : travel_color(path);
            else
                color = Neutral_Color;

            break;
        }
        case EMoveType::Wipe: { color = Wipe_Color; break; }
        default: { color = { 0.0f, 0.0f, 0.0f, 1.0f }; break; }
        }

        RenderPath key{ tbuffer_id, color, static_cast<unsigned int>(ibuffer_id), path_id };
        if (render_path == nullptr || !RenderPathPropertyEqual()(*render_path, key)) {
            buffer.render_paths.emplace_back(key);
            render_path = const_cast<RenderPath*>(&buffer.render_paths.back());
        }

        unsigned int delta_1st = 0;
        if (sub_path.first.s_id < m_sequential_view.current.first && m_sequential_view.current.first <= sub_path.last.s_id)
            delta_1st = static_cast<unsigned int>(m_sequential_view.current.first - sub_path.first.s_id);

        unsigned int size_in_indices = 0;
        switch (buffer.render_primitive_type)
        {
        case TBuffer::ERenderPrimitiveType::Line:
        case TBuffer::ERenderPrimitiveType::Triangle: {
            // BBS: modify to support moves which has internal point
            size_t max_s_id = std::min(m_sequential_view.current.last, sub_path.last.s_id);
            size_t min_s_id = std::max(m_sequential_view.current.first, sub_path.first.s_id);
            unsigned int segments_count = max_s_id - min_s_id;
            for (size_t i = min_s_id + 1; i < max_s_id + 1; i++) {
                size_t move_id = m_ssid_to_moveid_map[i];
                const GCodeProcessorResult::MoveVertex& curr = m_gcode_result->moves[move_id];
                if (curr.is_arc_move()) {
                    segments_count += curr.interpolation_points.size();
                }
            }
            size_in_indices = buffer.indices_per_segment() * segments_count;
            break;
        }
        default: { break; }
        }

        if (size_in_indices == 0)
            continue;

        if (buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::Triangle) {
            if (sub_path_id == 0 && delta_1st == 0)
                size_in_indices += 6; // add 2 triangles for starting cap
            if (sub_path_id == path.sub_paths.size() - 1 && path.sub_paths.back().last.s_id <= m_sequential_view.current.last)
                size_in_indices += 6; // add 2 triangles for ending cap
            if (delta_1st > 0)
                size_in_indices -= 6; // remove 2 triangles for corner cap
        }

        render_path->sizes.push_back(size_in_indices);

        if (buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::Triangle) {
            delta_1st *= buffer.indices_per_segment();
            if (delta_1st > 0) {
                delta_1st += 6; // skip 2 triangles for corner cap
                if (sub_path_id == 0)
                    delta_1st += 6; // skip 2 triangles for starting cap
            }
        }

        render_path->offsets.push_back(static_cast<size_t>((sub_path.first.i_id + delta_1st) * sizeof(IBufferType)));

        // Removes empty render paths and sort.
        for (size_t b = 0; b < m_buffers.size(); ++b) {
            TBuffer* buffer = const_cast<TBuffer*>(&m_buffers[b]);
            buffer->render_paths.erase(std::remove_if(buffer->render_paths.begin(), buffer->render_paths.end(),
                [](const auto& path) { return path.sizes.empty() || path.offsets.empty(); }),
                buffer->render_paths.end());
        }

        // second pass: for buffers using instanced and batched models, update the instances render ranges
        for (size_t b = 0; b < m_buffers.size(); ++b) {
            TBuffer& buffer = const_cast<TBuffer&>(m_buffers[b]);
            if (buffer.render_primitive_type != TBuffer::ERenderPrimitiveType::InstancedModel &&
                buffer.render_primitive_type != TBuffer::ERenderPrimitiveType::BatchedModel)
                continue;

            buffer.model.instances.render_ranges.reset();

            if (!buffer.visible || buffer.model.instances.s_ids.empty())
                continue;

            buffer.model.instances.render_ranges.ranges.push_back({ 0, 0, 0, buffer.model.color });
            bool has_second_range = top_layer_only && m_sequential_view.current.last != m_sequential_view.global.last;
            if (has_second_range)
                buffer.model.instances.render_ranges.ranges.push_back({ 0, 0, 0, Neutral_Color });

            if (m_sequential_view.current.first <= buffer.model.instances.s_ids.back() && buffer.model.instances.s_ids.front() <= m_sequential_view.current.last) {
                for (size_t id : buffer.model.instances.s_ids) {
                    if (has_second_range) {
                        if (id < m_sequential_view.endpoints.first) {
                            ++buffer.model.instances.render_ranges.ranges.front().offset;
                            if (id <= m_sequential_view.current.first)
                                ++buffer.model.instances.render_ranges.ranges.back().offset;
                            else
                                ++buffer.model.instances.render_ranges.ranges.back().count;
                        }
                        else if (id <= m_sequential_view.current.last)
                            ++buffer.model.instances.render_ranges.ranges.front().count;
                        else
                            break;
                    }
                    else {
                        if (id <= m_sequential_view.current.first)
                            ++buffer.model.instances.render_ranges.ranges.front().offset;
                        else if (id <= m_sequential_view.current.last)
                            ++buffer.model.instances.render_ranges.ranges.front().count;
                        else
                            break;
                    }
                }
            }
        }

        // set sequential data to their final value
        sequential_view->endpoints = top_layer_only ? top_layer_endpoints : global_endpoints;
        sequential_view->current.first = !top_layer_only && keep_sequential_current_first ? std::clamp(sequential_view->current.first, sequential_view->endpoints.first, sequential_view->endpoints.last) : sequential_view->endpoints.first;
        sequential_view->global = global_endpoints;

        // updates sequential range caps
        std::array<SequentialRangeCap, 2>* sequential_range_caps = const_cast<std::array<SequentialRangeCap, 2>*>(&m_sequential_range_caps);
        (*sequential_range_caps)[0].reset();
        (*sequential_range_caps)[1].reset();

        if (m_sequential_view.current.first != m_sequential_view.current.last) {
            for (const auto& [tbuffer_id, ibuffer_id, path_id, sub_path_id] : paths) {
                TBuffer& buffer = const_cast<TBuffer&>(m_buffers[tbuffer_id]);
                if (buffer.render_primitive_type != TBuffer::ERenderPrimitiveType::Triangle)
                    continue;

                const Path& path = buffer.paths[path_id];
                const Path::Sub_Path& sub_path = path.sub_paths[sub_path_id];
                if (m_sequential_view.current.last <= sub_path.first.s_id || sub_path.last.s_id <= m_sequential_view.current.first)
                    continue;

                // update cap for first endpoint of current range
                if (m_sequential_view.current.first > sub_path.first.s_id) {
                    SequentialRangeCap& cap = (*sequential_range_caps)[0];
                    const IBuffer& i_buffer = buffer.indices[ibuffer_id];

                    // calculate offset into the index buffer
                    unsigned int offset = sub_path.first.i_id;
                    offset += 6; // add 2 triangles for corner cap
                    offset += static_cast<unsigned int>(m_sequential_view.current.first - sub_path.first.s_id) * buffer.indices_per_segment();
                    if (sub_path_id == 0)
                        offset += 6; // add 2 triangles for starting cap

                    // extract indices from index buffer
                    std::array<IBufferType, 6> indices{ 0, 0, 0, 0, 0, 0 };
                    indices[3] = indices[0];
                    indices[5] = indices[1];
                }

                // update cap for last endpoint of current range
                if (m_sequential_view.current.last < sub_path.last.s_id) {
                    const SequentialRangeCap& cap = (*sequential_range_caps)[1];
                    const IBuffer& i_buffer = buffer.indices[ibuffer_id];

                    // calculate offset into the index buffer
                    unsigned int offset = sub_path.first.i_id;
                    offset += 6; // add 2 triangles for corner cap
                    offset += static_cast<unsigned int>(m_sequential_view.current.last - 1 - sub_path.first.s_id) * buffer.indices_per_segment();
                    if (sub_path_id == 0)
                        offset += 6; // add 2 triangles for starting cap

                    // extract indices from index buffer
                    std::array<IBufferType, 6> indices{ 0, 0, 0, 0, 0, 0 };
                }

                if ((*sequential_range_caps)[0].is_renderable() && (*sequential_range_caps)[1].is_renderable())
                    break;
            }
        }
    }
}

ColorRGBA GCodeRenderGenerator::option_color(EMoveType move_type) const
{
    switch (move_type)
    {
    case EMoveType::Tool_change:  { return Options_Colors[static_cast<unsigned int>(EOptionsColors::ToolChanges)]; }
    case EMoveType::Color_change: { return Options_Colors[static_cast<unsigned int>(EOptionsColors::ColorChanges)]; }
    case EMoveType::Pause_Print:  { return Options_Colors[static_cast<unsigned int>(EOptionsColors::PausePrints)]; }
    case EMoveType::Custom_GCode: { return Options_Colors[static_cast<unsigned int>(EOptionsColors::CustomGCodes)]; }
    case EMoveType::Retract:      { return Options_Colors[static_cast<unsigned int>(EOptionsColors::Retractions)]; }
    case EMoveType::Unretract:    { return Options_Colors[static_cast<unsigned int>(EOptionsColors::Unretractions)]; }
    case EMoveType::Seam:         { return Options_Colors[static_cast<unsigned int>(EOptionsColors::Seams)]; }
    default:                      { return { 0.0f, 0.0f, 0.0f, 1.0f }; }
    }
}

} // namespace GUI
} // namespace Slic3r


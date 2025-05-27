#ifndef GCODE_PROCESSOR_H_2
#define GCODE_PROCESSOR_H_2
#include "crslice2/interface.h"
#include <string>
#include <vector>

namespace crslice2
{
    class GCodePath;

    CRSLICE2_API void process_file(const std::string& file, std::vector<std::vector<std::pair<int, float>>>& times);
    
    CRSLICE2_API void process_file(const std::string& file, GCodePath& gcodePath);
}
#endif  // GCODE_PROCESSOR_H_2

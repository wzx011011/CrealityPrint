///|/ Copyright (c) Prusa Research 2023 Vojtěch Bubník @bubnikv
///|/
///|/ PrusaSlicer is released under the terms of the AGPLv3 or higher
///|/
#include "Timer.hpp"
#include "libslic3r.h"

using namespace std::chrono;

Slic3r::Timer::Timer(const std::string &name) : m_name(name), m_start(steady_clock::now()) {}

Slic3r::Timer::~Timer()
{
    BOOST_LOG_TRIVIAL(debug) << "Timer '" << m_name << "' spend " << 
        duration_cast<milliseconds>(steady_clock::now() - m_start).count() << "ms";
}


namespace Slic3r::Timing {

void TimeLimitAlarm::report_time_exceeded() const {
    BOOST_LOG_TRIVIAL(error) << "Time limit exceeded for " << m_limit_exceeded_message << ": " << m_timer.elapsed_seconds() << "s";
}

}

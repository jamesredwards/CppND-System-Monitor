#ifndef FORMAT_H
#define FORMAT_H

#include <chrono>
#include <string>

namespace Format {
std::string FormatBytes(std::string kb);
std::string ElapsedTime(long times);
std::string FormatUnit(long unit);  // TODO: See src/format.cpp
};                                  // namespace Format
                                    // namespace Format

#endif
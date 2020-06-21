#include "format.h"

#include <sstream>
#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long hours = seconds / 3600;
  seconds = seconds % 3600;
  long minutes = seconds / 60;
  seconds = seconds % 60;
  string zero = "0";

  std::ostringstream s;
  s << FormatUnit(hours) << ":" << FormatUnit(minutes) << ":"
    << FormatUnit(seconds);
  return s.str();
}

string Format::FormatUnit(long unit) {
  std::ostringstream s;
  if (unit < 10) {
    s << "0" << unit;
  } else {
    s << unit;
  }
  return s.str();
}

string Format::FormatBytes(string kb) {
  long kilobytes = std::stol(kb);
  if (kilobytes >= 1000 && kilobytes < 1000000) {
    return std::to_string(kilobytes / 1000);
  } else if (kilobytes >= 1000000) {
    return std::to_string(kilobytes / 1000000);
  } else {
    return kb;
  }
}
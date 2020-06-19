#include "processor.h"

#include <unistd.h>

#include <iostream>

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  usleep(1000000);
  long total_now, active_now, delta_total, delta_active = 0;

  active_now = LinuxParser::ActiveJiffies();
  total_now = LinuxParser::Jiffies();

  delta_total = total_now - start_total_jiffies;
  delta_active = active_now - start_active_jiffies;
  return float(delta_active) / float(delta_total);
}

Processor::Processor() {
  start_total_jiffies = LinuxParser::Jiffies();
  start_active_jiffies = LinuxParser::ActiveJiffies();
}
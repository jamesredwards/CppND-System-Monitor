#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  float util = LinuxParser::ProcessCpuUtilization(pid_);
  return util;
}

// TODO: Return the command that generated this process
string Process::Command() { return cmd_; }

// TODO: Return this process's memory utilization
string Process::Ram() {
  ram_ = LinuxParser::Ram(pid_);
  return ram_;
}

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  uptime_ = LinuxParser::UpTime(pid_);
  return uptime_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return (this->pid_ < a.pid_);
}

Process::Process(int pid) {
  pid_ = pid;
  user_ = LinuxParser::User(pid_);
  cmd_ = LinuxParser::Command(pid_);
  ram_ = LinuxParser::Ram(pid_);
  uptime_init_ = LinuxParser::UpTime(pid_);
  system_uptime_ = LinuxParser::Jiffies();
}
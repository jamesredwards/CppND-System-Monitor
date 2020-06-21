#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <filesystem>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "format.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;
namespace fs = std::filesystem;  // TODO

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  // added version per the video
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  // want MemTotal and MemAvailable
  float value, mem_total, mem_avail = 0.0f;
  string key, line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      // std::cout << key << " " << value << std::endl;
      if (key == "MemTotal:") {
        mem_total = value;
      } else if (key == "MemAvailable:") {
        mem_avail = value;
      }
    }
  }
  if (mem_total != 0.0) {
    return ((mem_total - mem_avail) / mem_total);
  }
  return 0.0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  /* from man proc
       /proc/uptime
              This  file  contains two numbers (values in seconds): the uptime
     of the system (including time spent in suspend) and the amount of time
     spent in the idle process.
  */
  float uptime, idle = 0.0f;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> uptime >> idle;
    }
  }
  return (long)uptime;  // just cast it to long
}

// Utility function to parse /proc/stat file
std::vector<long> LinuxParser::ParseProcStat(string key) {
  vector<long> data;
  string line, key_str, val_str;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      vector<string> info(std::istream_iterator<std::string>{linestream},
                          std::istream_iterator<std::string>());
      if (info.size() > 0) {
        if (info[0] == key) {
          // discard first element which is the key
          for (size_t i = 1; i < info.size(); i++) {
            data.push_back(stol(info[i]));
          }
          return data;
        }
      }
    }
  }
  return data;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  /* from
https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  // Guest time is already accounted in usertime
usertime = usertime - guest;
#As you see here, it subtracts guest from user time
nicetime = nicetime - guestnice;
#and guest_nice from nice time
      // Fields existing on kernels >= 2.6
      // (and RHEL's patched kernel 2.4...)
idlealltime = idletime + ioWait;
#ioWait is added in the idleTime
systemalltime = systemtime + irq + softIrq;
virtalltime = guest + guestnice;
totaltime = usertime + nicetime + systemalltime + idlealltime + steal +
virtalltime;
      */
  vector<string> cpu_jiffies_str = CpuUtilization();
  long totaltime = 0;
  for (size_t i = 0; i < cpu_jiffies_str.size(); i++) {
    totaltime += stol(cpu_jiffies_str[i]);
  }

  return totaltime;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  long active_jiffies = 0;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      vector<string> info(std::istream_iterator<std::string>{linestream},
                          std::istream_iterator<std::string>());

      if (info.size() > 0) {
        // elements 13:16 + 18
        for (size_t i = 13; i < 17; i++) {
          active_jiffies += stol(info[i]);
        }
        active_jiffies += stol(info[18]);
      }
    }
  }
  return active_jiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return Jiffies() - IdleJiffies(); }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<long> data = ParseProcStat("cpu");
  long sum = data[kIdle_] + data[kIOwait_];
  return sum;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> data;
  string line, key_str, val_str;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      vector<string> info(std::istream_iterator<std::string>{linestream},
                          std::istream_iterator<std::string>());
      if (info.size() > 0) {
        if (info[0] == "cpu") {
          // discard first element which is the key
          for (size_t i = 1; i < info.size(); i++) {
            data.push_back(info[i]);
          }
          return data;
        }
      }
    }
  }
  return data;
}

float LinuxParser::ProcessCpuUtilization(int pid) {
  float util = 0.0;
  string line, pid_s, comm, state, ppid, pgrp, session, tty_nr, tpgid, flags,
      minflt, cminflt, majflt, cmajflt, utime, stime, cutime, cstime, priority,
      nice, num_threads, itrealvalue, starttime;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> pid_s >> comm >> state >> ppid >> pgrp >> session >> tty_nr >>
        tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime >>
        stime >> cutime >> cstime >> priority >> nice >> num_threads >>
        itrealvalue >> starttime;

    long total_time = stol(utime) + stol(stime);

    total_time += stol(cutime) + stol(cstime);

    float seconds = float(UpTime()) - (stof(starttime) / sysconf(_SC_CLK_TCK));

    util = ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
  }
  return util;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int processes = 1;
  vector<long> data = ParseProcStat("processes");
  processes = (int)data[0];  // should only be 1 entry
  return processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int processes = 1;
  vector<long> data = ParseProcStat("procs_running");
  processes = (int)data[0];  // should only be 1 entry
  return processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string ram, line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        ram = Format::FormatBytes(value);
      }
    }
  }
  return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string uid;
  string line, item, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> item >> value;
      if (item == "Uid:") {
        uid = value;
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, tokens, user, x, p_uid_str;
  string uid = Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> p_uid_str;
      if (uid == p_uid_str) {
        return user;
      }
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  long uptime = 0;
  string line, token;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      vector<string> info(std::istream_iterator<std::string>{linestream},
                          std::istream_iterator<std::string>());
      // for (int i = 0; i < info.size(); i++) {
      //   std::cout << "Index: " << i << ": " << info[i] << std::endl;
      // }
      // why am i losing the first element? not sure. fix by setting to 22
      uptime = stol(info[23]) / sysconf(_SC_CLK_TCK);
    }
  }
  return uptime;
}
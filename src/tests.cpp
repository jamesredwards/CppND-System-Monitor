#include "tests.h"

#include <iostream>
#include <vector>

#include "format.h"
#include "linux_parser.h"
#include "processor.h"
#include "unistd.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

void test_linux_parser() {
  test_os();
  test_kernel();
  test_pids();
  test_mem_utilisation();
  test_uptime();
  test_parse_proc_stat();
  test_jiffies();
  test_total_processes();
  test_procs_running();
  test_get_uid();
  test_get_user();
  test_get_command();
  test_pid_uptime();
  test_ram();
  test_active_jiffies();
}

void test_processor() { test_processor_utilisation(); }

void test_os() {
  cout << "Testing OS...\t";
  string os = LinuxParser::OperatingSystem();
  cout << os << endl;
}
void test_kernel() {
  cout << "Testing Kernel...\t";
  string k = LinuxParser::Kernel();
  cout << k << endl;
}
void test_pids() {
  cout << "Testing PIDS...\t";
  vector<int> pids = LinuxParser::Pids();
  cout << "Got " << pids.size() << " PIDs" << endl;
}

void test_mem_utilisation() {
  cout << "Testing Memory Utilisation...\t";
  float mem = LinuxParser::MemoryUtilization();
  cout << mem << endl;
}
void test_uptime() {
  cout << "Testing Uptime...\t";
  long uptime = LinuxParser::UpTime();
  cout << Format::ElapsedTime(uptime) << endl;
}

void test_parse_proc_stat() {
  cout << "Testing ParseProcStat...\t";
  std::vector<long> info = LinuxParser::ParseProcStat("cpu");
  for (auto i : info) {
    cout << i << " ";
  }
  cout << endl;
}

void test_jiffies() {
  cout << "Testing Jiffies...\t";
  long j = LinuxParser::Jiffies();
  cout << j << endl;
}

void test_total_processes() {
  cout << "Testing Total Processes...\t";
  int i = LinuxParser::TotalProcesses();
  cout << i << endl;
}

void test_procs_running() {
  cout << "Testing Running Processes...\t";
  int i = LinuxParser::RunningProcesses();
  cout << i << endl;
}

void test_get_uid() {
  cout << "Testing Uid...\t";
  string i = LinuxParser::Uid(16295);
  cout << i << endl;
}

void test_get_user() {
  cout << "Testing User...\t";
  string i = LinuxParser::User(16295);
  cout << i << endl;
}

void test_get_command() {
  cout << "Testing Command...\t";
  string i = LinuxParser::Command(16295);
  cout << i << endl;
}

void test_pid_uptime() {
  cout << "Testing PID Uptime...\t";
  string i = Format::ElapsedTime(LinuxParser::UpTime(16295));
  cout << i << endl;
}

void test_ram() {
  cout << "Testing RAM...\t";
  string i = LinuxParser::Ram(16295);
  cout << i << endl;
}

void test_active_jiffies() {
  cout << "Testing active jiffies by PID...\t";
  long i = LinuxParser::ActiveJiffies(16295);
  cout << i << endl;
}

void test_processor_utilisation() {
  Processor p;
  for (int i = 0; i < 10; i++) {
    float util = p.Utilization();
    cout << "Util: " << util << endl;
  }
}
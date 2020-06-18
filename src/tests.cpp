#include "tests.h"

#include <iostream>
#include <vector>

#include "format.h"
#include "linux_parser.h"

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
}

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
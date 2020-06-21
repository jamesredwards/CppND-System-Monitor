#ifndef TESTS_H
#define TESTS_H

// LinuxParser tests
void test_linux_parser();
void test_os();
void test_kernel();
void test_pids();
void test_mem_utilisation();
void test_uptime();
void test_parse_proc_stat();
void test_jiffies();
void test_total_processes();
void test_procs_running();
void test_get_uid();
void test_get_user();
void test_get_command();
void test_pid_uptime();
void test_ram();
void test_active_jiffies();

// Processor test(s)
void test_processor();
void test_processor_utilisation();

// Process test
void test_process();
#endif
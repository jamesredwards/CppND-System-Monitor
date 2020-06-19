#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
  Processor();

 private:
  long start_total_jiffies{0};
  long start_active_jiffies{0};
};

#endif
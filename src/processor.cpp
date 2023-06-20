#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization()
{
  long idle_jiff = LinuxParser::IdleJiffies();
  long total_jiff = LinuxParser::ActiveJiffies() + idle_jiff;

  long total_delta = total_jiff - Processor::total_z_;
  long idle_delta = idle_jiff - Processor::idle_z_;

  Processor::total_z_ = total_jiff;
  Processor::idle_z_ = idle_jiff;

  return ((float)(total_delta - idle_delta) / (float)total_delta);
}
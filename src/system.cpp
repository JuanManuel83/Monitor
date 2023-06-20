#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_ = {};
  vector<int> kus = LinuxParser::Pids();

  for (int i : kus){
    Process process(i);
    if (!process.Command().empty()){
      // Insert the pid in the right place, depending on the CPU usage
      auto pos = std::find_if(processes_.begin(), processes_.end(),[process](auto s)
                           {
                             return process < s;
                           });
      processes_.insert(pos, process);
    }
  }
  //std::qsort(processes_.data(), processes_.size(), sizeof (Process), comp);

  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel()
{
  return System::kernel_;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization()
{
    return LinuxParser::MemoryUtilization();
}

// TODO: Return the operating system name
std::string System::OperatingSystem()
{
  return System::os_;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses()
{
  return LinuxParser::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses()
{
  return LinuxParser::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime()
{
  return LinuxParser::UpTime();
}

// Constructor to gather fixed information
System::System()
{
  System::kernel_ = LinuxParser::Kernel();
  System::os_ = LinuxParser::OperatingSystem();
}

// Update function to calculate the CPU usage, populate the running processes
void System::Update()
{

}
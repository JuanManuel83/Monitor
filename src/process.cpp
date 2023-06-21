#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() {
  return Process::pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization()
{
  auto total_time = (float)LinuxParser::ActiveJiffies(Process::pid_);
  auto uptime = (float)LinuxParser::UpTime();
  auto seconds = uptime - (float)LinuxParser::UpTime(Process::pid_);
  auto hertz = (float)sysconf(_SC_CLK_TCK);

  Process::cpu_ = (total_time /hertz /seconds);
  return Process::cpu_;
}

// TODO: Return the command that generated this process
string Process::Command()
{
  return Process::command_;
}

// TODO: Return this process's memory utilization
string Process::Ram()
{
  return LinuxParser::Ram(Process::pid_);
}

// TODO: Return the user (name) that generated this process
string Process::User()
{
  return Process::user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime()
{
  return (LinuxParser::UpTime() -  LinuxParser::UpTime(Process::pid_));
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const
{
  if (a.cpu_ < Process::cpu_){
    return true;
  } else {
    return false;
  }

}

Process::Process(int pid)
{
  // Initialize some process data
  Process::pid_ = pid;
  Process::user_ = LinuxParser::User(Process::pid_);
  Process::command_ = LinuxParser::Command(Process::pid_);
  Process::CpuUtilization();
}
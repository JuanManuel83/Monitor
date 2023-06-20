#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, kernel, version;
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
  std::string line;
  std::string key;
  std::string value;
  std::string units;
  float total {0.0};
  float available {0.0};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestr(line);
      linestr >> key >> value >> units;
      if (key == "MemTotal") {
        total = stof(value);

      } else if (key == "MemAvailable") {
        available = stof(value);
        break;
      }
    }
  }
 // return (total-used)/1024;
  if (total != 0.0){
    return ((total - available) / total);
  } else {
    return 0.0;
  }

}

// TODO: Read and return the system uptime
long LinuxParser::UpTime()
{
  std::string line {""};
  std::string uptime_s {""};
  std::string idle_s {""};

  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestr(line);
    linestr >> uptime_s >> idle_s;
    return stoi(uptime_s);
  }

  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {

  long active = LinuxParser::ActiveJiffies();
  long idle = LinuxParser::IdleJiffies();

  return active + idle;
}

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid)
{
  string line {""};
  string item {""};
  vector<string> elements {};

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestr(line);
      while (linestr >> item)
      {
        elements.push_back(item);
      }
    }
  }
  return std::stoi(elements[14]) + std::stoi(elements[15]);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
  std::vector<string> file_data = LinuxParser::CpuUtilization();
  long active {};

  // Add all the jiffies in the active positions
  for (int i : LinuxParser::active_pos)
  {
    active = active + std::stoi(file_data[i]);
  }
  return active;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
  std::vector<string> file_data = LinuxParser::CpuUtilization();
  long idle {};

  // Add all the jiffies in the idle positions
  for (int i : LinuxParser::idle_pos)
  {
    idle = idle + std::stoi(file_data[i]);
  }
  return idle;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()
{
  vector<string> result {};
  std::string line {""};
  std::string key {""};

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while (std::getline(filestream,line)){
      std::istringstream linestr(line);
      while (linestr >> key){
        result.push_back(key);
      };
      if (result[0] == "cpu") {
        result.erase(result.begin());
        break;
      } else {
        result.clear();
      }
    }
  }
  return result;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  std::string line {""};
  std::string key {""};
  std::string value {""};

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestr(line);
      linestr >> key >> value;
      if (key == "processes") {
        return stoi(value);
      }
    }
  }

  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  std::string line {""};
  std::string key {""};
  std::string value {""};

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestr(line);
      linestr >> key >> value;
      if (key == "procs_running") {
        return stoi(value);
      }
    }
  }

  return 0;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid)
{

  std::string line;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    return  line;
  }
  return {};
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid)
{
  std::string line {""};
  std::string key {""};
  std::string value {""};
  std::string units {""};

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::stringstream  linestr (line);
      linestr >> key >> value >> units;
      if (key == "VmSize")
      {
        return to_string(stoi(value) / 1024);
      }
    }
  }
  return string();
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid)
{
  std::string line {""};
  std::string key {""};
  std::string value {""};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()){
    while(std::getline(filestream, line )){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestr(line);
      linestr >> key >> value;
      // In my linux system the user is under Uid, I would check for either option
      if ((key == "uid") || (key == "Uid")){
        return value;
      }
    }
  }
  return string();
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid)
{
  string in_uid{""};
  string Username {""};
  string Pass{""};
  string UID {""};

  string line {""};

  in_uid = LinuxParser::Uid(pid);

  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestr(line);
      linestr >> Username >> Pass >> UID;
      if (UID == in_uid) return Username;
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid)
{
  string line;
  string item;
  string value;
  vector<string> elements {};
  long hertz = sysconf(_SC_CLK_TCK);

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace (std::find(line.begin(), line.end(), '('),
                    std::find(line.begin(), line.end(), ')'),
                    ' ', '_');

      std::istringstream linestr(line);
      while (linestr >> item)
      {
          elements.push_back(item);
      }
    }
  }
  return (std::stoll(elements[14] ) + std::stoll( elements[15]))/hertz;
}

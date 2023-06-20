#include "ncurses_display.h"
#include "system.h"


int main() {
  std::vector<std::string> cpu_data{};

  System system;
  //TODO: Why the number of active processes is not properly displayed?
  NCursesDisplay::Display(system, 20);
}
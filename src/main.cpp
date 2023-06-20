#include "ncurses_display.h"
#include "system.h"


int main() {
  std::vector<std::string> cpu_data{};

  System system;
  NCursesDisplay::Display(system, 20);
}
#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {

  string hours {};
  string minutes {};
  string secs {};

  hours = std::to_string(seconds / 3600);
  if (hours.length() == 1)(hours = "0" + hours);
  minutes = std::to_string((seconds % 3600)/60);
  if (minutes.length() == 1)(minutes = "0" + minutes);
  secs = std::to_string(seconds % 60);
  if (secs.length() == 1)(secs = "0" + secs);

  return hours + "::" + minutes + "::" + secs;
}
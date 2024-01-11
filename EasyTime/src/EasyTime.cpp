#include "EasyTime/EasyTime.h"
#include <sstream>
#include <iostream>
#include <chrono>

namespace ns_easytime {

std::string getTimePointString_ns(long long v_time) {
  int seconds =
      static_cast<int>(v_time / 1000000000 + 8 * 3600);  // time zone +8
  int days = seconds / 86400;
  int year = 1970 + (days / 1461) * 4;  // recycled in 4 years = 1461 days
  int month = 0;
  int day = days % 1461;
  day = day > 730 ? day - 1 : day;
  year += (day / 365);
  day = day % 365;
  int monthday[]{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  for (int i = 0; i < 13; i++) {
    if (day < monthday[i + 1]) {
      month = i + 1;
      day = day - monthday[i] + 1;
      break;
    }
  }
  int sec = seconds % 86400;
  int hour = sec / 3600;
  int minute = sec % 3600 / 60;
  int second = sec % 60;
  int nanoseconds = v_time % 1000000000;
  int millisecond = nanoseconds / 1000;

  std::stringstream ss;
  ss << year << "/" << month << "/" << day << " " << hour << ":" << minute
     << ":" << second << "." << millisecond;

  return ss.str();
}

long long getTimeCode_ms() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()).count();
}

int gcd(int a, int b) {
  if (b == 0) {
    return a;
  }
  return gcd(b, a % b);
}

int lcm(int a, int b) { return a * b / gcd(a, b); }

std::string getTimePointString(long long v_time) {
  int seconds = static_cast<int>(v_time + 8 * 3600);  // time zone +8
  int days = seconds / 86400;
  int year = 1970 + (days / 1461) * 4;  // recycled in 4 years = 1461 days
  int month = 0;
  int day = days % 1461;
  day = day > 730 ? day - 1 : day;
  year += (day / 365);
  day = day % 365;
  int monthday[]{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  for (int i = 0; i < 13; i++) {
    if (day < monthday[i + 1]) {
      month = i + 1;
      day = day - monthday[i] + 1;
      break;
    }
  }
  int sec = seconds % 86400;
  int hour = sec / 3600;
  int minute = sec % 3600 / 60;
  int second = sec % 60;

  std::stringstream ss;
  ss << year << "/" << month << "/" << day << " " << hour << ":" << minute
     << ":" << second;

  return ss.str();
}
}

#include "helpers.h"

int iso_to_str(char* date, char* str) {
  struct tm t;
  time_t t_of_day;
  int year;
  int month;
  int day;

  const char s[2] = "-";

  year = atoi(strtok(date, s));
  month = atoi(strtok(NULL, s));
  day = atoi(strtok(NULL, s));

  t.tm_year = year-1900;
  t.tm_mon = month-1;
  t.tm_mday = day;
  t.tm_hour = 0;
  t.tm_min = 0;
  t.tm_sec = 0;
  t.tm_isdst = 0;

  t_of_day = mktime(&t);
  sprintf(str, "%ld", (long) t_of_day);

  return 0;
}

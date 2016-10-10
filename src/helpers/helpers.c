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

int get_uuid_date(char* path, char* uuid, char* start, char* end) {
  char epochs[20];
  char end_epochs[20];
  const char s[2] = "/";
  char * tmp_uuid = strtok(path, s);
  char * tmp_start = strtok(NULL, s);

  iso_to_str(tmp_start, epochs);
  long end_date = atol(epochs) + 86400;
  sprintf(end_epochs, "%ld", end_date);

  strcpy(start, epochs);
  strcpy(end, end_epochs);
  strcpy(uuid, tmp_uuid);

  return 0;
}

int get_uuid_date_date(char* path, char* uuid, char* start, char* finish) {
  const char s[2] = "/";
  uuid = strtok(path, s);
  start = strtok(NULL, s);
  finish = strtok(NULL, s);

  return 0;
}

#include "helpers.h"

time_t current_to_GM_hacked_function(struct tm *tm) {
    time_t epoch = 0;
    time_t offset = mktime(gmtime(&epoch));
    time_t utc = mktime(tm);
    return difftime(utc, offset);
}

int epoched(char* date, int days) {
  struct tm tm;
  memset(&tm, 0, sizeof(struct tm));

  if (strptime(date, "%F", &tm) == NULL) return -1;
  
  tm.tm_mday += days;
  return current_to_GM_hacked_function(&tm);
}

int get_uuid_date(char* path, char* uuid, int* start, int* end) {
  const char s[2] = "/";
  
  char * tmp_uuid = strtok(path, s);
  char * tmp_start = strtok(NULL, s);

  *start = epoched(tmp_start, 0);
  *end = epoched(tmp_start, 1);

  strcpy(uuid, tmp_uuid);

  return 0;
}

int get_uuid_date_date(char* path, char* uuid, int* start, int* finish) {
  const char slash[2] = "/";
  const char dash = '-';

  char * tmp_uuid = strtok(path, slash);
  char * tmp_start = strtok(NULL, slash);
  char * tmp_finish = strtok(NULL, slash);

  if(strchr(tmp_start, dash))
    *start = epoched(tmp_start, 0);
  else
    *start = atoi(tmp_start);

  if(strchr(tmp_finish, dash))
    *finish = epoched(tmp_finish, 0);
  else
    *finish = atoi(tmp_finish);

  strcpy(uuid, tmp_uuid);
  return 0;
}

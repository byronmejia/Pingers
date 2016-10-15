#ifndef HELPERS_H
#define HELPERS_H
#include <string.h>
#include <kore/kore.h>
#include <string.h>
#include <time.h>

time_t current_to_GM_hacked_function(struct tm *);
time_t epoched(char*, int);
int get_uuid_date(char*, char*, time_t*, time_t*);
int get_uuid_date_date(char*, char*, time_t*, time_t*);

#endif /* HELPERS_H */
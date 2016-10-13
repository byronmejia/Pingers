#ifndef HELPERS_H
#define HELPERS_H
#include <string.h>
#include <kore/kore.h>
#include <string.h>
#include <time.h>

time_t current_to_GM_hacked_function(struct tm *);
int epoched(char*, int);
int get_uuid_date(char*, char*, int*, int*);
int get_uuid_date_date(char*, char*, int*, int*);

#endif /* HELPERS_H */
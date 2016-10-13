#ifndef DEVICE_H
#define DEVICE_H
#include <string.h>
#include <jansson.h>
#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>

json_t *device_get_all(void);
json_t *device_pings_between(char*, int, int);
json_t *all_pings_between(int, int);
int device_insert(char*, char*);
int device_new(char*);

#endif /* DEVICE_H */

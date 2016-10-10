#ifndef DEVICE_H
#define DEVICE_H
#include <string.h>
#include <jansson.h>
#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>

json_t *device_get_all(void);

#endif /* DEVICE_H */
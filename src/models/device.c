#include "device.h"

json_t *device_get_all(void) {
  json_t *output = json_array();
  struct kore_pgsql	sql;

  /* Escape on database error */
  if (!kore_pgsql_query_init(&sql, NULL, "db", KORE_PGSQL_SYNC)) {
    kore_pgsql_logerror(&sql);
    kore_pgsql_cleanup(&sql);
    return NULL;
  }
  
  if (!kore_pgsql_query(&sql, "SELECT * FROM device")) {
    kore_pgsql_logerror(&sql);
    kore_pgsql_cleanup(&sql);
    return NULL;
  }

  /* Generate the JSON Array */
  int rows, i;
  json_t *device_name;
  rows = kore_pgsql_ntuples(&sql);
  for (i = 0; i < rows; i++) {
    device_name = json_string(kore_pgsql_getvalue(&sql, i, 0));
    json_array_append(output, device_name);
  }

  kore_pgsql_cleanup(&sql);
  return output;
}

json_t *device_pings_between(char* device_uuid, char* start, char* end) {
  json_t *output = json_array();
  struct kore_pgsql	sql;

  /* Escape on database error */
  if (!kore_pgsql_query_init(&sql, NULL, "db", KORE_PGSQL_SYNC)) {
    kore_pgsql_logerror(&sql);
    kore_pgsql_cleanup(&sql);
    return NULL;
  }

  /* Escape on SQL Error */
  char queryBuilder[200];
  sprintf(queryBuilder, 
  "\
  SELECT EXTRACT(epoch FROM time) \
  FROM ping \
  WHERE device_id = '%s' \
  AND time >= to_timestamp(%s) \
  AND time < to_timestamp(%s)\
  ", device_uuid, start, end);

  kore_log(LOG_NOTICE, "QUERY: %s", queryBuilder);

  if (!kore_pgsql_query(&sql, queryBuilder)) {
    kore_pgsql_logerror(&sql);
    kore_pgsql_cleanup(&sql);
    return NULL;
  }

  int rows, i;
  json_t *epochs;
  rows = kore_pgsql_ntuples(&sql);

  for (i = 0; i < rows; i++) {
    epochs = json_string(kore_pgsql_getvalue(&sql, i, 0));
    json_array_append(output, epochs);
  }

  kore_pgsql_cleanup(&sql);
  return output;
}

int device_insert(char* device_uuid, char* ping){
  struct kore_pgsql	sql;

  /* Escape on database error */
  if (!kore_pgsql_query_init(&sql, NULL, "db", KORE_PGSQL_SYNC)) {
    kore_pgsql_logerror(&sql);
    kore_pgsql_cleanup(&sql);
    return -1;
  }

  /* Escape on SQL Error */
  char queryBuilder[80];
  sprintf(queryBuilder, "SELECT id FROM device WHERE id = '%s'", device_uuid);
  kore_log(LOG_NOTICE, "Query: %s", queryBuilder);

  if (!kore_pgsql_query(&sql, queryBuilder)) {
    kore_pgsql_logerror(&sql);
    kore_pgsql_cleanup(&sql);
    return -1;
  }

  kore_log(LOG_NOTICE, "Built: %i", kore_pgsql_ntuples(&sql));

  if(kore_pgsql_ntuples(&sql) == 0) {
     kore_pgsql_cleanup(&sql);
     if(device_new(device_uuid) != 0) {
         return -1;
     }
     return device_insert(device_uuid, ping);
  }

  char insertQuery[150];
  sprintf(insertQuery, "INSERT INTO ping (time, device_id) VALUES (to_timestamp(%s), '%s')", ping, device_uuid);
  if (!kore_pgsql_query(&sql, insertQuery)) {
    kore_pgsql_logerror(&sql);
    kore_pgsql_cleanup(&sql);
    return -1;
  }

  kore_pgsql_cleanup(&sql);
  return 0;
}

int device_new(char* device_uuid){
  struct kore_pgsql sql;

  /* Escape on database error */
  if (!kore_pgsql_query_init(&sql, NULL, "db", KORE_PGSQL_SYNC)) {
    kore_pgsql_logerror(&sql);
    kore_pgsql_cleanup(&sql);
    return -1;
  }

  /* Escape on SQL Error */
  char queryBuilder[80];
  sprintf(queryBuilder, "INSERT INTO device (id) VALUES ('%s')", device_uuid);
  kore_log(LOG_NOTICE, "Query: %s", queryBuilder);

  if (!kore_pgsql_query(&sql, queryBuilder)) {
    kore_pgsql_logerror(&sql);
    kore_pgsql_cleanup(&sql);
    return -1;
  }

  kore_pgsql_cleanup(&sql);
  return 0;
}

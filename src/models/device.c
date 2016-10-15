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

json_t *device_pings_between(char* device_uuid, time_t start, time_t end) {
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
  SELECT time \
  FROM ping \
  WHERE device_id = '%s' \
  AND time >= %li \
  AND time < %li \
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
    epochs = json_integer((json_int_t) atoi(kore_pgsql_getvalue(&sql, i, 0)));
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
  sprintf(insertQuery, "INSERT INTO ping (time, device_id) VALUES ('%s', '%s')", ping, device_uuid);
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

json_t *all_pings_between(time_t start, time_t end) {
  struct kore_pgsql sql;
  json_t *output = NULL;

  /* Escape on database error */
  if (!kore_pgsql_query_init(&sql, NULL, "db", KORE_PGSQL_SYNC)) {
    kore_pgsql_logerror(&sql);
    kore_pgsql_cleanup(&sql);
    return NULL;
  }

  /* Escape on SQL Error */
  char queryBuilder[250];
  sprintf(queryBuilder, 
  "\
  SELECT * \
  FROM ping \
  WHERE \
  time >= %li \
  AND time < %li \
  ORDER BY device_id \
  ", start, end);

  kore_log(LOG_NOTICE, "QUERY: %s", queryBuilder);

  if (!kore_pgsql_query(&sql, queryBuilder)) {
    kore_pgsql_logerror(&sql);
    kore_pgsql_cleanup(&sql);
    return NULL;
  }

  int rows, i;

  json_t *current_device_json = NULL;
  json_t *device_array = NULL;
  char current_device[37];
  current_device[0] = '\0';
  rows = kore_pgsql_ntuples(&sql);

  for (i = 0; i < rows; i++) {
    if(strcmp(current_device, kore_pgsql_getvalue(&sql, i, 1)) != 0 ) {
      if(current_device[0] == '\0') {  
        output = json_array();    
      } else {
        current_device_json = json_object();
        json_object_set(current_device_json, current_device, device_array);
        json_array_append(output, current_device_json);
        current_device_json = NULL;
      }

      strcpy(current_device, kore_pgsql_getvalue(&sql, i, 1));

      device_array = json_array();
    }

    json_array_append(device_array, json_integer((json_int_t) atoi(kore_pgsql_getvalue(&sql, i, 0))));
  }

  current_device_json = json_object();
  json_object_set(current_device_json, current_device, device_array);
  json_array_append(output, current_device_json);
  current_device_json = NULL;

  kore_pgsql_cleanup(&sql);
  return output;
}

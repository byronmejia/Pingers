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
  
  /* Escape on SQL Error */
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
#include <string.h>
#include <time.h>
#include <jansson.h>
#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>
#include "models/device.h"
#include "helpers/helpers.h"

int init(int);
int catch_last_resort(struct http_request *);
int page(struct http_request *);
int get_all_devices(struct http_request *);
int clear_data(struct http_request *);
int get_all_pings_between(struct http_request *);
int get_all_pings_on_date(struct http_request *);
int get_device_pings_between(struct http_request *);
int get_device_pings_on_date(struct http_request *);
int post_device_ping(struct http_request *);
int truncate_database(void);

int init(int state) {
  kore_pgsql_register("db", "host=/tmp dbname=pingers");
  return (KORE_RESULT_OK);
}

int page(struct http_request *req) {
  char *response = "Hello homepage";
  http_response(req, 200, response, strlen(response));
  return (KORE_RESULT_OK);
}

int clear_data(struct http_request *req) {
  if(truncate_database() != 0) {
    char *response = "Error";
    http_response(req, 500, response, strlen(response));
    return (KORE_RESULT_OK);
  }

  char *response = "Success";
  http_response(req, 200, response, strlen(response));
  return (KORE_RESULT_OK);
}

int get_all_devices(struct http_request *req) {
  char *solution;

  json_t *output = device_get_all();

  if(output == NULL) {
    req->status = HTTP_STATUS_INTERNAL_ERROR;
  } else {
    req->status = HTTP_STATUS_OK; 
  }

  solution = json_dumps(output, JSON_COMPACT);
  http_response(req, req->status, solution, strlen(solution));
  return (KORE_RESULT_OK);
}

int get_all_pings_between(struct http_request *req) {
  kore_log(LOG_NOTICE, "PATH: %s", req->path);
  char useless_var[37];
  time_t start = 0;
  time_t end = 0;
  char * solution;

  get_uuid_date_date(req->path, useless_var, &start, &end);
  
  json_t *output = all_pings_between(start, end);
  if(output == NULL) {
    req->status = HTTP_STATUS_INTERNAL_ERROR;
  } else {
    req->status = HTTP_STATUS_OK; 
  }

  solution = json_dumps(output, JSON_COMPACT);

  http_response(req, 200, solution, strlen(solution));
  return (KORE_RESULT_OK);
}

int get_all_pings_on_date(struct http_request *req) {
  kore_log(LOG_NOTICE, "PATH: %s", req->path);
  char useless_var[37];
  time_t start = 0;
  time_t end = 0;
  char * solution;

  get_uuid_date(req->path, useless_var, &start, &end);

  kore_log(LOG_NOTICE, "EPOCH_S: %s", start);
  
  json_t *output = all_pings_between(start, end);
  if(output == NULL) {
    req->status = HTTP_STATUS_INTERNAL_ERROR;
  } else {
    req->status = HTTP_STATUS_OK; 
  }

  solution = json_dumps(output, JSON_COMPACT);


  char *response = "Hello All Pings On Date";
  http_response(req, 200, response, strlen(response));
  return (KORE_RESULT_OK);
}

int get_device_pings_between(struct http_request *req) {
  kore_log(LOG_NOTICE, "PATH: %s", req->path);
  char uuid[37];
  time_t start = 0;
  time_t end = 0;
  char * solution;

  get_uuid_date_date(req->path, uuid, &start, &end);

  kore_log(LOG_NOTICE, "UUID:    %s", uuid);
  kore_log(LOG_NOTICE, "EPOCH_S: %i", start);
  kore_log(LOG_NOTICE, "EPOCH_E: %i", end);

  json_t *output = device_pings_between(uuid, start, end);
  if(output == NULL) {
    req->status = HTTP_STATUS_INTERNAL_ERROR;
  } else {
    req->status = HTTP_STATUS_OK; 
  }

  solution = json_dumps(output, JSON_COMPACT);

  http_response(req, 200, solution, strlen(solution));
  return (KORE_RESULT_OK);
}

int get_device_pings_on_date(struct http_request *req) {
  kore_log(LOG_NOTICE, "PATH: %s", req->path);
  char uuid[37];
  time_t start = 0;
  time_t end = 0;
  char * solution;

  get_uuid_date(req->path, uuid, &start, &end);

  kore_log(LOG_NOTICE, "UUID:    %s", uuid);
  kore_log(LOG_NOTICE, "EPOCH_S: %i", start);
  kore_log(LOG_NOTICE, "EPOCH_E: %i", end);

  json_t *output = device_pings_between(uuid, start, end);
  if(output == NULL) {
    req->status = HTTP_STATUS_INTERNAL_ERROR;
  } else {
    req->status = HTTP_STATUS_OK; 
  }

  solution = json_dumps(output, JSON_COMPACT);

  http_response(req, req->status, solution, strlen(solution));
  return (KORE_RESULT_OK);
}

int post_device_ping(struct http_request *req) {
  if (req->method != HTTP_METHOD_POST) {
    char* response = "Not Found";
    http_response(req, 400, response, strlen(response));
    return (KORE_RESULT_OK);
  }

  kore_log(LOG_NOTICE, "PATH: %s", req->path);

  char * uuid;
  char * ping;
  const char s[2] = "/";

  uuid = strtok(req->path, s);
  ping = strtok(NULL, s);

  kore_log(LOG_NOTICE, "UUID: %s", uuid);
  kore_log(LOG_NOTICE, "PING: %s", ping);

  if (device_insert(uuid, ping) != 0) {
    char* response = "Error";
    http_response(req, 500, response, strlen(response));
    return (KORE_RESULT_OK);
  }

  char *response = "Success";
  http_response(req, 200, response, strlen(response));
  return (KORE_RESULT_OK);
}

int truncate_database(){
  struct kore_pgsql sql;
  
  /* Escape on database error */
  if (!kore_pgsql_query_init(&sql, NULL, "db", KORE_PGSQL_SYNC)) {
    kore_pgsql_logerror(&sql);
    kore_pgsql_cleanup(&sql);
    return -1;
  }

  /* Escape on SQL Error */
  if (!kore_pgsql_query(&sql, "TRUNCATE device, ping")) {
    kore_pgsql_logerror(&sql);
    kore_pgsql_cleanup(&sql);
    return -1;
  }

  kore_pgsql_cleanup(&sql);
  return 0;
}

int catch_last_resort(struct http_request *req) {
  char *response = "{}";
  http_response(req, 200, response, strlen(response));
  return (KORE_RESULT_OK);
}

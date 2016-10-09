#include <string.h>
#include <jansson.h>
#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>

int init(int);
int page(struct http_request *);
int getAllDevices(struct http_request *);
int clearData(struct http_request *);
int getAllPingsBetween(struct http_request *);
int getAllPingsOnDate(struct http_request *);
int getDevicePingsBetween(struct http_request *);
int getDevicePingsOnDate(struct http_request *);
int postDevicePing(struct http_request *);

int init(int state) {
	kore_pgsql_register("db", "host=/tmp dbname=pingers");
	return (KORE_RESULT_OK);
}


int page(struct http_request *req) {
  char *response = "Hello homepage";
  http_response(req, 200, response, strlen(response));
  return (KORE_RESULT_OK);
}

int clearData(struct http_request *req) {
  char *response = "Hello Clear Data";
  http_response(req, 200, response, strlen(response));
  return (KORE_RESULT_OK);
}

int getAllDevices(struct http_request *req) {
  struct kore_pgsql	sql;
  char *name;
  int rows, i;

  kore_log(LOG_NOTICE, json_dumps(json_pack("{}"), JSON_COMPACT));

  /* Start Postres */

  req->status = HTTP_STATUS_INTERNAL_ERROR;

  if (!kore_pgsql_query_init(&sql, NULL, "db", KORE_PGSQL_SYNC)) {
    kore_pgsql_logerror(&sql);
    goto out;
  }

  if (!kore_pgsql_query(&sql, "SELECT * FROM device")) {
    kore_pgsql_logerror(&sql);
    goto out;
  }

  rows = kore_pgsql_ntuples(&sql);
  for (i = 0; i < rows; i++) {
    name = kore_pgsql_getvalue(&sql, i, 0);
    kore_log(LOG_NOTICE, "name: '%s'", name);
  }

  req->status = HTTP_STATUS_OK;

  out:
  kore_pgsql_cleanup(&sql);

  char *response = "Hello All Devices";
  http_response(req, req->status, response, strlen(response));
  return (KORE_RESULT_OK);
}

int getAllPingsBetween(struct http_request *req) {
  char *response = "Hello All Pings Between";
  http_response(req, 200, response, strlen(response));
  return (KORE_RESULT_OK);
}

int getAllPingsOnDate(struct http_request *req) {
  char *response = "Hello All Pings On Date";
  http_response(req, 200, response, strlen(response));
  return (KORE_RESULT_OK);
}

int getDevicePingsBetween(struct http_request *req) {
  char *response = "Hello Device Pings Between";
  http_response(req, 200, response, strlen(response));
  return (KORE_RESULT_OK);
}

int getDevicePingsOnDate(struct http_request *req) {
  char *response = "Hello Device Pings on Date";
  http_response(req, 200, response, strlen(response));
  return (KORE_RESULT_OK);
}

int postDevicePing(struct http_request *req) {
  char *response = "Hello Post Device Pinga";
  http_response(req, 200, response, strlen(response));
  return (KORE_RESULT_OK);
}


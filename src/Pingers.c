#include <string.h>
#include <jansson.h>
#include <kore/kore.h>
#include <kore/http.h>
#include <kore/pgsql.h>
#include "models/device.h"

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


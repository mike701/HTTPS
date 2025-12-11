#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <stddef.h>

#define HTTP_MAX_REQUEST_LEN 8192*4
#define HTTP_METHOD_MAX_LEN 8
#define HTTP_PATH_MAX_LEN 2048
#define HTTP_PROTOCOL_MAX_LEN 16
#define HTTP_MAX_HEADER_KEY_LEN 256
#define HTTP_MAX_HEADER_VAL_LEN 256//512
#define HTTP_MAX_REASON_LEN 64

// typedef struct {
    // char method[HTTP_METHOD_MAX_LEN];    // Maximum 7 characters for HTTP methods (e.g., GET, POST) + null terminator
    // char path[HTTP_PATH_MAX_LEN];   // Based on practical limits for request URIs
    // char protocol[HTTP_PROTOCOL_MAX_LEN]; // Maximum length for protocol versions (e.g., HTTP/1.1)
// } http_request;

typedef enum {
    HTTP_PARSE_OK,
    HTTP_PARSE_INVALID,
} http_parse_e;

typedef enum {
    HTTP_METHOD_GET,
    HTTP_METHOD_POST,
    HTTP_METHOD_PUT,
    HTTP_METHOD_UNK,
} http_method_e;

typedef struct {
    char key[HTTP_MAX_HEADER_KEY_LEN]; // Header key (e.g. "Host")
    char value[HTTP_MAX_HEADER_VAL_LEN]; // Header value (e.g. "localhost:8080")
} http_header_t;

typedef struct {
    char method[16];
    char path[256];
    char protocol[16];
    http_header_t *headers;
    size_t header_count;
    char buffer[HTTP_MAX_REQUEST_LEN];
} http_request;

typedef struct {
    int status_code;    //Numeric status code (e.g., 200, 404)
    char reason_phrase[64]; //Text explanation of the status (e.g. "OK", "Not Found")
    http_header_t *headers; //Array of HTTP headers (key-value pairs)
    size_t header_count;    //Number of headers
    char *body;         //Response body content
    size_t body_length; //Length of the response body
} http_response;

void send_http_response(int client_fd, const http_response *response);
char *construct_http_response(const http_response *response, size_t *response_length);
void init_http_response(http_response *response);
void set_http_body(http_response *response,const char *body);
void add_http_header(http_response *response, const char *key, const char *value);
void free_http_response();
http_parse_e parse_http_headers(const char *raw_request, http_request *request);
http_parse_e read_http_headers(http_request *request);
void free_http_headers(http_request *request);
#endif // HTTP_H

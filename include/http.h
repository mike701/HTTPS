#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <stddef.h>

#define HTTP_MAX_REQUEST_LEN 8192*4
#define HTTP_METHOD_MAX_LEN 8
#define HTTP_PATH_MAX_LEN 2048
#define HTTP_PROTOCOL_MAX_LEN 16

typedef enum {
    HTTP_PARSE_OK,
    HTTP_PARSE_INVALID,
} http_parse_e;

typedef struct {
    char method[HTTP_METHOD_MAX_LEN];    // Maximum 7 characters for HTTP methods (e.g., GET, POST) + null terminator
    char path[HTTP_PATH_MAX_LEN];   // Based on practical limits for request URIs
    char protocol[HTTP_PROTOCOL_MAX_LEN]; // Maximum length for protocol versions (e.g., HTTP/1.1)
} http_request;

typedef struct {
    char key[256]; // Header key (e.g. "Host")
    char value[512]; // Header value (e.g. "localhost:8080")
} http_header_t;

http_parse_e read_http_request(int socket_fd, http_request *request);
void parse_http_headers(const char *raw_request, http_request *request);
void free_http_headers(http_request *request);
#endif //HTTP_H
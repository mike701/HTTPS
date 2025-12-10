#include "http.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void add_http_header(http_response *response, const char *key, const char *value){
    response->headers = realloc(response->headers, sizeof(http_header_t) * (response->header_count+1));
    if(!response->headers){
        perror("Failed to allocate memory fdor headers");
        exit(EXIT_FAILURE);
    }
    strncpy(response->headers[response->header_count].key, key, sizeof(response->headers[response->header_count].key)-1);
    strncpy(response->headers[response->header_count].value, value, sizeof(response->headers[response->header_count].value));
    response->header_count++;
}

void free_http_response(http_response *response){
    free(response->headers);
    response->headers = NULL;
    response->header_count = 0;
}

void init_http_response(http_response *response) {
    response->status_code = 200; // Default to OK
    strncpy(response->reason_phrase, "OK", sizeof(response->reason_phrase) - 1);
    response->headers = NULL;
    response->header_count = 0;
    response->body = NULL;
    response->body_length = 0;
}

http_parse_e parse_http_headers(const char *raw_request, http_request *request) {
    const char *line_start = strstr(raw_request, "\r\n");
    if(!line_start) return HTTP_PARSE_INVALID;
    
    line_start +=2; // skip the request line
    while(line_start && *line_start && *line_start != '\r' && *line_start != '\n'){
        const char *line_end = strstr(line_start, "\r\n");
        if(!line_end) break;

        size_t line_length = line_end - line_start;
        char line[1024] = {0};
        strncpy(line, line_start, line_length);

        char *colon_pos = strchr(line, ':');
        if(colon_pos){
            *colon_pos = '\0';
            const char *key = line;
            const char *value = colon_pos + 1;

            while(*value == ' ') value++; //skip Leading spaces

            request->headers = realloc(request->headers, sizeof(http_header_t)*(request->header_count + 1));
            if(!request->headers){
                perror("Failed to allocate memory for headers");
                exit(EXIT_FAILURE);
            }

            strncpy(request->headers[request->header_count].key, key, sizeof(request->headers[request->header_count].key)-1);
            strncpy(request->headers[request->header_count].value, value, sizeof(request->headers[request->header_count].value)-1);
            
            request->header_count++;
        }

        line_start = line_end + 2;
    }
    return HTTP_PARSE_OK;
}

void free_http_headers(http_request *request){
    free(request->headers);
    request->headers = NULL;
    request->header_count = 0;
}

http_parse_e read_http_request(int socket_fd, http_request *request){
    // char buffer[HTTP_MAX_REQUEST_LEN];
    ssize_t bytes_read = read(socket_fd, request->buffer, sizeof(request->buffer)-1);
    if(bytes_read <= 0){
        return HTTP_PARSE_INVALID;
    }
    request->buffer[bytes_read] = '\0';
    
    if(sscanf(request->buffer, "%7s %2047s %15s", request->method, request->path, request->protocol) != 3){
        return HTTP_PARSE_INVALID;
    }
    return HTTP_PARSE_OK;
}
//first pass attempt
// char client_buffer[4096];
// memset(&client_buffer, '\0', 4096);
//  ssize_t request_size = read(client_fd, &client_buffer, sizeof(client_buffer)-1);

// if(request_size > 0){
    // printf("%s\n", &client_buffer);
    // free(client_buffer);
    // // client_buffer = NULL;s
    // close(client_fd);
    // close(server.socket_fd);
    // return 0;
// }
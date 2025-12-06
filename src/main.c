#include "main.h"
#include "tcp.h"
#include "http.h"
#include <string.h>

int main() {
    printf("START\n");
    tcp_server server = {0};
    server_status_e status = bind_tcp_port(&server, 8080);
    if (status != SERVER_OK) {
        printf("Server initialization failed\n");
        exit(EXIT_FAILURE);
    }

    int client_fd = accept_client(server.socket_fd);
    if (client_fd == -1) {
        printf("Failed to accept client connection\n");
        close(server.socket_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");


   http_request req = {0};
   if(read_http_request(client_fd, &req) != HTTP_PARSE_OK){
    printf("Failed to read or parse HTTP request\n");
    close(client_fd);
    return 0;
   }

   if(parse_http_headers(req.buffer, &req) != HTTP_PARSE_OK){
    printf("Failed to read or parse HTTP request");
    close(client_fd);
    return 0;
   }
   
   printf("HTTP request parsed successfully\n");
   printf("%s\n", req.method);
   printf("%s\n", req.path);
   printf("%s\n", req.protocol);

   printf("Parsed HTTP Headers:\n");
   for (size_t i = 0; i < req.header_count; i++) {
      printf("%s: %s\n", req.headers[i].key, req.headers[i].value);
   }   

   free_http_headers(&req);
   
   close(client_fd);
   close(server.socket_fd);

//    const char *raw_request =
//         "GET /index.html HTTP/1.1\r\n"
//         "Host: localhost:8080\r\n"
//         "User-Agent: curl/7.68.0\r\n"
//         "Accept: */*\r\n"
//         "\r\n";
    
//     http_request request = {0};
//     parse_http_headers(raw_request, &request);

//     printf("Parsed HTTP Headers:\n");
//     for (size_t i = 0; i < request.header_count; i++) {
//         printf("%s: %s\n", request.headers[i].key, request.headers[i].value);
//     }

//     free_http_headers(&request);
   return 0;
}
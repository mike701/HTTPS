#include "main.h"
#include "tcp.h"


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

    close(client_fd);
    close(server.socket_fd);
    return 0;
}
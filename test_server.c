#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "lib/tcpsock.h"

#define MAX_CONNECTIONS 5
#define BUFFER_SIZE 256

void *client_handler(void *arg);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <port> <max_connections>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    int max_connections = atoi(argv[2]);

    if (max_connections > MAX_CONNECTIONS) {
        fprintf(stderr, "Maximum connections allowed: %d\n", MAX_CONNECTIONS);
        exit(EXIT_FAILURE);
    }

    tcpsock_t *server;
    if (tcp_passive_open(&server, port) != TCP_NO_ERROR) {
        fprintf(stderr, "Error: Could not open server socket on port %d\n", port);
        exit(EXIT_FAILURE);
    }

    printf("Server started on port %d, waiting for connections...\n", port);

    pthread_t threads[MAX_CONNECTIONS];
    int connection_count = 0;

    while (1) {
        tcpsock_t *client;
        if (tcp_wait_for_connection(server, &client) != TCP_NO_ERROR) {
            fprintf(stderr, "Error: Could not accept client connection\n");
            continue;
        }

        printf("New client connected\n");

        if (pthread_create(&threads[connection_count], NULL, client_handler, client) != 0) {
            fprintf(stderr, "Error: Could not create thread for client\n");
            tcp_close(&client);
        } else {
            connection_count = (connection_count + 1) % MAX_CONNECTIONS;
        }
    }

    tcp_close(&server);
    return 0;
}

void *client_handler(void *arg) {
    tcpsock_t *client = (tcpsock_t *)arg;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while (1) {
        bytes_received = BUFFER_SIZE;
        if (tcp_receive(client, buffer, &bytes_received) != TCP_NO_ERROR || bytes_received == 0) {
            printf("Client disconnected\n");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Received: %s\n", buffer);

        // Echo the message back to the client
        int bytes_to_send = bytes_received;
        if (tcp_send(client, buffer, &bytes_to_send) != TCP_NO_ERROR) {
            fprintf(stderr, "Error: Could not send data to client\n");
            break;
        }
    }

    tcp_close(&client);
    return NULL;
}

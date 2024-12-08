#include "tcpsock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Open a passive (server-side) TCP socket
int tcp_passive_open(tcpsock_t **sock, int port) {
    *sock = malloc(sizeof(tcpsock_t));
    if (!*sock) return TCP_MEMORY_ERROR;

    (*sock)->sd = socket(AF_INET, SOCK_STREAM, 0);
    if ((*sock)->sd < 0) {
        free(*sock);
        return TCP_SOCKET_ERROR;
    }

    memset(&((*sock)->addr), 0, sizeof((*sock)->addr));
    (*sock)->addr.sin_family = AF_INET;
    (*sock)->addr.sin_port = htons(port);
    (*sock)->addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind((*sock)->sd, (struct sockaddr *)&((*sock)->addr), sizeof((*sock)->addr)) < 0) {
        close((*sock)->sd);
        free(*sock);
        return TCP_SOCKOP_ERROR;
    }

    if (listen((*sock)->sd, 5) < 0) {
        close((*sock)->sd);
        free(*sock);
        return TCP_SOCKOP_ERROR;
    }

    return TCP_NO_ERROR;
}

// Open an active (client-side) TCP socket
int tcp_active_open(tcpsock_t **sock, int remote_port, const char *remote_ip) {
    *sock = malloc(sizeof(tcpsock_t));
    if (!*sock) return TCP_MEMORY_ERROR;

    (*sock)->sd = socket(AF_INET, SOCK_STREAM, 0);
    if ((*sock)->sd < 0) {
        free(*sock);
        return TCP_SOCKET_ERROR;
    }

    memset(&((*sock)->addr), 0, sizeof((*sock)->addr));
    (*sock)->addr.sin_family = AF_INET;
    (*sock)->addr.sin_port = htons(remote_port);

    if (inet_pton(AF_INET, remote_ip, &((*sock)->addr.sin_addr)) <= 0) {
        close((*sock)->sd);
        free(*sock);
        return TCP_ADDRESS_ERROR;
    }

    if (connect((*sock)->sd, (struct sockaddr *)&((*sock)->addr), sizeof((*sock)->addr)) < 0) {
        close((*sock)->sd);
        free(*sock);
        return TCP_SOCKOP_ERROR;
    }

    return TCP_NO_ERROR;
}

// Wait for a connection
int tcp_wait_for_connection(tcpsock_t *server, tcpsock_t **client) {
    *client = malloc(sizeof(tcpsock_t));
    if (!*client) return TCP_MEMORY_ERROR;

    socklen_t addr_len = sizeof((*client)->addr);
    (*client)->sd = accept(server->sd, (struct sockaddr *)&((*client)->addr), &addr_len);
    if ((*client)->sd < 0) {
        free(*client);
        return TCP_SOCKET_ERROR;
    }

    return TCP_NO_ERROR;
}

// Close the socket
int tcp_close(tcpsock_t **sock) {
    if (*sock) {
        close((*sock)->sd);
        free(*sock);
        *sock = NULL;
    }
    return TCP_NO_ERROR;
}

// Send data through the socket
int tcp_send(tcpsock_t *sock, const void *buffer, int *buf_size) {
    int bytes_sent = send(sock->sd, buffer, *buf_size, 0);
    if (bytes_sent < 0) return TCP_SOCKOP_ERROR;

    *buf_size = bytes_sent;
    return TCP_NO_ERROR;
}

// Receive data from the socket
int tcp_receive(tcpsock_t *sock, void *buffer, int *buf_size) {
    int bytes_received = recv(sock->sd, buffer, *buf_size, 0);
    if (bytes_received < 0) return TCP_SOCKOP_ERROR;

    *buf_size = bytes_received;
    return TCP_NO_ERROR;
}

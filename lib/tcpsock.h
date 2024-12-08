#ifndef TCPSOCK_H
#define TCPSOCK_H

#include <netinet/in.h>

// Constants for error codes
#define TCP_NO_ERROR 0
#define TCP_SOCKET_ERROR -1
#define TCP_MEMORY_ERROR -2
#define TCP_ADDRESS_ERROR -3
#define TCP_SOCKOP_ERROR -4
#define TCP_CONNECTION_CLOSED -5

// TCP socket structure
typedef struct {
    int sd;                 // Socket descriptor
    struct sockaddr_in addr; // Address structure
} tcpsock_t;

// Function prototypes
int tcp_passive_open(tcpsock_t **sock, int port);
int tcp_active_open(tcpsock_t **sock, int remote_port, const char *remote_ip);
int tcp_wait_for_connection(tcpsock_t *server, tcpsock_t **client);
int tcp_close(tcpsock_t **sock);
int tcp_send(tcpsock_t *sock, const void *buffer, int *buf_size);
int tcp_receive(tcpsock_t *sock, void *buffer, int *buf_size);

#endif

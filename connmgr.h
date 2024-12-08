#ifndef CONNMGR_H
#define CONNMGR_H

void start_server(int port, int max_clients);
void handle_connection(int client_socket);

#endif

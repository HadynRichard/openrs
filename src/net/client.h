#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include "buffer.h"
#include "packet.h"
#include "player.h"

// Client states
#define DISCONNECTED 0
#define CONNECTED 1
#define LOGGING_IN 2
#define LOGGED_IN 3

#define BUFFER_SIZE 4096

// Defines a client.
struct client {
	int state;
	int sockfd;
	struct sockaddr_in *addr;
	struct epoll_event *epollev;
	struct buffer *in_buffer;
	struct buffer *out_buffer;
	struct packet *packet;
	struct player *player;
};

// Initializes a client.
void init_client(struct client *c);

// Frees the memory of a client.
void free_client(struct client *c);

void client_send(struct client *c, struct buffer *b);

// Performs a read operation.
void do_read(struct client *c);

// Performs a write operation.
void do_write(struct client *c);

#endif // #ifndef CLIENT_H

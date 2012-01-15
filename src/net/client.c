#include "client.h"

#include "decoder.h"
#include "reactor.h"

#include <sys/epoll.h>
#include <stdlib.h>
#include <stdio.h>

void init_client(struct client *c) {
	c->state = CONNECTED;
	
	// Initialize the in buffer.
	c->in_buffer = malloc(sizeof(struct buffer));
	init_buffer(c->in_buffer);
	
	// Initialize the out buffer.
	c->out_buffer = malloc(sizeof(struct buffer));
	init_buffer(c->out_buffer);
	
	// Initialize the packet.
	c->packet = malloc(sizeof(struct packet));
	init_packet(c->packet);
	
	// Initialize the ciphers to 0 for now.
	c->in_cipher = 0;
	c->out_cipher = 0;
}

void free_client(struct client *c) {
	c->state = DISCONNECTED;
	free(c->addr);
	free(c->epollev);
	free_buffer(c->in_buffer);
	free_buffer(c->out_buffer);
	free_packet(c->packet);
	if (c->in_cipher)
		free(c->in_cipher);
	if (c->out_cipher)
		free(c->out_cipher);
	free(c);
}

void client_send(struct client *c, struct buffer *b) {
	// Queue the data into the outgoing buffer.
	int i;
	for (i = 0; i < b->position; i++)
		put_byte(c->out_buffer, b->data[i], X_NONE);
		
	// Flag write interest with epoll.
	c->epollev->events |= EPOLLOUT;
	epoll_ctl(epfd, EPOLL_CTL_MOD, c->sockfd, c->epollev);
}

void do_read(struct client *c) {
	// Read the data into a temporary buffer.
	char data[256];
	int amt = recv(c->sockfd, &data, 256, 0);
	if (amt == -1) {
		c->state = DISCONNECTED;
		return;
	}
	
	// Append the data to the in buffer (so position can be maintained).
	c->in_buffer->size += amt;
	int i;
	for (i = 0; i < amt; i++)
		put_byte(c->in_buffer, data[i], X_NONE);
	c->in_buffer->position -= amt;
	
	// Decode the login process.
	if (c->state == CONNECTED || c->state == LOGGING_IN)
		login_decode(c, c->in_buffer);
	else if (c->state == LOGGED_IN)
		// Decode the game stream.
		while (c->in_buffer->size > c->in_buffer->position)
			if (!decode(c, c->in_buffer))
				break;
	
	// Shift the unread data to the beginning of the buffer.
	c->in_buffer->position = 0;
	c->in_buffer->size = 0;
}

void do_write(struct client *c) {
	// Send the data along the socket.
	int amtsent = send(c->sockfd, c->out_buffer->data, c->out_buffer->position, 0);
	
	// Shift the unsent data to the beginning of the buffer.
	c->out_buffer->position = 0;
	
	// Remove write interest if we have no more data to send.
	if (0 == 0) {
		c->epollev->events &= ~EPOLLOUT;
		epoll_ctl(epfd, EPOLL_CTL_MOD, c->sockfd, c->epollev);
	}
}

#include "client.h"

#include "decoder.h"

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
}

void free_client(struct client *c) {
	c->state = DISCONNECTED;
	free(c->addr);
	free(c->epollev);
	free_buffer(c->in_buffer);
	free_buffer(c->out_buffer);
	free_packet(c->packet);
	free(c);
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
	if (c->state == CONNECTED || c->state == LOGGING_IN) {
		login_decode(c, c->in_buffer);
	} else {	
		// Decode the game stream.
		while (c->in_buffer->size - 1 > c->in_buffer->position)
			decode(c, c->in_buffer);
	}
	
	// Shift the remaining data to the beginning of the buffer.
	int position = 0;
	for (i = c->in_buffer->position; i < c->in_buffer->size - 1; i++)
		c->in_buffer->data[position++] = (char) get_byte(c->in_buffer, X_NONE);
	c->in_buffer->position = position;
	c->in_buffer->size = position - 1;
}

void do_write(struct client *c) {
	// TODO: Send queued data
}

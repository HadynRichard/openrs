#include "decoder.h"

#include <stdio.h>
#include <stdlib.h>

#include "player.h"

void decode(struct client *c, struct buffer *b) {
	struct packet *packet = c->packet;
	
	// Decode the opcode.
	if (packet->opcode == -1) {
		if (b->size - b->position - 1 < 1)
				return; // Not enough data to read the byte
		packet->opcode = get_byte(b, X_NONE) & 0xff;
		// TODO: ISAAC decryption
	}
	
	// Decode the length.
	if (packet->length == -1) {
		packet->length = 0; // TODO: Pull it from a packet length array.
	 	
	 	// Decode a variable-sized packet.
		if (packet->length == -1) {
			if (b->size - b->position - 1 < 1)
				return; // Not enough data to read the byte
			packet->length = get_byte(b, X_NONE) & 0xff;
		}
		
		// Set the packet payload size to the length.
		packet->payload->size = packet->length;
	}
	
	// Ensure that all of the data has arrived.
	if ((b->size - b->position - 1) < packet->length)
		return; // Not enough data.
	
	// Fill the packet payload with data from the buffer.	
	int idx;
	for (idx = 0; idx < packet->length; idx++)
		put_byte(packet->payload, get_byte(b, X_NONE), X_NONE);
		
	// TODO: Handle the packet
	printf("Received packet [opcode:%d length:%d]\n", packet->opcode, packet->length);
	
	// Reset the packet for the next initialization.
	packet->opcode = -1;
	packet->length = -1;
	packet->payload->size = 0;
	packet->payload->position = 0;
}

void login_decode(struct client *c, struct buffer *b) {
	struct buffer out;
	int32_t i, *isaac_seed = malloc(sizeof(int32_t) * 4);
	int64_t server_key, client_key;
	switch (c->state) {
	case CONNECTED:
		// Ensure that all data has arrived.
		if (b->size < 2) {
			return;
		}
	
		// Verify the service request.
		i = get_byte(b, X_NONE);
		if (i != 14) {
			printf("Invalid service request: %d\n", i);
			c->state = DISCONNECTED;
			return;
		}
		
		get_byte(b, X_NONE); // Skip le name hash.
		
		// Send the response.
		init_buffer(&out);
		
		// First 8 bytes ignored.
		for (i = 0; i < 8; i++)
			put_byte(&out, 0, X_NONE);
			
		// Proceed with login.
		put_byte(&out, 0, X_NONE);
		
		server_key = rand();
		put_long(&out, server_key, ENDIAN_BIG, X_NONE);
		
		// Send the data.
		client_send(c, &out);
		c->state = LOGGING_IN;
		break;
	case LOGGING_IN:
		// Ensure that the login type is valid.
		i = get_byte(b, X_NONE);
		if (i != 16 && i != 18) {
			printf("Invalid login type: %d\n", i);
			c->state = DISCONNECTED;
			return;
		}
		
		// Ensure that all of the data has arrived.
		i = get_byte(b, X_NONE);
		if (i > b->size) {
			return;
		}
		
		get_byte(b, X_NONE); // Skip 255 byte.
		
		// Make sure the client revision is 317.
		i = get_short(b, ENDIAN_BIG, X_NONE);
		if (i != 317) {
			printf("Invalid client revision: %d\n", i);
			c->state = DISCONNECTED;
			return;
		}
		
		get_byte(b, X_NONE); // Skip memory version.
		
		// Skip the CRC keys for now.
		for (i = 0; i < 9; i++)
			get_int(b, ENDIAN_BIG, X_NONE);
			
		get_byte(b, X_NONE); // Skip RSA block size.
		
		// TODO: Decode RSA at this point.
		
		// Ensure RSA decoded properly.
		i = get_byte(b, X_NONE);
		if (i != 0xA) {
			printf("Invalid RSA opcode (expected 0xA): %x\n", i);
			c->state = DISCONNECTED;
			return;
		}
		
		// Set up the ISAAC cipher seed.
		server_key = get_long(b, ENDIAN_BIG, X_NONE);
		client_key = get_long(b, ENDIAN_BIG, X_NONE);
		isaac_seed[0] = (int32_t) (client_key >> 32);
		isaac_seed[1] = (int32_t) (client_key);
		isaac_seed[2] = (int32_t) (server_key >> 32);
		isaac_seed[3] = (int32_t) (server_key);
		// TODO: init_isaac
		
		// Initialize the player.
		struct player *p = malloc(sizeof(struct player));
		init_player(p);
		i = get_int(b, ENDIAN_BIG, X_NONE); // UID
		p->username = get_string(b);
		p->password = get_string(b);
		p->client = c;
		c->player = p;
		
		// Send the response
		init_buffer(&out);
		put_byte(&out, 2, X_NONE);
		put_byte(&out, 0, X_NONE);
		put_byte(&out, 0, X_NONE);
		client_send(c, &out);
		c->state = LOGGED_IN;
		break;
	}
}

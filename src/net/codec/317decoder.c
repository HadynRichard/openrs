#include "script_loader.h"

#include "decoder.h"

#include <stdio.h>
#include <stdlib.h>

#include "world.h"
#include "player.h"
#include "script_loader.h"
#include "isaac_rand.h"

static int packet_sizes[] = {
	0, 0, 0, 1, -1, 0, 0, 0, 0, 0, // 0
	0, 0, 0, 0, 8, 0, 6, 2, 2, 0,  // 10
	0, 2, 0, 6, 0, 12, 0, 0, 0, 0, // 20
	0, 0, 0, 0, 0, 8, 4, 0, 0, 2,  // 30
	2, 6, 0, 6, 0, -1, 0, 0, 0, 0, // 40
	0, 0, 0, 12, 0, 0, 0, 0, 8, 0, // 50
	0, 8, 0, 0, 0, 0, 0, 0, 0, 0,  // 60
	6, 0, 2, 2, 8, 6, 0, -1, 0, 6, // 70
	0, 0, 0, 0, 0, 1, 4, 6, 0, 0,  // 80
	0, 0, 0, 0, 0, 3, 0, 0, -1, 0, // 90
	0, 13, 0, -1, 0, 0, 0, 0, 0, 0,// 100
	0, 0, 0, 0, 0, 0, 0, 6, 0, 0,  // 110
	1, 0, 6, 0, 0, 0, -1, 0, 2, 6, // 120
	0, 4, 6, 8, 0, 6, 0, 0, 0, 2,  // 130
	0, 0, 0, 0, 0, 6, 0, 0, 0, 0,  // 140
	0, 0, 1, 2, 0, 2, 6, 0, 0, 0,  // 150
	0, 0, 0, 0, -1, -1, 0, 0, 0, 0,// 160
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 170
	0, 8, 0, 3, 0, 2, 0, 0, 8, 1,  // 180
	0, 0, 12, 0, 0, 0, 0, 0, 0, 0, // 190
	2, 0, 0, 0, 0, 0, 0, 0, 4, 0,  // 200
	4, 0, 0, 0, 7, 8, 0, 0, 10, 0, // 210
	0, 0, 0, 0, 0, 0, -1, 0, 6, 0, // 220
	1, 0, 0, 0, 6, 0, 6, 8, 1, 0,  // 230
	0, 4, 0, 0, 0, 0, -1, 0, -1, 4,// 240
	0, 0, 6, 6, 0, 0, 0            // 250
};

int decode(struct client *c, struct buffer *b) {
	struct packet *packet = c->packet;
	
	// Decode the opcode.
	if (packet->opcode == -1) {
		if (b->size - (b->position + 1) < 1)
			return 0; // Not enough data to read the byte
		packet->opcode = get_byte(b, X_NONE) & 0xff;
		packet->opcode = packet->opcode - isaac_next_int(c->in_cipher) & 0xff;
		printf("Opcode %d\n", packet->opcode);
	}
	
	// Decode the length.
	if (packet->length == -1) {
		packet->length = packet_sizes[packet->opcode];
	 	
	 	// Decode a variable-sized packet.
		if (packet->length == -1) {
			if (b->size - (b->position + 1) < 1)
				return 0; // Not enough data to read the byte
			packet->length = get_byte(b, X_NONE) & 0xff;
		}
		
		// Set the packet payload size to the length.
		packet->payload->size = packet->length;
	}
	
	// Ensure that all of the data has arrived.
	if ((b->size - (b->position + 1)) < packet->length)
		return 0; // Not enough data.
	
	// Fill the packet payload with data from the buffer.
	int idx;
	for (idx = 0; idx < packet->length; idx++)
		put_byte(packet->payload, get_byte(b, X_NONE), X_NONE);
		
	// TODO: Handle the packet
	printf("Received packet [opcode:%d length:%d]\n", packet->opcode, packet->length);
	int i;
	for (i = 0; i < packet->length; i++)
		get_byte(b, X_NONE);
	
	// Reset the packet for the next initialization.
	packet->opcode = -1;
	packet->length = -1;
	packet->payload->size = 0;
	packet->payload->position = 0;
	return 1;
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
		c->in_cipher = malloc(sizeof(struct isaac_context));
		init_isaac(c->in_cipher, isaac_seed);
		for (i = 0; i < 4; i++)
			isaac_seed[i] += 50;
		c->out_cipher = malloc(sizeof(struct isaac_context));
		init_isaac(c->out_cipher, isaac_seed);
		
		// Initialize the player.
		struct player *p = malloc(sizeof(struct player));
		init_player(p);
		i = get_int(b, ENDIAN_BIG, X_NONE); // UID
		p->username = get_string(b);
		p->password = get_string(b);
		p->client = c;
		c->player = p;
		world_register(p);
		
		// Send the response
		init_buffer(&out);
		put_byte(&out, 2, X_NONE);
		put_byte(&out, 0, X_NONE);
		put_byte(&out, 0, X_NONE);
		client_send(c, &out);
		c->state = LOGGED_IN;
		
		// Notify the Python scripting system of this event.
		PyObject *func = PyDict_GetItemString(py_dict, "on_login");
		PyObject_CallFunction(func, "l", p);
		break;
	}
}

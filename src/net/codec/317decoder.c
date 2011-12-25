#include "decoder.h"

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
	 	// TODO: Pull it from a packet length array.
	 	
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
	
	// Reset the packet for the next initialization.
	packet->opcode = -1;
	packet->length = -1;
	packet->payload->size = 0;
	packet->payload->position = 0;
}

void login_decode(struct client *c, struct buffer *b) {
	int request;
	switch (c->state) {
	case CONNECTED:
		// Verify the service request.
		request = get_byte(b, X_NONE);
		if (request != 14) {
			printf("Invalid service request: %d\n", request);
			c->state = DISCONNECTED;
			return;
		}
		break;
	case LOGGING_IN:
		break;
	}
}

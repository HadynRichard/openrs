#include "protocol.h"

#include "packet.h"
#include "encoder.h"

#include <stdlib.h>
#include <string.h>

void send_message(struct client *c, char *msg) {
	// Initialize the packet
	struct packet *p = malloc(sizeof(struct packet));
	init_packet(p);
	p->opcode = 253;
	p->length_type = LENGTH_BYTE;
	
	// Write the string
	put_string(p->payload, msg);
	p->length = p->payload->position;
	
	// Encode and send it
	struct buffer *b = encode(c, p);
	client_send(c, b);
}

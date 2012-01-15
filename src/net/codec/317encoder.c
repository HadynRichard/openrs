#include "encoder.h"

#include <string.h>
#include <stdlib.h>

struct buffer *encode(struct client *c, struct packet *p) {
	// Calculate the total packet length
	int length = 1 + p->length;
	if (p->length_type == LENGTH_BYTE)
		length += 1;
	else if (p->length_type == LENGTH_SHORT)
		length += 2;
	
	// Manually initialize the buffer
	struct buffer *b = malloc(sizeof(struct buffer));
	b->capacity = length;
	b->data = malloc(length);
	b->position = 0;
	b->size = 0;
	
	// Encode the packet
	put_byte(b, p->opcode += isaac_next_int(c->out_cipher), X_NONE);
	
	if (p->length_type == LENGTH_BYTE)
		put_byte(b, p->length, X_NONE);
	else if (p->length_type == LENGTH_SHORT)
		put_short(b, p->length, ENDIAN_BIG, X_NONE);
	
	int i;
	for (i = 0; i < p->length; i++)
		put_byte(b, p->payload->data[i], X_NONE);
	
	// And produce it
	return b;
}

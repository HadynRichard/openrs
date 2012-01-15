#include "packet.h"

#include <stdlib.h>

void init_packet(struct packet *p) {
	p->opcode = -1;
	p->length = -1;
	p->length_type = LENGTH_FIXED;
	p->payload = malloc(sizeof(struct packet));
	init_buffer(p->payload);
}

void free_packet(struct packet *p) {
	free_buffer(p->payload);
	free(p);
}

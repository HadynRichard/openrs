#include "packet_handler.h"

#include <stdio.h>

// Handles a default packet.
inline void handle_default(struct client *c, struct packet *p) {
	printf("Unhandled packet [opcode:%d length:%d]\n", p->opcode, p->length);
}

void handle_packet(struct client *c, struct packet *p) {
	// Lazy initialization of the handlers array
	if (handlers == NULL) {
		handlers = malloc(255 * sizeof(packet_handler));
		// TODO: Map handlers
		
		// Make the rest of the handlers default.
		int idx;
		for (idx = 0; idx < 255; idx++)
			if (handlers[idx] == NULL)
				handlers[idx] = handle_default;
	}
}

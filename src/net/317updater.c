#include "updater.h"

#include "packet.h"
#include "client.h"

#include <stdlib.h>

void update_players(struct player *player) {
	struct packet *p = malloc(sizeof(struct packet));
	init_packet(p);
	p->opcode = 81;
	p->length_type = LENGTH_SHORT;

	bit_access_open(p->payload);
	put_bits(p->payload, 1, 0);
	put_bits(p->payload, 8, 0);
	bit_access_close(p->payload);
	
	// Encode and send it
	p->length = p->payload->position;
	struct buffer *b = encode(player->client, p);
	client_send(player->client, b);

	// Clean up
	free_buffer(b);
}
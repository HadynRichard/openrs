#include "world.h"

#include <stdlib.h>

void world_register(struct player *p) {
	int i;
	for (i = 1; i <= MAX_PLAYERS; i++)
		if (!players[i]) {
			p->id = i;
			players[i] = p;
			break;
		}
}

void world_unregister(struct player *p) {
	players[p->id] = 0;
	p->id = 0;
}

void world_init() {
	players = malloc(sizeof(struct player *) * (MAX_PLAYERS + 1));
	
	// Clear out the players pointers.
	int i;
	for (i = 1; i <= MAX_PLAYERS; i++)
		players[i] = 0;
}

void world_free() {
	free(players);
}

void world_tick() {
}

void world_sync() {
}

void world_reset() {
}

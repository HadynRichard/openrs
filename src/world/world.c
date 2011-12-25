#include "world.h"

#include <stdlib.h>

void world_init() {
	players = malloc(sizeof(struct player) * 2000);
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

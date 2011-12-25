#ifndef WORLD_H
#define WORLD_H

#define MAX_PLAYERS 2000

#include "player.h"

struct player *players;

void world_init();

void world_free();

void world_tick();

void world_sync();

void world_reset();

#endif // #ifndef WORLD_H

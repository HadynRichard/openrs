#include "player.h"

#include <stdlib.h>

#include "world.h"

void init_player(struct player *p) {
	p->id = 0;
	p->client = 0;
	p->username = 0;
	p->password = 0;
}

void free_player(struct player *p) {
	if (p->username)
		free(p->username);
	if (p->password)
		free(p->password);
}

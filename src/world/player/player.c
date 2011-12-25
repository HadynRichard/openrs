#include "player.h"

void init_player(struct player *p) {
}

void free_player(struct player *p) {
	free(p->username);
	free(p->password);
}

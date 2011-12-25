#ifndef PLAYER_H
#define PLAYER_H

struct player {
	struct client *c;
	char *username;
	char *password;
};

void init_player(struct player *p);

void free_player(struct player *p);

#endif // #ifndef PLAYER_H

#ifndef PACKET_H
#define PACKET_H

struct packet {
	int opcode;
	int length;
	struct buffer *payload;
};

void init_packet(struct packet *p);

void free_packet(struct packet *p);

#endif // #ifndef PACKET_H

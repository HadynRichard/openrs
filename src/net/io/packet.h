#ifndef PACKET_H
#define PACKET_H

#define LENGTH_FIXED 0
#define LENGTH_BYTE 1
#define LENGTH_SHORT 2

struct packet {
	int opcode;
	int length;
	int length_type;
	struct buffer *payload;
};

void init_packet(struct packet *p);

void free_packet(struct packet *p);

#endif // #ifndef PACKET_H

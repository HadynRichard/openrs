#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include "client.h"
#include "packet.h"

// Defines a packet handler function.
typedef void (*packet_handler)(struct client *c, struct packet *p);

// Packet handlers array.
packet_handler *handlers;

// Gets the packet handler.
void handle_packet(struct client *c, struct packet *p);

#endif

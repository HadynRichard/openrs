#ifndef ENCODER_H
#define ENCODER_H

#include "client.h"
#include "buffer.h"
#include "packet.h"

// Encodes packet and returns a buffer
struct buffer *decode(struct client *c, struct packet *p);

#endif 

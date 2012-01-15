#ifndef DECODER_H
#define DECODER_H

#include "client.h"
#include "buffer.h"
#include "packet.h"

// Decodes the buffer into a packet.
int decode(struct client *c, struct buffer *b);

// Decodes the login process.
void login_decode(struct client *c, struct buffer *b);

#endif // #ifndef DECODER_H

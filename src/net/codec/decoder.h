#ifndef _317_DECODER
#define _317_DECODER

#include "client.h"
#include "buffer.h"
#include "packet.h"

// Decodes the buffer into a packet.
void decode(struct client *c, struct buffer *b);

// Decodes the login process.
void login_decode(struct client *c, struct buffer *b);

#endif // #ifndef _317_DECODER

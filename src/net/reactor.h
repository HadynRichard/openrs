#ifndef REACTOR_H
#define REACTOR_H

#define MAX_CLIENTS 2000
#define SERVER_PORT 43594

// The epoll file descriptor
int epfd;

// Initializes the reactor system.
int reactor_init();

// Performs a reactor poll operation.
void reactor_poll();

#endif // #ifndef REACTOR_H

#include "reactor.h"
#include "client.h"

#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

struct epoll_event *events;
int serversockfd;

int reactor_init() {
	int ON = 1, OFF = 0;
	
	// Initialize the server socket.
	if ((serversockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket()");
		return 0;
	}
	
	// Configure the socket for asynchronous operation.
	if (fcntl(serversockfd, F_SETFL, O_NONBLOCK) == -1) {
		perror("fcntl()");
		return 0;
	}
	
	// Set the SO_REUSEADDR flag.
	if (setsockopt(serversockfd, SOL_SOCKET, SO_REUSEADDR, &ON, sizeof(int)) == -1) {
		perror("setsockopt()");
		return 0;
	}
	
	// Configure and bind the address.
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(SERVER_PORT);
	memset(&(addr.sin_zero), '\0', 8);
	if (bind(serversockfd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("bind()");
		return 0;
	}
	
	// Configure as a listener socket.
	if (listen(serversockfd, MAX_CLIENTS) == -1) {
		perror("listen()");
		return 0;
	}
	
	// Initialize the epoll system.
	events = malloc(MAX_CLIENTS * sizeof(struct epoll_event));
	epfd = epoll_create(MAX_CLIENTS);
	if (!epfd) {
		perror("epoll_create()");
		return 0;
	}
	
	// Register the server socket with epoll.
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = serversockfd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, serversockfd, &ev) < 0) {
		perror("epoll_ctl()");
		return 0;
	}
	return 1;
}

void do_accept() {
	struct sockaddr_in *addr = malloc(sizeof(struct sockaddr_in));
	int addrlen = sizeof(addr);
	int sockfd;
	
	// Accept the client.
	if ((sockfd = accept(serversockfd, (struct sockaddr *) addr, &addrlen)) == -1) {
		perror("accept()");
		return;
	}
	
	// Configure the socket for asynchronous operation.
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) {
		perror("fcntl()");
		return;
	}
	
	// Initialize the client.
	printf("Accepted client from %s\n", inet_ntoa(addr->sin_addr));
	struct epoll_event *ev = malloc(sizeof(struct epoll_event));
	struct client *c = malloc(sizeof(struct client));
	ev->data.fd = sockfd;
	ev->events = EPOLLIN;
	ev->data.ptr = c;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, ev) < 0) {
		perror("epoll_ctl()");
		return;
	}
	c->sockfd = sockfd;
	c->addr = addr;
	c->epollev = ev;
	init_client(c);
}

void reactor_poll() {
	int i;
	int nfds = epoll_wait(epfd, events, MAX_CLIENTS, 0);
	for (i = 0; i < nfds; i++) {
		// Handle accepting an incoming connection.
		if (events[i].data.fd == serversockfd) {
			do_accept();
		} else {
			// Handle reading data.
			if (events[i].events & EPOLLIN)
				do_read((struct client *) events[i].data.ptr);
			
			// Handle writing data.
			else if (events[i].events & EPOLLOUT)
				do_write((struct client *) events[i].data.ptr);
		}
	}
}

#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

struct thread {
	pthread_t *pthread;
	(void *) function;
};

#endif

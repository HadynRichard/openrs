#include <sys/time.h>
#include <unistd.h>

#include "core.h"
#include "reactor.h"

void core_run() {
	struct timeval start, end;
	long mtime, seconds, useconds;
	
	// Main server loop.
	for (;;) {
		gettimeofday(&start, NULL);
		printf("tick\n");
		
		// First, handle network events.
		reactor_poll();
		
		// Tick world logic.
		world_tick();
	
		// Synchronize clients.
		world_sync();
	
		// Reset the world.
		world_reset();
		
		// Sleep until the next cycle.
		gettimeofday(&end, NULL);
		seconds = end.tv_sec - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;
		mtime = ((seconds) * 1000 + useconds / 1000.0) + 0.5;
		usleep((TICKRATE - mtime) * 1000);
	}
}

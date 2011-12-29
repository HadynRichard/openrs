#include "script_loader.h"

#include <stdio.h>

#include "reactor.h"
#include "world.h"

int main(void) {
	// Perform the startup procedure.
	printf("Starting OpenRS emulator...\n");
	
	// First initialize the world.
	printf("\tInitializing the game world\t");
	world_init();
	printf("\t\t[OK]\n");
	
	// Initialize the script system.
	printf("\tLoading Python scripting system\t");
	if (!load_scripts()) {
		printf("\nUnable to start.\n");
		return -1;
	}
	printf("\t\t[OK]\n");
	
	// Initialize the reactor system.
	printf("\tInitializing reactor networking system");
	if (!reactor_init()) {
		printf("\nUnable to start.\n");
		return -1;
	}
	printf("\t\t[OK]\n");
	
	// All done, begin the core execution.
	printf("Startup complete.\n");
	core_run();
	return 0;
}

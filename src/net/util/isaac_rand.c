#include "isaac_rand.h"

#include <stdlib.h>

#define mix(a, b, c, d, e, f, g, h) \
{ \
   a ^= b << 11; d += a; b += c; \
   b ^= c >> 2;  e += b; c += d; \
   c ^= d << 8;  f += c; d += e; \
   d ^= e >> 16; g += d; e += f; \
   e ^= f << 10; h += e; f += g; \
   f ^= g >> 4;  a += f; g += h; \
   g ^= h << 8;  b += g; h += a; \
   h ^= a >> 9;  c += h; a += b; \
}

void do_isaac(struct isaac_context *isaac) {
	register int32_t i, x, y;
	isaac->cc += 1;
	isaac->bb += isaac->cc;
	
	for (i = 0; i < 256; i++) {
		x = isaac->memory[i];
		switch (i % 4) {
		case 0:
			isaac->aa = isaac->aa ^ (isaac->aa << 13);
			break;
		case 1:
			isaac->aa = isaac->aa ^ (isaac->aa >> 6);
			break;
		case 2:
			isaac->aa = isaac->aa ^ (isaac->aa << 2);
			break;
		case 3:
			isaac->aa = isaac->aa ^ (isaac->aa >> 16);
			break;
		}
		isaac->aa = isaac->memory[(i + 128) % 256] + isaac->aa;
     	isaac->memory[i] = y = isaac->memory[(x >> 2) % 256] + isaac->aa + isaac->bb;
     	isaac->results->results[i] = isaac->bb = isaac->memory[(y >> 10) % 256] + x;
	}
}

void init_isaac(struct isaac_context *isaac, int32_t *seed) {
	isaac->memory = calloc(256, sizeof(int32_t));
	isaac->results = malloc(sizeof(struct isaac_results));
	isaac->results->results = calloc(256, sizeof(int32_t));
	
	// First, apply the seed
	int i;
	for (i = 0; i < 4; i++)
		isaac->results->results[i] = seed[i];
	
	register int32_t a, b, c, d, e, f, g, h;
	a = b = c = d = e = f= g = h = 0x9e3779b9;
	
	// Clear out the counters
	isaac->aa = 0;
	isaac->bb = 0;
	isaac->cc = 0;
	
	// Scamble the initial values
	for (i = 0; i < 4; i++) {
		mix(a, b, c, d, e, f, g, h);
	}
	
	// Fill in the memory with messy stuff
	for (i = 0; i < 256; i += 8) {
		// Fill in the information from the seed
		a += isaac->results->results[i];
		b += isaac->results->results[i + 1];
		c += isaac->results->results[i + 2];
		d += isaac->results->results[i + 3];
		e += isaac->results->results[i + 4];
		f += isaac->results->results[i + 5];
		g += isaac->results->results[i + 6];
		h += isaac->results->results[i + 7];
		
		// Mix it up
		mix(a, b, c, d, e, f, g, h)
		
		// Fill the memory
		isaac->memory[i] = a;
		isaac->memory[i + 1] = b;
		isaac->memory[i + 2] = c;
		isaac->memory[i + 3] = d;
		isaac->memory[i + 4] = e;
		isaac->memory[i + 5] = f;
		isaac->memory[i + 6] = g;
		isaac->memory[i + 7] = h;
	}
	
	// Perform a second pass
	for (i = 0; i < 256; i+= 8) {
		a += isaac->memory[i];
		b += isaac->memory[i + 1];
		c += isaac->memory[i + 2];
		d += isaac->memory[i + 3];
		e += isaac->memory[i + 4];
		f += isaac->memory[i + 5];
		g += isaac->memory[i + 6];
		h += isaac->memory[i + 7];
		
		// Scamble it again
		mix(a, b, c, d, e, f, g, h);
		
		// Insert the values into the memory
		isaac->memory[i] = a;
		isaac->memory[i + 1] = b;
		isaac->memory[i + 2] = c;
		isaac->memory[i + 3] = d;
		isaac->memory[i + 4] = e;
		isaac->memory[i + 5] = f;
		isaac->memory[i + 6] = g;
		isaac->memory[i + 7] = h;
	}
	
	do_isaac(isaac);
	isaac->results->position = 256;
}

void free_isaac(struct isaac_context *isaac) {
	free(isaac->results->results);
	free(isaac->results);
	free(isaac);
}

int32_t isaac_next_int(struct isaac_context *isaac) {
	if (isaac->results->position-- == 0) {
		do_isaac(isaac);
		isaac->results->position = 255;
	}
	return isaac->results->results[isaac->results->position];
}

#ifndef ISAAC_RAND
#define ISAAC_RAND

#include <stdint.h>

/*
 * The results of an ISAAC number generator.
 */
struct isaac_results {
	uint32_t *results;
	uint32_t position;
};

/*
 * The context of an ISAAC number generator.
 */
struct isaac_context {
	struct isaac_results *results;
	uint32_t *memory;
	uint32_t aa;
	uint32_t bb;
	uint32_t cc;
};

// Initializes the isaac cipher for the context.
void init_isaac(struct isaac_context *, int32_t *seed);

// Frees the isaac cipher context.
void free_isaac(struct isaac_context *);

// Gets a random integer from the isaac cipher context.
int32_t isaac_next_int(struct isaac_context *);

#endif

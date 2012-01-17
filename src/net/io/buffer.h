#ifndef BUFFER_H
#define BUFFER_H

#include <sys/types.h>

#define BUFFER_SIZE 4096

#define ENDIAN_BIG 0
#define ENDIAN_LITTLE 1

#define X_NONE 0
#define X_ADD 1
#define X_SUBTRACT 2
#define X_INVERT 3

// Defines a network buffer.
struct buffer {
	int size; // Amount of data in the buffer.
	char *data; // The actual character array.
	int capacity; // The overall capacity of the buffer.
	int position; // The current position in the buffer.
	int bit_position;
};

// Initializes a buffer.
void init_buffer(struct buffer *b);

// Frees a buffer.
void free_buffer(struct buffer *b);

// Opens bit access to the buffer.
void bit_access_open(struct buffer *b);

// Closes bit access to the buffer.
void bit_access_close(struct buffer *b);

// Puts bits into the buffer.
void put_bits(struct buffer *b, int amt, int value);

// Puts a string into the buffer.
void put_string(struct buffer *b, char *str);

// Gets a string from the buffer.
char *get_string(struct buffer *b);

// Gets a byte from the buffer.
int32_t get_byte(struct buffer *b, int xform);

// Puts a byte into the buffer.
void put_byte(struct buffer *b, int32_t value, int xform);

// Gets a short from the buffer.
int32_t get_short(struct buffer *b, int endian, int xform);

// Puts a short into the buffer.
void put_short(struct buffer *b, int32_t value, int endian, int xform);

// Gets an int from the buffer.
int32_t get_int(struct buffer *b, int endian, int xform);

// Puts an int into the buffer.
void put_int(struct buffer *b, int32_t value, int endian, int xform);

// Gets a long from the buffer.
int64_t get_long(struct buffer *b, int endian, int xform);

// Puts a long into the buffer.
void put_long(struct buffer *b, int64_t value, int endian, int xform);

#endif // #ifndef BUFFER_H


#include "buffer.h"

#include <stdlib.h>

void init_buffer(struct buffer *b) {
	b->capacity = BUFFER_SIZE;
	b->data = (char *) malloc(BUFFER_SIZE);
	b->position = 0;
	b->size = 0;
}

void free_buffer(struct buffer *b) {
	free(b->data);
	free(b);
}

void put_string(struct buffer *b, char *str) {
	char c;
	int idx = 0;
	while ((c = str[idx++]) != '\0')
		put_byte(b, c, X_NONE);
	put_byte(b, '\n', X_NONE);
}

// Note: Supports a maximum of 256 characters
char *get_string(struct buffer *b) {
	char *str = malloc(256);
	int idx = 0;
	char c;
	while ((c = get_byte(b, X_NONE)) != '\n' && idx < 256)
		str[idx++] = c;
	str[idx] = '\0';
	return str;
}

int32_t get_byte(struct buffer *b, int xform) {
	if (b->position + 1 >= b->capacity) {
		perror("Buffer underflow");
		return 0;
	}
	int32_t value = 0;
	value = b->data[b->position++];
	switch (xform) {
	case X_ADD:
		value -= 128;
		break;
	case X_SUBTRACT:
		value = 128 - value;
		break;
	case X_INVERT:
		value = -value;
		break;
	}
	return value;
}

void put_byte(struct buffer *b, int32_t value, int xform) {
	if (b->position + 1 >= b->capacity) {
		perror("Buffer overflow");
		return; // Nope.
	}
	switch (xform) {
	case X_ADD:
		value += 128;
		break;
	case X_SUBTRACT:
		value = 128 - value;
		break;
	case X_INVERT:
		value = -value;
		break;
	}
	b->data[b->position++] = value;
}

int32_t get_short(struct buffer *b, int endian, int xform) {
	int32_t value = 0;
	switch (endian) {
	case ENDIAN_BIG:
		value |= (get_byte(b, X_NONE) & 0xff) << 8;
		value |= (get_byte(b, xform) & 0xff);
		break;
	case ENDIAN_LITTLE:
		value |= (get_byte(b, xform) & 0xff);
		value |= (get_byte(b, X_NONE) & 0xff) << 8;
		break;
	}
	return value;
}

void put_short(struct buffer *b, int32_t value, int endian, int xform) {
	switch (endian) {
	case ENDIAN_BIG:
		put_byte(b, value >> 8, X_NONE);
		put_byte(b, value, xform);
		break;
	case ENDIAN_LITTLE:
		put_byte(b, value, xform);
		put_byte(b, value >> 8, X_NONE);
		break;
	}
}

int32_t get_int(struct buffer *b, int endian, int xform) {
	int32_t value = 0;
	switch (endian) {
	case ENDIAN_BIG:
		value |= (get_byte(b, X_NONE) & 0xff) << 24;
		value |= (get_byte(b, X_NONE) & 0xff) << 16;
		value |= (get_byte(b, X_NONE) & 0xff) << 8;
		value |= (get_byte(b, xform) & 0xff);
		break;
	case ENDIAN_LITTLE:
		value |= (get_byte(b, xform) & 0xff);
		value |= (get_byte(b, X_NONE) & 0xff) << 8;
		value |= (get_byte(b, X_NONE) & 0xff) << 16;
		value |= (get_byte(b, X_NONE) & 0xff) << 24;
		break;
	}
	return value;
}

void put_int(struct buffer *b, int32_t value, int endian, int xform) {
	switch (endian) {
	case ENDIAN_BIG:
		put_byte(b, value >> 24, X_NONE);
		put_byte(b, value >> 16, X_NONE);
		put_byte(b, value >> 8, X_NONE);
		put_byte(b, value, xform);
		break;
	case ENDIAN_LITTLE:
		put_byte(b, value, xform);
		put_byte(b, value >> 8, X_NONE);
		put_byte(b, value >> 16, X_NONE);
		put_byte(b, value >> 24, X_NONE);
		break;
	}
}

int64_t get_long(struct buffer *b, int endian, int xform) {
	int64_t value = 0;
	switch (endian) {
	case ENDIAN_BIG:
		value |= (int64_t) (get_byte(b, X_NONE) & 0xff) << 56L;
		value |= (int64_t) (get_byte(b, X_NONE) & 0xff) << 48L;
		value |= (int64_t) (get_byte(b, X_NONE) & 0xff) << 40L;
		value |= (int64_t) (get_byte(b, X_NONE) & 0xff) << 32L;
		value |= (int64_t) (get_byte(b, X_NONE) & 0xff) << 24L;
		value |= (int64_t) (get_byte(b, X_NONE) & 0xff) << 16L;
		value |= (int64_t) (get_byte(b, X_NONE) & 0xff) << 8L;
		value |= (int64_t) (get_byte(b, xform) & 0xff);
		break;
	case ENDIAN_LITTLE:
		value |= (int64_t) (get_byte(b, xform) & 0xff);
		value |= (int64_t) (get_byte(b, X_NONE) & 0xff) << 8L;
		value |= (int64_t) (get_byte(b, X_NONE) & 0xff) << 16L;
		value |= (int64_t) (get_byte(b, X_NONE) & 0xff) << 24L;
		value |= (int64_t) (get_byte(b, X_NONE) & 0xff) << 32L;
		value |= (int64_t) (get_byte(b, X_NONE) & 0xff) << 40L;
		value |= (int64_t) (get_byte(b, X_NONE) & 0xff) << 48L;
		value |= (int64_t) (get_byte(b, X_NONE) & 0xff) << 56L;
		break;
	}
	return value;
}

void put_long(struct buffer *b, int64_t value, int endian, int xform) {
	switch (endian) {
	case ENDIAN_BIG:
		put_byte(b, value >> 56L, X_NONE);
		put_byte(b, value >> 48L, X_NONE);
		put_byte(b, value >> 40L, X_NONE);
		put_byte(b, value >> 32L, X_NONE);
		put_byte(b, value >> 24L, X_NONE);
		put_byte(b, value >> 16L, X_NONE);
		put_byte(b, value >> 8L, X_NONE);
		put_byte(b, value, xform);
		break;
	case ENDIAN_LITTLE:
		put_byte(b, value, xform);
		put_byte(b, value >> 8, X_NONE);
		put_byte(b, value >> 16, X_NONE);
		put_byte(b, value >> 24, X_NONE);
		put_byte(b, value >> 32, X_NONE);
		put_byte(b, value >> 40, X_NONE);
		put_byte(b, value >> 48, X_NONE);
		put_byte(b, value >> 56, X_NONE);
		break;
	}
}

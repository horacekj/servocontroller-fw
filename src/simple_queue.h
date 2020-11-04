#ifndef _SIMPLE_QUEUE_H_
#define _SIMPLE_QUEUE_H_

/* Simple queue. */

#include <stdint.h>
#include <stdbool.h>

#define MAX_LENGTH 4

typedef struct {
	uint8_t data[MAX_LENGTH];
	uint8_t length;
} SimpleQueue;

typedef SimpleQueue SQ;

void sq_init(SQ*);
void sq_enqueue(SQ*, uint8_t value);
uint8_t sq_dequeue(SQ*);
bool sq_empty(SQ*);
uint8_t sq_size(SQ*);
uint8_t sq_front(SQ*);
bool sq_full(SQ*);
bool sq_contains(SQ*, uint8_t value);

#endif

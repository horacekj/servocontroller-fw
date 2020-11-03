#include "simple_queue.h"

void sq_init(SQ* sq) {
	sq->length = 0;
}

void sq_enqueue(SQ* sq, uint8_t value) {
	if (sq_full(sq))
		return;

	sq->data[sq->length] = value;
	sq->length++;
}

uint8_t sq_dequeue(SQ* sq) {
	if (sq_empty(sq))
		return 0;
	uint8_t value = sq->data[0];
	for (uint8_t i = 0; i < sq->length-1; i++)
		sq->data[i] = sq->data[i+1];
	sq->length--;
	return value;
}

bool sq_empty(SQ* sq) {
	return sq->length == 0;
}

uint8_t sq_size(SQ* sq) {
	return sq->length;
}

uint8_t sq_front(SQ* sq) {
	if (!sq_empty(sq))
		return sq->data[0];
	return 0;
}

bool sq_full(SQ* sq) {
	return sq->length >= MAX_LENGTH;
}

bool sq_contains(SQ* sq, uint8_t value) {
	for (uint8_t i = 0; i < sq->length; i++)
		if (sq->data[i] == value)
			return true;
	return false;
}

#ifndef _TURNOUT_H_
#define _TURNOUT_H_

#include <stdint.h>

typedef enum {
	tpPlus = 0,
	tpMinus = 1,
	tpMovingToPlus = 2,
	tpMovingToMinus = 3
} TurnoutPos;

typedef struct {
	uint8_t pin_pot;
	uint8_t pin_led;
	uint8_t pin_servo;
	uint8_t pin_button;

	TurnoutPos position; // TODO: reset on init
	int8_t debounce_val; // TODO: reset on init
	int16_t angle;
	uint16_t width;
	// TODO: data for turning-off signal to servo after some time
} Turnout;

#endif

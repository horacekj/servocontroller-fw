#ifndef _TURNOUT_H_
#define _TURNOUT_H_

#include <stdint.h>
#include <stdbool.h>

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

	uint8_t index;
	TurnoutPos position; // TODO: reset on init
	int8_t debounce_val; // TODO: reset on init
	int16_t angle;
	uint16_t width;
	uint8_t btn_debounce_val;
	bool btn_pressed;
	// TODO: data for turning-off signal to servo after some time
} Turnout;

#define TURNOUTS_COUNT 1

#endif

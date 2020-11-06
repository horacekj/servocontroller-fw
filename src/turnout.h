#ifndef _TURNOUT_H_
#define _TURNOUT_H_

/* Definition of turnout structure */

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
	TurnoutPos position;
	int8_t debounce_val;
	int16_t angle;
	uint16_t width;
	uint8_t btn_debounce_val;
	bool btn_pressed;
	uint8_t blink;
	bool save_pos;
} Turnout;

#endif

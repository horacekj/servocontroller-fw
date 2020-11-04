#include "turnouts.h"
#include "io.h"

Turnout turnouts[TURNOUTS_COUNT] = {
	{ // servo 1 [0]
		.pin_pot = IO_PINC0,
		.pin_led = IO_PINB4,
		.pin_servo = IO_PIND5,
		.pin_button = IO_PIND4,
	},
	{ // servo 2 [1]
		.pin_pot = IO_PINC1,
		.pin_led = IO_PINB3,
		.pin_servo = IO_PIND6,
		.pin_button = IO_PIND3,
	},
	{ // servo 3 [2]
		.pin_pot = IO_PINC2,
		.pin_led = IO_PINB1,
		.pin_servo = IO_PIND7,
		.pin_button = IO_PIND1,
	},
	{ // servo 4 [3]
		.pin_pot = IO_PINC3,
		.pin_led = IO_PINB2,
		.pin_servo = IO_PINB0,
		.pin_button = IO_PIND2,
	},
};

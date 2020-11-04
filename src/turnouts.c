#include "turnouts.h"
#include "io.h"

Turnout turnouts[TURNOUTS_COUNT] = {
	{ // servo 3 [0]
		.pin_pot = IO_PINC2,
		.pin_led = IO_PINB1,
		.pin_servo = IO_PIND6,
		.pin_button = IO_PIND1,
	},
};

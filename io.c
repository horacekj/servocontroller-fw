#include <avr/io.h>
#include "io.h"

void set_output(uint8_t pin, bool state) {
	if (pin >= IO_PINB0 && pin <= IO_PINB7) {
		if (state)
			PORTB |= (1 << (pin-IO_PINB0));
		else
			PORTB &= ~(1 << (pin-IO_PINB0));
	} else if (pin >= IO_PINC0 && pin <= IO_PINC7) {
		if (state)
			PORTC |= (1 << (pin-IO_PINC0));
		else
			PORTC &= ~(1 << (pin-IO_PINC0));
	} else if (pin >= IO_PIND0 && pin <= IO_PIND7) {
		if (state)
			PORTD |= (1 << (pin-IO_PIND0));
		else
			PORTD &= ~(1 << (pin-IO_PIND0));
	}
}

bool get_input(uint8_t pin) {
	if (pin >= IO_PINB0 && pin <= IO_PINB7)
		return (PINB >> (pin-IO_PINB0)) & 1;
	if (pin >= IO_PINC0 && pin <= IO_PINC7)
		return (PINC >> (pin-IO_PINC0)) & 1;
	if (pin >= IO_PIND0 && pin <= IO_PIND7)
		return (PINC >> (pin-IO_PIND0)) & 1;

	return false;
}

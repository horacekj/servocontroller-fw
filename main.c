/* Main source file of Servocontroller ATmega CPU.
 */

#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "io.h"
#include "pwm_servo_gen.h"
#include "turnout.h"

///////////////////////////////////////////////////////////////////////////////

#define TURNOUTS_COUNT 1
Turnout turnouts[TURNOUTS_COUNT] = {
	{ // servo 1 [0]
		.pin_pot = IO_PINC0,
		.pin_led = IO_PINB4,
		.pin_servo = IO_PINB0,
		.pin_button = IO_PIND4,
	},
};

#define DEBOUNCE_READS 20 // also time in ms

///////////////////////////////////////////////////////////////////////////////

int main();
static inline void init();

void eeprom_load_all_pos();
void eeprom_store_pos(Turnout*);

void debounce_update(Turnout*);
void on_btn_pressed(Turnout*);

void leds_update(Turnout*);
void move_update(Turnout*);

// TODO: servo angle update on potentiometer value changed (and possibly change servo position)

///////////////////////////////////////////////////////////////////////////////

int main() {
	init();
	pwm_servo_gen(IO_PIND5, 0);
	pwm_servo_gen(IO_PIND6, 0);

	while (true) {
		// wdt_reset();
	}
}

static inline void init() {
	ACSR |= ACD;  // analog comparator disable
	// WDTCR |= 1 << WDE;  // watchdog enable
	// WDTCR |= WDP2; // ~250 ms timeout

	DDRD |= (1 << PD5) | (1 << PD6) | (1 << PD7);
	DDRB |= (1 << PB0);

	PORTD |= (1 << PD1) | (1 << PD2) | (1 << PD3) | (1 << PD4);

	// Setup timer 0 TODO
	TCCR0A |= 1 << WGM01; // CTC mode
	TCCR0B |= 1 << CS01; // no prescaler
	TIMSK0 |= 1 << OCIE0A; // enable compare match A
	OCR0A = 99;

	pwm_servo_init();

	sei(); // enable interrupts globally
}

///////////////////////////////////////////////////////////////////////////////

ISR(TIMER0_COMPA_vect) {
	// Timer 0 @ 10 kHz (period 100 us)
}

///////////////////////////////////////////////////////////////////////////////

void eeprom_load_all_pos() {
}

void eeprom_store_pos(Turnout* turnout) {
}

///////////////////////////////////////////////////////////////////////////////

void debounce_update(Turnout* turnout) {
}

///////////////////////////////////////////////////////////////////////////////

void on_btn_pressed(Turnout* turnout) {
}

///////////////////////////////////////////////////////////////////////////////

void leds_update(Turnout* turnout) {
}

void move_update(Turnout* turnout) {
}

///////////////////////////////////////////////////////////////////////////////

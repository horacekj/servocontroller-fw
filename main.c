/* Main source file of Servocontroller ATmega CPU.
 */

#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "io.h"
#include "pwm_servo_gen.h"
#include "turnout.h"
#include "switch.h"

///////////////////////////////////////////////////////////////////////////////

#define TURNOUTS_COUNT 1
Turnout turnouts[TURNOUTS_COUNT] = {
	{ // servo 1 [0]
		.pin_pot = IO_PINC0,
		.pin_led = IO_PINB1,
		.pin_servo = IO_PIND6,
		.pin_button = IO_PIND4,
		.angle = 0, // TODO
		.width = 1000, // TODO
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

void leds_update_20ms(Turnout*);

// TODO: servo angle update on potentiometer value changed (and possibly change servo position)

///////////////////////////////////////////////////////////////////////////////

int main() {
	init();
	pwm_servo_gen(IO_PIND6, 0);
	_delay_ms(500);

	while (true) {
		// wdt_reset();
		switch_turnout(&turnouts[0], tpPlus);
		while (is_switching());
		_delay_ms(1000);
		switch_turnout(&turnouts[0], tpMinus);
		while (is_switching());
		_delay_ms(1000);
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
	TCCR0B |= (1 << CS01) | (1 << CS00); // 64× prescaler
	TIMSK0 |= 1 << OCIE0A; // enable compare match A
	OCR0A = 127;

	pwm_servo_init();

	sei(); // enable interrupts globally
}

///////////////////////////////////////////////////////////////////////////////

ISR(TIMER0_COMPA_vect) {
	// Timer 0 @ 1 kHz (period 1 ms)
	static volatile uint8_t counter_20ms = 0;
	counter_20ms++;
	if (counter_20ms >= 20) {
		counter_20ms = 0;
		switch_update();

		for (uint8_t i = 0; i < TURNOUTS_COUNT; i++)
			leds_update_20ms(&turnouts[i]);
	}
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

void leds_update_20ms(Turnout* turnout) {
	static volatile uint8_t blink = 0;
	#define BLINK_TIMEOUT 15

	if (turnout->position == tpPlus) {
		set_dir_out(turnout->pin_led);
		set_output(turnout->pin_led, true);
		blink = 0;
	} else if (turnout->position == tpMinus) {
		set_dir_out(turnout->pin_led);
		set_output(turnout->pin_led, false);
		blink = 0;
	} else {
		blink++;
		if (blink < BLINK_TIMEOUT) {
			set_dir_out(turnout->pin_led);
			set_output(turnout->pin_led, (turnout->position == tpMovingToPlus));
		} else if (blink < BLINK_TIMEOUT*2) {
			set_dir_in(turnout->pin_led);
		} else
			blink = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////

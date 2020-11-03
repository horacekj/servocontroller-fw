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
#include "inputs.h"
#include "simple_queue.h"

///////////////////////////////////////////////////////////////////////////////

#define TURNOUTS_COUNT 1
Turnout turnouts[TURNOUTS_COUNT] = {
	{ // servo 1 [0]
		.pin_pot = IO_PINC0,
		.pin_led = IO_PINB1,
		.pin_servo = IO_PIND6,
		.pin_button = IO_PIND1,
		.position = tpPlus, // TODO
		.angle = -1000, // TODO
		.width = 1000, // TODO
		.btn_debounce_val = 0, // TODO general reset
		.btn_pressed = false, // TODO general reset
	},
};

SimpleQueue command_queue;

///////////////////////////////////////////////////////////////////////////////

int main();
static inline void init();

void eeprom_load_all_pos();
void eeprom_store_pos(Turnout*);

void leds_update_20ms(Turnout*);
void queue_poll();

// TODO: servo angle update on potentiometer value changed (and possibly change servo position)

///////////////////////////////////////////////////////////////////////////////

int main() {
	init();

	while (true) {
		queue_poll();
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
	TCCR0B |= (1 << CS01) | (1 << CS00); // 64× prescaler
	TIMSK0 |= 1 << OCIE0A; // enable compare match A
	OCR0A = 127;

	pwm_servo_init();
	sq_init(&command_queue);

	for (uint8_t i = 0; i < TURNOUTS_COUNT; i++)
		turnouts[i].index = i;

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

	for (uint8_t i = 0; i < TURNOUTS_COUNT; i++)
		button_update_1ms(&turnouts[i]);
}

///////////////////////////////////////////////////////////////////////////////

void eeprom_load_all_pos() {
}

void eeprom_store_pos(Turnout* turnout) {
}

///////////////////////////////////////////////////////////////////////////////

void btn_pressed(Turnout* turnout) {
	if (!sq_full(&command_queue) && !sq_contains(&command_queue, turnout->index)
			&& switching_turnout() != turnout)
		sq_enqueue(&command_queue, turnout->index);
}

void queue_poll() {
	if (sq_empty(&command_queue))
		return;
	if (pwm_servo_generating())
		return;

	uint8_t turnout_index = sq_dequeue(&command_queue);
	Turnout* turnout = &turnouts[turnout_index];
	if (turnout->position == tpPlus)
		switch_turnout(turnout, tpMinus);
	else if (turnout->position == tpMinus)
		switch_turnout(turnout, tpPlus);
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

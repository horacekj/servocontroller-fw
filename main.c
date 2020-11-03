/* Main source file of Servocontroller ATmega CPU.
 */

#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "io.h"
#include "pwm_servo_gen.h"

///////////////////////////////////////////////////////////////////////////////

typedef enum {
	tpPlus = 0,
	tpMinus = 1,
	tpMovingToPlus = 2,
	tpMovingToMinus = 3
} TurnoutPos;

typedef struct {
	uint8_t pinc_pot_mask;
	uint8_t pinb_led_mask;
	uint8_t pind_servo_mask;
	uint8_t pinb_servo_mask;
	uint8_t pind_btn_mask;

	TurnoutPos position; // TODO: reset on init
	int8_t debounce_val; // TODO: reset on init
	uint8_t angle; // TODO: add proper data type
	// TODO: data for turning-off signal to servo after some time
} Turnout;

#define TURNOUTS_COUNT 4
Turnout turnouts[TURNOUTS_COUNT] = {
	{ // servo 1 [0]
		.pinc_pot_mask = 1 << PC0,
		.pinb_led_mask = 1 << PB4,
		.pind_servo_mask = 0,
		.pinb_servo_mask = 1 << PB0,
		.pind_btn_mask = 1 << PD4,
	},

	{ // servo 2 [1]
		.pinc_pot_mask = 1 << PC1,
		.pinb_led_mask = 1 << PB3,
		.pind_servo_mask = 1 << PD7,
		.pinb_servo_mask = 0,
		.pind_btn_mask = 1 << PD3,
	},

	{ // servo 3 [2]
		.pinc_pot_mask = 1 << PC2,
		.pinb_led_mask = 1 << PB1,
		.pind_servo_mask = 1 << PD6,
		.pinb_servo_mask = 0,
		.pind_btn_mask = 1 << PD1,
	},

	{ // servo 4 [3]
		.pinc_pot_mask = 1 << PC3,
		.pinb_led_mask = 1 << PB2,
		.pind_servo_mask = 1 << PD5,
		.pinb_servo_mask = 0,
		.pind_btn_mask = 1 << PD2,
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

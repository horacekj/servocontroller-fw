/* Main source file of Servocontroller ATmega CPU.
 */

#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

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
	uint8_t servo_pin; // TODO
	uint8_t pind_btn_mask;

	TurnoutPos position; // TODO: reset on init
	int8_t debounce_val; // TODO: reset on init
	uint8_t angle; // TODO: add proper data type
	// TODO: data for turning-off signal to servo after some time
} Turnout;

#define TURNOUTS_COUNT 4
Turnout turnouts[TURNOUTS_COUNT];

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

	while (true) {
		// wdt_reset();
	}
}

static inline void init() {
	ACSR |= ACD;  // analog comparator disable
	// WDTCR |= 1 << WDE;  // watchdog enable
	// WDTCR |= WDP2; // ~250 ms timeout

	// Setup timer 0
	TCCR0A |= 1 << WGM01; // CTC mode
	TCCR0B |= 1 << CS01; // no prescaler
	TIMSK0 |= 1 << OCIE0A; // enable compare match A
	OCR0A = 99;

	sei(); // enable interrupts globally
}

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

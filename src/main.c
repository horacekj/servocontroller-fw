/* Main source file of Servocontroller ATmega CPU.
 */

#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "io.h"
#include "pwm_servo_gen.h"
#include "turnout.h"
#include "switch.h"
#include "inputs.h"
#include "simple_queue.h"
#include "turnouts.h"

///////////////////////////////////////////////////////////////////////////////

#define EEPROM_POS_START 0
#define EEPROM_ANGLE_START 32

SimpleQueue command_queue;

///////////////////////////////////////////////////////////////////////////////

int main();
static inline void init();

void eeprom_load_all_pos();
void eeprom_store_pos(Turnout*);

void leds_update_20ms(Turnout*);
void queue_poll();

///////////////////////////////////////////////////////////////////////////////

int main() {
	volatile uint16_t adc_counter = 0;
	#define ADC_TIMEOUT 100 // cca in ms
	init();

	while (true) {
		queue_poll();
		_delay_ms(1);

		adc_counter++;
		if (adc_counter >= ADC_TIMEOUT) {
			adc_read_all();
			adc_counter = 0;
		}

		wdt_reset();
	}
}

static inline void init() {
	ACSR |= ACD;  // analog comparator disable
	WDTCSR |= 1 << WDE;  // watchdog enable
	WDTCSR |= WDP2; // ~250 ms timeout

	// Setup timer 0
	TCCR0A |= 1 << WGM01; // CTC mode
	TCCR0B |= (1 << CS01) | (1 << CS00); // 64× prescaler
	OCR0A = 127;

	eeprom_load_all_pos();
	pwm_servo_init();
	sq_init(&command_queue);
	adc_init();

	for (uint8_t i = 0; i < TURNOUTS_COUNT; i++) {
		pin_mode(turnouts[i].pin_pot, INPUT);
		pin_mode(turnouts[i].pin_led, OUTPUT);
		pin_mode(turnouts[i].pin_servo, OUTPUT);
		pin_mode(turnouts[i].pin_button, INPUT_PULLUP);

		turnouts[i].index = i;
		turnouts[i].btn_debounce_val = 0;
		turnouts[i].btn_pressed = false;
	}

	sei(); // enable interrupts globally

	adc_read_all();
	while (adc_reading());

	TIMSK0 |= 1 << OCIE0A; // timer 0 enable compare match A
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
	for (uint8_t i = 0; i < TURNOUTS_COUNT; i++) {
		turnouts[i].position = (TurnoutPos)eeprom_read_byte((uint8_t*)(EEPROM_POS_START+i));

		if (turnouts[i].position == tpMovingToPlus)
			turnouts[i].position = tpPlus;
		else if (turnouts[i].position == tpMovingToMinus)
			turnouts[i].position = tpMinus;
		else if (turnouts[i].position > 3)
			turnouts[i].position = tpPlus;

		turnouts[i].angle = eeprom_read_word((uint16_t*)(EEPROM_ANGLE_START+i));
		if ((uint16_t)turnouts[i].angle == 0xFFFF)
			turnouts[i].angle = 0;
		else if (turnouts[i].angle < PWM_ANGLE_MIN)
			turnouts[i].angle = PWM_ANGLE_MIN;
		else if (turnouts[i].angle > PWM_ANGLE_MAX)
			turnouts[i].angle = PWM_ANGLE_MAX;
	}
}

void eeprom_store_pos(Turnout* turnout) {
	TurnoutPos pos = turnout->position;
	if (pos == tpMovingToPlus)
		pos = tpPlus;
	else if (pos == tpMovingToMinus)
		pos = tpMinus;

	eeprom_write_byte((uint8_t*)(EEPROM_POS_START + turnout->index), pos);
	eeprom_write_word((uint16_t*)(EEPROM_ANGLE_START + turnout->index), turnout->angle);
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

void switch_done(Turnout* turnout) {
	eeprom_store_pos(turnout);
}

///////////////////////////////////////////////////////////////////////////////

void leds_update_20ms(Turnout* turnout) {
	static volatile uint8_t blink = 0;
	#define BLINK_TIMEOUT 15

	if (turnout->position == tpPlus) {
		pin_mode(turnout->pin_led, OUTPUT);
		set_output(turnout->pin_led, true);
		blink = 0;
	} else if (turnout->position == tpMinus) {
		pin_mode(turnout->pin_led, OUTPUT);
		set_output(turnout->pin_led, false);
		blink = 0;
	} else {
		blink++;
		if (blink < BLINK_TIMEOUT) {
			pin_mode(turnout->pin_led, OUTPUT);
			set_output(turnout->pin_led, (turnout->position == tpMovingToPlus));
		} else if (blink < BLINK_TIMEOUT*2) {
			pin_mode(turnout->pin_led, INPUT);
		} else
			blink = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////

#include <avr/io.h>
#include <avr/interrupt.h>
#include "inputs.h"
#include "io.h"
#include "switch.h"
#include "pwm_servo_gen.h"
#include "turnouts.h"

void button_update_1ms(Turnout* turnout) {
	if (!get_input(turnout->pin_button)) {
		if (turnout->btn_debounce_val < DEBOUNCE_READS && !turnout->btn_pressed) {
			turnout->btn_debounce_val++;
			if (turnout->btn_debounce_val == DEBOUNCE_READS) {
				turnout->btn_pressed = true;
				btn_pressed(turnout);
			}
		}
	} else {
		if (turnout->btn_debounce_val > 0) {
			turnout->btn_debounce_val--;
			if (turnout->btn_debounce_val == 0)
				turnout->btn_pressed = false;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void _adc_start_conv(uint8_t pin);

volatile int8_t _adc_reading = -2;
#define ADC_READING_NO -2
#define ADC_READING_SPEED -1
#define ADC_READING_TURNOUT0 0

void adc_init() {
	// This function sets ADC properties. No change should be done to these
	// registers in any other function in future!
	// ADMUX: reference: AREF; user ADC0
	ADMUX |= (1 << REFS0); // AVCC with external capacitor at AREF pin
	ADCSRA |= 1 << ADIE; // enable ADC interrupt
	ADCSRA |= 0x5; // prescaler 32×
}

void adc_read_all() {
	_adc_reading = ADC_READING_SPEED;
	_adc_start_conv(PIN_SPEED);
}

void _adc_start_conv(uint8_t pin) {
	if (pin == IO_PINC0)
		ADMUX = (ADMUX & 0xF0) | 0x0;
	else if (pin == IO_PINC1)
		ADMUX = (ADMUX & 0xF0) | 0x1;
	else if (pin == IO_PINC2)
		ADMUX = (ADMUX & 0xF0) | 0x2;
	else if (pin == IO_PINC3)
		ADMUX = (ADMUX & 0xF0) | 0x3;
	else if (pin == IO_PINC4)
		ADMUX = (ADMUX & 0xF0) | 0x4;
	else if (pin == IO_PINC5)
		ADMUX = (ADMUX & 0xF0) | 0x5;
	else if (pin == IO_PINC6)
		ADMUX = (ADMUX & 0xF0) | 0x6;
	else if (pin == IO_PINC7)
		ADMUX = (ADMUX & 0xF0) | 0x7;
	else
		return;

	// Beware: no pins PC* could be switched during measurement!
	ADCSRA |= 1 << ADEN; // enable ADC!
	ADCSRA |= 1 << ADSC; // start conversion
}

ISR(ADC_vect) {
	uint16_t value = ADCL;
	value |= (ADCH << 8);

	extern Turnout turnouts[];

	if (_adc_reading == ADC_READING_NO)
		return;
	else if (_adc_reading == ADC_READING_SPEED) {
		switch_move_per_tick = value >> 3;
		if (switch_move_per_tick < 3)
			switch_move_per_tick = 3;
		_adc_reading++;
		_adc_start_conv(turnouts[0].pin_pot);
	} else if (_adc_reading >= ADC_READING_TURNOUT0 && _adc_reading < TURNOUTS_COUNT) {
		if (value > 1000)
			value = 1000;
		if (value < 10)
			value = 10;
		turnouts[_adc_reading].width = 2*value;
		_adc_reading++;
		if (_adc_reading >= TURNOUTS_COUNT) {
			_adc_reading = ADC_READING_NO;
			return;
		}
		_adc_start_conv(turnouts[_adc_reading].pin_pot);
	}
}

bool adc_reading() {
	return _adc_reading != ADC_READING_NO;
}

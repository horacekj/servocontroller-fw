#include <avr/interrupt.h>
#include "pwm_servo_gen.h"
#include "io.h"

#define OCR_SERVO_PERIOD 40000 // 20 ms
#define OCR_SERVO_MIDDLE 37000 // 20 ms - 1.5 ms = 18.5 m = servo middle

volatile int16_t _angle;
volatile int16_t _angle_buf;
volatile int8_t _pin = -1;
volatile int8_t _pin_buf = -1;
volatile bool _signal_high = false;

void pwm_servo_init() {
	// Setup 16-bit timer 1 for servo PWM generation
	// Servos are not connected to physical PWM output → we must generate PWM in firmware
	TCCR1B |= 1 << WGM12; /// CTC mode
	TCCR1B |= 1 << CS11; // prescaler 8×
	//TIMSK1 |= 1 << OCIE1A; // enable compare match A
	OCR1A = 40000; // 20 ms ~ 50 Hz (35000 = 2.5 ms = servo middle)
}

void pwm_servo_gen(uint8_t pin, int16_t angle) {
	_angle_buf = angle;
	_pin_buf = pin;

	if (_pin == -1) {
		// start PWM generation
		_pin = _pin_buf;
		_angle = _angle_buf;
		TCNT1H = 0;
		TCNT1L = 0;
		OCR1A = OCR_SERVO_PERIOD - OCR_SERVO_MIDDLE - _angle;
		set_output(_pin, false);
		_signal_high = false;
		TIMSK1 |= 1 << OCIE1A;
	}
}

void pwm_servo_stop() {
	_pin_buf = -1;
}

bool pwm_servo_generating() {
	return _pin > -1;
}

uint8_t pwm_servo_pin() {
	return _pin;
}

ISR(TIMER1_COMPA_vect) {
	// Timer 1 for servo control

	if (_signal_high) { // go low
		set_output(_pin, false);
		_pin = _pin_buf;
		_angle = _angle_buf;
		if (_pin < 0)
			TIMSK1 &= ~(1 << OCIE1A); // disable timer
		OCR1A = OCR_SERVO_MIDDLE + _angle;
	} else { // go high
		OCR1A = OCR_SERVO_PERIOD - OCR_SERVO_MIDDLE - _angle;
	}

	_signal_high = !_signal_high;
	set_output(_pin, _signal_high);
}

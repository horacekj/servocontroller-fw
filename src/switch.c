#include <stddef.h>
#include "switch.h"
#include "pwm_servo_gen.h"

Turnout* _switching = NULL;
volatile uint16_t switch_move_per_tick = 10;
volatile TurnoutPos _target_pos;
volatile uint8_t end_remain_counter;
volatile uint8_t begin_remain_counter;

#define BEGIN_REMAIN 6 // send 100 ms stable signal at end of switching
#define END_REMAIN 25 // send 500 ms stable signal at end of switching

void switch_turnout(Turnout* turnout, TurnoutPos pos) {
	_switching = turnout;
	_target_pos = pos;
	begin_remain_counter = 0;
	end_remain_counter = 0;

	if (pos == tpPlus)
		_switching->position = tpMovingToPlus;
	else
		_switching->position = tpMovingToMinus;

	pwm_servo_gen(_switching->pin_servo, _switching->angle);
}

void switch_stop() {
	if (_switching != NULL) {
		Turnout* tmp = _switching;
		_switching = NULL;
		pwm_servo_stop();
		switch_done(tmp);
	}
}

bool is_switching() {
	return _switching != NULL;
}

Turnout* switching_turnout() {
	return _switching;
}

void switch_update() {
	if (_switching == NULL)
		return;
	if (begin_remain_counter < BEGIN_REMAIN) {
		begin_remain_counter++;
		return;
	}

	if (_target_pos == tpPlus) {
		if (_switching->angle <= -(int16_t)_switching->width) {
			end_remain_counter++;
			if (end_remain_counter >= END_REMAIN) {
				_switching->position = tpPlus;
				switch_stop();
				return;
			}
		} else {
			_switching->angle -= switch_move_per_tick;
		}
	} else { // assert _target_pos == tpMinus
		if (_switching->angle >= (int16_t)_switching->width) {
			end_remain_counter++;
			if (end_remain_counter >= END_REMAIN) {
				_switching->position = tpMinus;
				switch_stop();
				return;
			}
		} else {
			_switching->angle += switch_move_per_tick;
		}
	}

	pwm_servo_gen(_switching->pin_servo, _switching->angle);
}

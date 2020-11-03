#ifndef _PWM_SERVO_GEN_
#define _RWM_SERVO_GEN_

#include <stdbool.h>
#include <stdint.h>

// Angle range: -2000 – 2000

void pwm_servo_init();
void pwm_servo_gen(uint8_t pin, int16_t angle);
void pwm_servo_stop();
bool pwm_servo_generating();
uint8_t pwm_servo_pin();

#endif

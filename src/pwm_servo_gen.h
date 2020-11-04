#ifndef _PWM_SERVO_GEN_
#define _RWM_SERVO_GEN_

/* Low-level software PWM generation */

#include <stdbool.h>
#include <stdint.h>

// Angle range: -2000 – 2000

#define PWM_ANGLE_MIN -2000
#define PWM_ANGLE_MAX 2000

void pwm_servo_init();
void pwm_servo_gen(uint8_t pin, int16_t angle);
void pwm_servo_stop();
bool pwm_servo_generating();
int8_t pwm_servo_pin();

#endif

#ifndef _INPUTS_H_
#define _INPUTS_H_

/* Inputs processing:
   * pin debouncing
   * potentiometers reading
 */

#include "turnout.h"
#include "io.h"

#define PIN_SPEED IO_PINC4
#define DEBOUNCE_READS 20 // also time in ms

void btn_pressed(Turnout* turnout);
void button_update_1ms(Turnout* turnout);

void adc_init();
void adc_read_all();
bool adc_reading();

#endif

#ifndef _INPUTS_H_
#define _INPUTS_H_

#include "turnout.h"

#define DEBOUNCE_READS 20 // also time in ms

void btn_pressed(Turnout* turnout);
void button_update_1ms(Turnout* turnout);

#endif

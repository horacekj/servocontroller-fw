#ifndef _SWITCH_H_
#define _SWITCH_H_

#include <stdbool.h>
#include "turnout.h"

void switch_turnout(Turnout*, TurnoutPos);
void switch_stop();
bool is_switching();
Turnout* switching_turnout();
void switch_update();
void switch_done(Turnout*);

extern volatile uint16_t switch_move_per_tick;

#endif

#include "inputs.h"
#include "io.h"

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

/* Main source file of Servocontroller ATmega CPU.
 */

#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

///////////////////////////////////////////////////////////////////////////////

int main();
static inline void init();

///////////////////////////////////////////////////////////////////////////////

int main() {
	init();

	while (true) {
		wdt_reset();
	}
}

static inline void init() {
	ACSR |= ACD;  // analog comparator disable
	WDTCR |= 1 << WDE;  // watchdog enable
	WDTCR |= WDP2; // ~250 ms timeout

	// Setup timer 0
	TCCR0A |= 1 << WGM01; // CTC mode
	TCCR0B |= 1 << CS01; // no prescaler
	TIMSK |= 1 << OCIE0A; // enable compare match A
	OCR0A = 99;

	sei(); // enable interrupts globally
}

ISR(TIM0_COMPA_vect) {
	// Timer 0 @ 10 kHz (period 100 us)
}

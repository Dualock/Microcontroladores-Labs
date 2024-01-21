#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

// ***************** local headers ***********************
#include "display7seg.h"

// ***************** Macros ******************************
#define COMPARE_TIME 195

/// *********************** Function Prototypes *************************
void setup(void);
void initTimerInterrupt(void);

// ************************ Global Variables ****************************
volatile int ticks_per_second;
volatile int time_left;

/// ********************** Timer 40hz interrupt *************************
ISR(TIMER0_COMPA_vect){
	if(ticks_per_second == 0){
		PORTB ^= (1<<PORTB6);
		ticks_per_second = 40;
		time_left--;
	}
	else{
		ticks_per_second--;
	}

}

/// *********************** Setup function ******************************
void setup(){
	//---- D3, D2, D1, D0 as outputs ---------
	DDRD = (1 << DDD3) | (1 << DDD2) | (1 << DDD1) | (1 << DDD0) | (1 << DDD3);
	
	//---- B7, B6 s outputs ------------------
	DDRB = (1 << DDB7) | (1 << DDB6) | (1 << DDB5);
	initTimerInterrupt();
}

/// *********************** main function ******************************
int main(void)
{
	unsigned char tens;
	unsigned char units;
	ticks_per_second = 40;
	time_left = 30;
	setup();
	
  while (1) {
		tens = time_left/10;
		units = time_left%10;
		multiplexar(tens,units);
    
  }
}

/// ************************ Timer interrupt ****************************
void initTimerInterrupt(){
	// -------- Timer0 setup -----------------
	TCCR0A = (1 << WGM01); // CTC mode - Clear timer on compare
	OCR0A = COMPARE_TIME; // Generate 40Hz interruption
	TIMSK = (1 << OCIE0A); // Enable compare match A
	TCCR0B = (1 << CS02) | (1 << CS00); // Set prescaler clk/1024
	sei();
}

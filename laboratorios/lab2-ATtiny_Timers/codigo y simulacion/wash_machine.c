#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

// ***************** local headers ***********************
#include "display7seg.h"

// ***************** Macros ******************************
#define COMPARE_TIME 195
#define LOAD_SIZE_BIT1 PB3
#define LOAD_SIZE_BIT2 PB4
#define SWITCH_7SEG PB6

// ************************ Global Variables ****************************
volatile int mode;
volatile int play;
volatile int ticks_per_second;
volatile int time_left;
volatile unsigned char pause_flag;

/// *********************** Function Prototypes *************************
void setup(void);
void initTimerInterrupt(void);
void initExternalInterrupts(void);
void loadSize(int);
void pauseHandler(void);

/// ********************** Timer 40hz interrupt *************************
ISR(TIMER0_COMPA_vect){
	if(ticks_per_second == 0){
		PORTB ^= (1<<PORTB5);
		ticks_per_second = 40;
		time_left--;
	}
	else{
		ticks_per_second--;
	}

}

// Interrupt service routine for load_size_mode choice
ISR(INT0_vect)        // Interrupt service routine 
{
  pause_flag++;
  if(pause_flag > 1){
  	pause_flag = 0;
  }
}

// Interrupt service routine for play/pause button
ISR(INT1_vect)        // Interrupt service routine 
{
  mode++; // increase the mode
  if(mode > 3){
  	mode = 0;
  }
}

/// *********************** main function ******************************
int main(void)
{
	unsigned char tens;
	unsigned char units;
	ticks_per_second = 40;
	time_left = 30;
	pause_flag = 1;
	setup();
	
  while (1) {
		pauseHandler();
		tens = time_left/10;
		units = time_left%10;
		multiplexar(tens,units);
  }
}

/// *********************** Setup function ******************************
void setup(){
	//---- A1, A0, D1, D0 as outputs ---------
	DDRD = (1 << DDD1) | (1 << DDD0);
	DDRA = (1 << DDA1) | (1 << DDA0);
	
	//---- B7, B6, B5, B4 and B3 as outputs ------------------
	DDRB = (1 << DDB7) | (1 << DDB6) | (1 << DDB5) | (1 << DDB4) | (1 << DDB3);
	
	// ----------------- Enable interrupts ----------------------
	initTimerInterrupt();
	initExternalInterrupts();
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

/// *********************** External interrupts *************************
void initExternalInterrupts(){
	//------------------ Enable external interrupt INT1 at pin D3 ------------------
  GIMSK |= (1<<INT1);	// enabling the INT0 (external interrupt)  
  MCUCR |= (1<<ISC11); // Configuring interrupt triggered by falling edge at PB1
  
  //------------------ Enable external interrupt INT0 at pin D4 ------------------
  GIMSK |= (1<<INT0);	// enabling the INT0 (external interrupt)  
  MCUCR |= (1<<ISC01); // Configuring interrupt triggered by falling edge at PB0 
	sei();//enabling global interrupt
}

/// ********************* Size load options *****************************
void loadSize(int nivel){
	// clean bits
	PORTB &= ~((1 << LOAD_SIZE_BIT1) | (1 << LOAD_SIZE_BIT2));
	switch(nivel){
		// low size load
		case 1:
			PORTB |= (1 << LOAD_SIZE_BIT1);
			break;
		// medium size load
		case 2:
			PORTB |= (1 << LOAD_SIZE_BIT2);
			break;
		// high size load
		case 3:
			PORTB |= (1 << LOAD_SIZE_BIT1) | (1 << LOAD_SIZE_BIT2);
			break;
		default:
			break;
	}
}

void pauseHandler(){
	if(pause_flag == 1){
		//pause, disable compare match A
		TIMSK &= ~(1 << OCIE0A);
		// load size selection only available on pause
		loadSize(mode);
	}
	//play, enable compare match A
	else if(pause_flag == 0) {
		TIMSK = (1 << OCIE0A);
		PORTB |= (1<<SWITCH_7SEG); //turn on the 7 segments
	}
}



#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

// ***************** local headers ***********************
#include "display7seg.h"
#include "modes_and_times.h"
#include "state_machine.h"

// ***************** Macros ******************************
#define COMPARE_TIME 195
#define SWITCH_7SEG PB6
#define TIMES 4
#define MOTOR_ENABLE PD4
#define WARNING PD6

// ************************ Global Variables ****************************
volatile unsigned char mode;
volatile unsigned char ticks_per_second;
volatile unsigned char time_left;
volatile unsigned char pause_flag;
volatile unsigned char time_array [TIMES];

/// *********************** Function Prototypes *************************
void setup(void);
void initTimerInterrupt(void);
void initExternalInterrupts(void);
void pauseHandler(stateMachine_struct * stateMachine);

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

// Interrupt service routine for play/pause button
ISR(INT0_vect)        // Interrupt service routine 
{
	// Clear warning bit
	PORTD &= ~(1 << WARNING);
	// if not paused, pause
	if(pause_flag ==  0){
		pause_flag = 1;
	}
	// If not paused and mode is 0, then pause;
	else if(pause_flag == 0 && mode == 0){
		pause_flag = 1;
		
	}
	// if paused, play only if mode is gt 0
	else if(pause_flag == 1 && mode > 0){
		pause_flag = 0;
	}
	// If paused, and try to play with mode 0, send a warning
	else if(pause_flag == 1 && mode == 0){
		pause_flag = 1;
		PORTD |= (1 << WARNING);
	}
}

// Interrupt service routine for load_size_mode choice
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
	unsigned int iterator = 0;
	 // Create new state machine object
  stateMachine_struct stateMachine;
  // Establish the ticks for the timer interrupt
	ticks_per_second = 40;
	pause_flag = 1;
	mode = 0;
	setup();
	StateMachine_Init(&stateMachine);
	
  while (1) {
  	// Estado inactivo
		if(strcmp(StateMachine_GetState(stateMachine.currState), "ST_IDLE") == 0){
			//load size selection only available on idle mode
			loadSize(mode, time_array);
			//after selecting mode, set the timer of water as a time left
			time_left = time_array[0];
			// pause handler to activate TIMSK
			pauseHandler(&stateMachine);
			//
			iterator = 0;
		}
		
		else if(time_left != 0){
			PORTB |= (1<<SWITCH_7SEG); //turn on the 7 segments
			pauseHandler(&stateMachine);
			//StateMachine_Iterate(&stateMachine, EV_NONE);
			tens = time_left/10;
			units = time_left%10;
			multiplexar(tens,units);
		}
		// if time runs out, go to next state
		else if(time_left == 0 && iterator < 3){
			iterator++;
			multiplexar(0,0);
			StateMachine_Iterate(&stateMachine, EV_TIME_OUT);
			_delay_ms(20000);
			PORTB &= ~(1<<SWITCH_7SEG);
			time_left = time_array[iterator];
		}
  }
}

/// *********************** Setup function ******************************
void setup(){
	//---------------- BCD bits: A1, A0, D1, D0 as outputs ------------------
	DDRD = (1 << DDD1) | (1 << DDD0);
	DDRA = (1 << DDA1) | (1 << DDA0);
	
	// --- B7, B6, B5, B4, B3 B2, B1, B0 as outputs -------------------------
	//display: B7, B6 - mode: B4, B3 - state indicator: B2, B1, B0
	DDRB = (1 << DDB7) | (1 << DDB6) | (1 << DDB5) | (1 << DDB4) | (1 << DDB3);
	
	// ---------Motor controller: D4 and Warning: D6 as outputs -------------
	DDRD = (1 << DDD4) | (1 << DDD6);
	
	// ----------------- Enable interrupts ----------------------------------
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

/// ************* Pause handler ******************
void pauseHandler(stateMachine_struct * stateMachine){
	if(pause_flag == 1){
		//pause, disable compare match A
		TIMSK &= ~(1 << OCIE0A);
		// Stops motor
		PORTD &= ~(1 << MOTOR_ENABLE); // clear PD4

	}
	else if(pause_flag == 0){
		TIMSK = (1 << OCIE0A);
		// Play event in the state machine
		StateMachine_Iterate(stateMachine, EV_PLAY);
		// Runs motor
		//PORTD |= (1 << MOTOR_ENABLE);
	}
}



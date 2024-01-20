#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
// Macros


volatile int mode;
volatile int play;
// Interrupt service routine for load_size_mode choice
ISR(INT0_vect)        // Interrupt service routine 
{
  PORTB ^= (1<<PB3);    // Toggling the PB5 pin
  mode++;
}

// Interrupt service routine for play/pause button
ISR(INT1_vect)        // Interrupt service routine 
{
  PORTB ^= (1<<PB4);    // Toggling the PB5 pin
  play++;
}


void initExternalInterrupts(){

	//------------------ Enable external interrupt INT1 at pin D3 ------------------
  GIMSK |= (1<<INT1);	// enabling the INT0 (external interrupt)  
  MCUCR |= (1<<ISC11); // Configuring interrupt triggered by falling edge at PB1
  
  //------------------ Enable external interrupt INT0 at pin D4 ------------------
  GIMSK |= (1<<INT0);	// enabling the INT0 (external interrupt)  
  MCUCR |= (1<<ISC01); // Configuring interrupt triggered by falling edge at PB0 
	sei();//enabling global interrupt
}

void setup()
{
	// set PB6, PB5, PB4 and PB3 as output(LED)
  DDRB |= (1<<PB6)|(1<<PB5)|(1<<PB4)|(1<<PB3);
  initExternalInterrupts();
}



int main()
{
	mode = 0;
  setup();
  while(1)
  {
  	if(mode > 3){
  		PORTB |= (1<<PB6);
  		_delay_ms(1000);
  	}
  	if(play > 3){
  		PORTB |= (1<<PB5);
  		_delay_ms(1000);
  	}
  }
}

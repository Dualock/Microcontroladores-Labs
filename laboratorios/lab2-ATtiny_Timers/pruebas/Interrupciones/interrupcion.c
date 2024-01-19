#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
// Macros

volatile int counter;
ISR(INT1_vect)        // Interrupt service routine 
{
  PORTB ^= (1<<PB5);    // Toggling the PB5 pin
  counter++;
  _delay_ms(200);
}

ISR(PCINT1_vect)        // Interrupt service routine 
{
  PORTB ^= (1<<PB5);    // Toggling the PB5 pin
  counter++;
  _delay_ms(200);
}

void initExternalInterrupt(){

	//------------------ Enable external interrupt at pin B1 ------------------
  GIMSK |= (1<<INT1);	// enabling the INT0 (external interrupt)  
  PCMSK |= (1 << PCINT1); // configure interrupt at PB1
  MCUCR |= (1<<ISC11); // Configuring interrupt triggered by falling edge at PB1 
	sei();//enabling global interrupt
}

void pinChangeInterrupt()
{
	sei();
  GIMSK|= (1<<PCIE0);
  PCMSK|=(1<<PCINT1);
  
}

void setup()
{
	// set PB6 and PB5 as output(LED)
  DDRB |= (1<<PB6)|(1<<PB5);
  // set PB1 as an input
  DDRB &= ~ (1 << DDB1);
  //initExternalInterrupt();
  pinChangeInterrupt();
  
}



int main()
{
	counter = 0;
  setup();
  while(1)
  {
  	if(counter > 0){
  		PORTB = (1<<PB6);
  		_delay_ms(1000);
  	}
  }
}

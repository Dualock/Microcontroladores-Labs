#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
// Macros


// External interrupt service routine
ISR(INT1_vect){
	// if INT1 button is pressed 
	_delay_ms(50);
	// PB5 ON
	PORTB |= (1 << PB5);
}

void setup(){
	//---- B7, B6, B5, B4 as outputs ---------
	DDRB = (1 << DDB7)|(1 << DDB6)|(1 << DDB5)|(1 << DDB4) | (1 << DDB3);
	
	//-------- External interrupt setup PCINT1 (PB1) ------
	PCMSK = (1 << PCINT1);
	
	//-------- External interrupt setup INT1 (PB1) ------
	MCUCR |= (1 << ISC11); // Sets INT1 as falling edge
	GIMSK |= (1 << INT1);  // Habilitar INT1
	
	//Global interrupt enable
	sei();
}
int main(void)
{
	setup();
	
  while (1) {
    PORTB &= ~((1 << PB3) | (1 << PB4)); // Apaga el bit 4 del puerto B
    _delay_ms(1000); 
    PORTB |=  ((1 << PB3) | (1 << PB4));
    _delay_ms(1000); 
  }
}

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#define LOAD_SIZE_LED1 PB3
#define LOAD_SIZE_LED2 PB4

void setup(){
	//---- B4, B3 as outputs ---------
	DDRB = (1 << DDB4) | (1 << DDB3);
}

void nivelDeCarga(int nivel){
	// clean bits
	PORTB &= ~((1 << LOAD_SIZE_LED1) | (1 << LOAD_SIZE_LED2));
	switch(nivel){
		// low size load
		case 1:
			PORTB |= (1 << LOAD_SIZE_LED1);
			break;
		// medium size load
		case 2:
			PORTB |= (1 << LOAD_SIZE_LED2);
			break;
		// high size load
		case 3:
			PORTB |= (1 << LOAD_SIZE_LED1) | (1 << LOAD_SIZE_LED2);
			break;
		default:
			break;
	}
}
int main(void)
{
	setup();
	
  while (1) {
  	nivelDeCarga(1);
  	_delay_ms(20000);
  	nivelDeCarga(2);
  	_delay_ms(20000);
  	nivelDeCarga(3);
  	_delay_ms(20000);
    
  }
}

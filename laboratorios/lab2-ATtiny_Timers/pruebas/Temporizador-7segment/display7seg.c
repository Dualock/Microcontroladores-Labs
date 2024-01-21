#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

// Macros
#define BCD3 PD0
#define BCD2 PD1
#define BCD1 PA1
#define BCD0 PA0
#define MULTIPLEXER PB7
#define APAGADOR B5
#define F_60HZtoDelay 133
#define COMPARE_TIME 195
#define SECOND_DELAY 8000

/// *********************** BCD map function ******************************
void map_bcd(int num){
	// Clean the BCD bits
	PORTD &= ~((1 << BCD3) | (1 << BCD2));
	PORTA &= ~((1 << BCD1) | (1 << BCD0));
	switch (num) {
		case 0:
			break;
		case 1:
			PORTA |= (1 << BCD0);
		  break;
    case 2:
		  PORTA |= (1 << BCD1);
		  break;
		case 3:
			PORTA |= ((1 << BCD1) | (1 << BCD0));
			break;
		case 4:
			PORTD |= (1 << BCD2);
			break;
		case 5:
			PORTD |= (1 << BCD2);
			PORTA |= (1 << BCD0);
		  break;
    case 6:
		  PORTD |= ((1 << BCD2));
		  PORTA |= (1 << BCD1);
		  break;
		case 7:
			PORTD |= (1 << BCD2);
			PORTA |= ((1 << BCD1) | (1 << BCD0));
			break;
		case 8:
			PORTD |= (1 << BCD3);
			break;
		case 9:
			PORTD |= (1 << BCD3);
			PORTA |= (1 << BCD0);
			break;
		default:
			// Hacer algo por default
      break;
    }
}


/// *********************** multiplexing function ******************************
void multiplexar(int decenas, int unidades){
	PORTB ^= (1 << MULTIPLEXER);//toggle the bit
	map_bcd(decenas);
	_delay_ms(F_60HZtoDelay);
	PORTB ^= (1 << MULTIPLEXER); //toggle the bit
	map_bcd(unidades);
	_delay_ms(F_60HZtoDelay); 
}

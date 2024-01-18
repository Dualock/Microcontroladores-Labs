#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

// Macros
#define BCD3 PD0
#define BCD2 PD1
#define BCD1 PD2
#define BCD0 PD3
#define MULTIPLEXER PB7
#define APAGADOR B5
#define F_60HZtoDelay 133
#define COMPARE_TIME 195
//#define F_CPU 1600000
#define SECOND_DELAY 8000

// ************************ Global Variables ****************************
volatile int ticks_per_second;
volatile int time_left;
/// *********************** Function prototypes *************************
void setup(void);
void map_bcd(int);
void multiplexar(int , int );


/// ********************** Interruption *********************************
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


/// *********************** Setup function ******************************
void setup(){
	//---- D3, D2, D1, D0 as outputs ---------
	DDRD = (1 << DDD3) | (1 << DDD2) | (1 << DDD1) | (1 << DDD0) | (1 << DDD3);
	
	//---- B7, B6 s outputs ------------------
	DDRD = (1 << DDB7) | (1 << DDB6) | (1 << DDB5);
	
	// -------- Timer0 setup -----------------
	TCCR0A = (1 << WGM01); // CTC mode - Clear timer on compare
	OCR0A = COMPARE_TIME; // Generate 40Hz interruption
	TIMSK = (1 << OCIE0A); // Enable compare match A
	TCCR0B = (1 << CS02) | (1 << CS00); // Set prescaler clk/1024
	sei();
}


/// *********************** BCD map function ******************************
void map_bcd(int num){
	// Clean the BCD bits
	PORTD &= ~((1 << BCD3) | (1 << BCD2) | (1 << BCD1) | (1 << BCD0));
	switch (num) {
		case 0:
			break;
		case 1:
			PORTD |= (1 << BCD0);
		  break;
    case 2:
		  PORTD |= (1 << BCD1);
		  break;
		case 3:
			PORTD |= ((1 << BCD1) | (1 << BCD0));
			break;
		case 4:
			PORTD |= (1 << BCD2);
			break;
		case 5:
			PORTD |= ((1 << BCD2) | (1 << BCD0));
		  break;
    case 6:
		  PORTD |= ((1 << BCD2) | (1 << BCD1));
		  break;
		case 7:
			PORTD |= ((1 << BCD2) | (1 << BCD1) | (1 << BCD0));
			break;
		case 8:
			PORTD |= (1 << BCD3);
			break;
		case 9:
			PORTD |= ((1 << BCD3) | (1 << BCD0));
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

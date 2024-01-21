#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "modes_and_times.h"
void loadSize(unsigned char mode, unsigned char * time_array){
	// clean bits
	PORTB &= ~((1 << LOAD_SIZE_BIT1) | (1 << LOAD_SIZE_BIT2));
	switch(mode){
		// low size load
		case 1:
			PORTB |= (1 << LOAD_SIZE_BIT1);
			time_array[0] = LOW_WATER;
			time_array[1] = LOW_WASH;
			time_array[2] = LOW_RINSE;
			time_array[3] = LOW_SPIN;
			break;
		// medium size load
		case 2:
			PORTB |= (1 << LOAD_SIZE_BIT2);
			time_array[0] = MED_WATER;
			time_array[1] = MED_WASH;
			time_array[2] = MED_RINSE;
			time_array[3] = MED_SPIN;
			//time_array [4] = {MED_WATER, MED_WASH, MED_RINSE, MED_SPIN};
			break;
		// high size load
		case 3:
			PORTB |= (1 << LOAD_SIZE_BIT1) | (1 << LOAD_SIZE_BIT2);
			time_array[0] = HIGH_WATER;
			time_array[1] = HIGH_WASH;
			time_array[2] = HIGH_RINSE;
			time_array[3] = HIGH_SPIN;
			//time_array [4] = {HIGH_WATER, HIGH_WASH, HIGH_RINSE, HIGH_SPIN};
			break;
		default:
			break;
	}
}

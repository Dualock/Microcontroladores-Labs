#include <stdio.h>
#include "state_machine.h"
#include <avr/interrupt.h>
#include <util/delay.h>


#define MOTOR_ENABLE PD4
#define STATE_INDICATOR_BIT0 PB2
#define STATE_INDICATOR_BIT1 PB1
#define STATE_INDICATOR_BIT2 PB0
typedef struct {
    const char * name;
    void (*func)(void);
} stateFunction_struct;

// State transition Matrix
typedef struct {
    states_enum currState;
    event_enum event;
    states_enum nextState;
} stateTransitionArray_struct;

// Functions
void Water() {
  // clear indicator bits 2 and 1
  PORTB &= ~((1 << STATE_INDICATOR_BIT2) | (1 << STATE_INDICATOR_BIT1));
  //PORTD |= (1 << MOTOR_ENABLE);
  // 001
  PORTB |= (1 << STATE_INDICATOR_BIT0);
}

void Wash() {
  // clear indicator bits 0 and 2
  PORTB &= ~((1 << STATE_INDICATOR_BIT2) | (1 << STATE_INDICATOR_BIT0));
  // Motor ON
  PORTD |= (1 << MOTOR_ENABLE);
  // 010
  PORTB |= (1 << STATE_INDICATOR_BIT1);
}

void Rinse() {
	//clear indicator bit 2
  PORTB &= ~(1 << STATE_INDICATOR_BIT2);
  // MOTOR ON
  PORTD |= (1 << MOTOR_ENABLE);
  // 011
  PORTB |= ((1 << STATE_INDICATOR_BIT1) | ((1 << STATE_INDICATOR_BIT0)));
}

void Spin() {
	// clear indicator bit 0 and 1
	PORTB &= ~((1 << STATE_INDICATOR_BIT0) | (1 << STATE_INDICATOR_BIT1));
	// Motor ON
  PORTD |= (1 << MOTOR_ENABLE);
  // 100
  PORTB |= (1 << STATE_INDICATOR_BIT2);
}
void Idle() {
	//clear all indicator bits 000
	PORTB &= ~((1 << STATE_INDICATOR_BIT0) | (1 << STATE_INDICATOR_BIT1) | (1 << STATE_INDICATOR_BIT2));
	//Motor OFF
 // PORTD &= ~(1 << MOTOR_ENABLE); // clear PD4
  
}

// 
static stateFunction_struct stateFunction[] = {
	// NAME         		// FUNC
	{ "ST_IDLE",      	&Idle 	},// ST_IDLE
	{ "ST_WATER",    		&Water 	},// ST_WATER
	{ "ST_WASH",   			&Wash 	},// ST_WASH
	{ "ST_RINSE",   		&Rinse 	},// ST_RINSE
	{ "ST_SPIN",   			&Spin 	},//ST_SPIN
};

static stateTransitionArray_struct stateTransMatrix[] = {
    // CURR STATE		// EVENT            // NEXT STATE
    { ST_IDLE,     		EV_NONE,     			ST_IDLE				},
    { ST_IDLE,     		EV_PLAY,     			ST_WATER			},
    { ST_WATER,     	EV_NONE,     			ST_WATER			},
    { ST_WATER,     	EV_TIME_OUT,     	ST_WASH				},
    { ST_WATER,     	EV_PLAY,     			ST_WATER			},
    { ST_WASH,     		EV_NONE,     			ST_WASH				},
    { ST_WASH,     		EV_PLAY,     			ST_WASH				},
    { ST_WASH,   			EV_TIME_OUT,      ST_RINSE			},
    { ST_RINSE,     	EV_NONE,     			ST_RINSE			},
    { ST_RINSE,     	EV_PLAY,     			ST_RINSE			},
    { ST_RINSE,   		EV_TIME_OUT,			ST_SPIN				},
    { ST_SPIN,     		EV_NONE,     			ST_SPIN				},
    { ST_SPIN,     		EV_PLAY,     			ST_SPIN				},
    { ST_SPIN,  			EV_TIME_OUT,      ST_IDLE				},
};

// Initialize the state machine
void StateMachine_Init(stateMachine_struct * stateMachine) {
    printf("Initialising state machine.\r\n");
    stateMachine->currState = ST_IDLE;
}
// Iterate through the state machine
void StateMachine_Iterate(stateMachine_struct *stateMachine, event_enum event) {

	int matrix_length = sizeof(stateTransMatrix)/sizeof(stateTransMatrix[0]);
	// Iterate through the state transition matrix, checking if there is both a match with the current state
	for(int i = 0; i < matrix_length ; i++) {
		if(stateTransMatrix[i].currState == stateMachine->currState) {
			if((stateTransMatrix[i].event == event)) {

				// Transition to the next state
				stateMachine->currState = stateTransMatrix[i].nextState;

				// Call the function associated with transition
				(stateFunction[stateMachine->currState].func)();
				break;
			}
		}
	}
}

// get the state name
const char * StateMachine_GetState(states_enum state) {
    return stateFunction[state].name;
}


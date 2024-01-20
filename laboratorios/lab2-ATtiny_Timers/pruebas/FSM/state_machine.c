#include <stdio.h>
#include "state_machine.h"

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
    printf("Watering.\r\n");
}

void Wash() {
    printf("Washing.\r\n");
}

void Rinse() {
    printf("Rinsing.\r\n");
}

void Spin() {
    printf("Spinning.\r\n");
}
void Idle() {
    printf("Idle.\r\n");
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
    { ST_IDLE,     		EV_PLAY,     			ST_WATER			},
    { ST_WATER,     	EV_TIME_OUT,     	ST_WASH				},
    { ST_WASH,   			EV_TIME_OUT,      ST_RINSE			},
    { ST_RINSE,   		EV_TIME_OUT,			ST_SPIN				},
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


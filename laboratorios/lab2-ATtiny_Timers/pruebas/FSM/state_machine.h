
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

// States
typedef enum{
	ST_IDLE,
	ST_WATER,
	ST_WASH,
	ST_RINSE,
	ST_SPIN
} states_enum;

// Inputs/Events
typedef enum {
	EV_PLAY,
	EV_TIME_OUT
} event_enum;

// State machine
typedef struct {
    states_enum currState;
} stateMachine_struct;


// Function prototypes
void Water();

void Wash();

void Rinse();

void Spin();

void Idle();

void StateMachine_Init(stateMachine_struct * stateMachine);

void StateMachine_Iterate(stateMachine_struct *stateMachine, event_enum event);

const char * StateMachine_GetState(states_enum state);

#endif

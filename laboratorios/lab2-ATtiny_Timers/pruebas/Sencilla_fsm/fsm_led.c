#include <stdio.h>

// States
typedef enum{
	ST_IDLE,
	ST_LED_ON,
	ST_LED_BLINK,
	ST_LED_OFF
} states_enum;

// Inputs/Events
typedef enum {
	EV_NONE,
	EV_PLAY,
	EV_TIME_OUT
} event_enum;

// State machine
typedef struct {
    states_enum currState;
} stateMachine_struct;

// Struct with function pointer and name of the state
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
void Led_Init() {
    printf("Led_Init() called.\r\n");
}

void Led_On() {
    printf("LED turned on.\r\n");
}

void Led_Off() {
    printf("LED turned off.\r\n");
}

void Led_Idle() {
    printf("LED in idle state.\r\n");
}
void Led_Blink() {
    printf("LED blinking.\r\n");
}

// 
static stateFunction_struct stateFunction[] = {
      // NAME         	// FUNC
    { "ST_IDLE",      	&Led_Idle },      // ST_IDLE
    { "ST_LED_ON",    	&Led_On },        // ST_LED_ON
	{ "ST_LED_BLINK",   &Led_Blink },     // ST_LED_BLINK
    { "ST_LED_OFF",   	&Led_Off },       // ST_LED_OFF
};

static stateTransitionArray_struct stateTransMatrix[] = {
    // CURR STATE		// EVENT            // NEXT STATE
    { ST_IDLE,     		EV_NONE,     		ST_IDLE			},
    { ST_IDLE,     		EV_PLAY,     		ST_LED_ON		},
    { ST_LED_ON,   		EV_TIME_OUT,        ST_LED_OFF		},
    { ST_LED_ON,   		EV_PLAY,			ST_LED_BLINK	},
    { ST_LED_BLINK,  	EV_TIME_OUT,        ST_LED_OFF		},
    { ST_LED_BLINK,   	EV_PLAY,			ST_LED_OFF		},
    { ST_LED_OFF,  		EV_NONE,   			ST_IDLE			}
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

int main(){
 	printf("main() called.\r\n");

    // Create new state machine object
    stateMachine_struct stateMachine;

    StateMachine_Init(&stateMachine);
    printf("State is now %s.\r\n", StateMachine_GetState(stateMachine.currState));

    // Push button to start flasher
    printf("Button pushed.\r\n");
    StateMachine_Iterate(&stateMachine, EV_PLAY);
    printf("State is now %s.\r\n", StateMachine_GetState(stateMachine.currState));
    
        // Push button to start flasher
    printf("Button pushed.\r\n");
    StateMachine_Iterate(&stateMachine, EV_PLAY);
    printf("State is now %s.\r\n", StateMachine_GetState(stateMachine.currState));
    
    // Timeout
    printf("Timeout.\r\n");
    StateMachine_Iterate(&stateMachine, EV_TIME_OUT);
    printf("State is now %s.\r\n", StateMachine_GetState(stateMachine.currState));
    
    printf("None.\r\n");
    StateMachine_Iterate(&stateMachine, EV_NONE);
    printf("State is now %s.\r\n", StateMachine_GetState(stateMachine.currState));
    
    printf("Play.\r\n");
    StateMachine_Iterate(&stateMachine, EV_PLAY);
    printf("State is now %s.\r\n", StateMachine_GetState(stateMachine.currState));
    
    printf("None.\r\n");
    StateMachine_Iterate(&stateMachine, EV_NONE);
    printf("State is now %s.\r\n", StateMachine_GetState(stateMachine.currState));
    
    printf("Time out.\r\n");
    StateMachine_Iterate(&stateMachine, EV_TIME_OUT);
    printf("State is now %s.\r\n", StateMachine_GetState(stateMachine.currState));
    
    return 0;

}

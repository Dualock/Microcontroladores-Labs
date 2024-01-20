#include <stdio.h>
#include "state_machine.h"

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
    
    // Timeout
    printf("Timeout.\r\n");
    StateMachine_Iterate(&stateMachine, EV_TIME_OUT);
    printf("State is now %s.\r\n", StateMachine_GetState(stateMachine.currState));
    
    // Timeout
    printf("Timeout.\r\n");
    StateMachine_Iterate(&stateMachine, EV_TIME_OUT);
    printf("State is now %s.\r\n", StateMachine_GetState(stateMachine.currState));
    
    // Timeout
    printf("Timeout.\r\n");
    StateMachine_Iterate(&stateMachine, EV_TIME_OUT);
    printf("State is now %s.\r\n", StateMachine_GetState(stateMachine.currState));
    
    return 0;

}

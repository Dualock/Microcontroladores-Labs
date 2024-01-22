#include <stdio.h>
#include "state_machine.h"
#include <string.h>

int time_left;

void play(stateMachine_struct * stateMachine){
	printf("PLAY.\r\n");
	StateMachine_Iterate(stateMachine, EV_PLAY);
}

int main(){
	int i = 0;
	printf("main() called.\r\n");
	// Create new state machine object
	stateMachine_struct stateMachine;
	StateMachine_Init(&stateMachine);
	play(&stateMachine);
	printf("State is now %s.\r\n", StateMachine_GetState(stateMachine.currState));
	printf("PLAY.\r\n");
	StateMachine_Iterate(&stateMachine, EV_PLAY);
	while(i<25){
		if(strcmp(StateMachine_GetState(stateMachine.currState), "ST_RINSE") == 0){
			printf("OBTENIENDO NOMBRE DE ESTADO: RINSE. \n");
		}
		if(time_left == 0){
			printf("Timeout.\r\n");
			StateMachine_Iterate(&stateMachine, EV_TIME_OUT);
			printf("State is now %s.\r\n", StateMachine_GetState(stateMachine.currState));
			printf("time left %i\n", time_left);
			time_left = 5;
		
		}
		else{
			printf("NONE.\r\n");
			StateMachine_Iterate(&stateMachine, EV_NONE);
			printf("State is now %s.\r\n", StateMachine_GetState(stateMachine.currState));
			time_left--;
			printf("time left %i\n", time_left);
		}
		i++;
	}

  
  return 0;

}


#include <stdio.h>

// states
typedef enum {
	ST_IDLE,
	ST_WATER,
	ST_WASHING,
	ST_RINSING,
	ST_SPINNING,
	ST_PAUSED
} state_type;

// possible events
typedef enum {
	EV_NONE,
	EV_PAUSE,
	EV_PLAY,
	EV_TIMEOUT
} event_type

// state machine
typedef struct {
	state_type current_state;
}

// Function prototypes



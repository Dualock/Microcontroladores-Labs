#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

// Definición de los estados
enum States {
  ST_IDLE,
  ST_CLOSED,
  ST_OPENNING,
  ST_OPENED,
  ST_CLOSING,
  ST_STOPPED
};

enum Events {
  EV_NONE,
  EV_DONE,
  EV_OPEN,
  EV_CLOSE,
};
void setup_fsm();

int eventHandler(States &currentState, Events &event, int current_pos);

int open_gate(int current_position);

int close_gate(int current_position);

#endif 

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

void initStateMachine(States &currentState, Events &event);
void eventHandler(States &currentState, Events &event);

#endif 

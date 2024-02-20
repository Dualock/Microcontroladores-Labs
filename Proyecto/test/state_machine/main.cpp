#include "state_machine.h"
#include <iostream>

States currentState;
Events event;

int main() {
    initStateMachine(currentState, event);
    eventHandler(currentState, event);
    eventHandler(currentState, event);
    eventHandler(currentState, event);
    event = EV_DONE;
    eventHandler(currentState, event);
    eventHandler(currentState, event);
    event = EV_OPEN;
    eventHandler(currentState, event);
    eventHandler(currentState, event);
    event = EV_CLOSE;
    eventHandler(currentState, event);
    eventHandler(currentState, event);
    event = EV_OPEN;
    eventHandler(currentState, event);
    eventHandler(currentState, event);
    event = EV_DONE;
    eventHandler(currentState, event);
    eventHandler(currentState, event);
    event = EV_CLOSE;
    eventHandler(currentState, event);
    event = EV_OPEN;
    eventHandler(currentState, event);
    event = EV_OPEN;
    eventHandler(currentState, event);
    eventHandler(currentState, event);
    event = EV_CLOSE;
    eventHandler(currentState, event);
    event = EV_OPEN;
    eventHandler(currentState, event);
    event = EV_DONE;
    eventHandler(currentState, event);
    eventHandler(currentState, event);
    return 0;
}


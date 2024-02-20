//#include "Arduino.h"
#include "state_machine.h"
//#include <Servo.h>
#include <iostream>

// Definicion de Pines

// Función para manejar eventos en la FSM
void initStateMachine(States &currentState, Events &event){
  currentState = ST_IDLE;
  event = EV_NONE;
  }
void eventHandler(States &currentState, Events &event) {
  //Serial.begin("Event handler");
  switch (currentState) {
    case ST_IDLE:
      //Serial.println("ST_IDLE");
      std::cout << "ST_IDLE" <<  std::endl;
      currentState = ST_CLOSING;
      break;
      
    case ST_CLOSED:
      //Serial.println("ST_CLOSED");
      std::cout << "ST_CLOSED" <<  std::endl;

      if(event == EV_OPEN){
        currentState = ST_OPENNING;
        }
      break;
      
    case ST_OPENNING:
      //Serial.println("ST_OPENNING");
      std::cout << "ST_OPENNING" <<  std::endl;
      if(event == EV_DONE){
        currentState = ST_OPENED;
        }
      if(event == EV_CLOSE){
        currentState = ST_STOPPED;
          }
      break;
      
    case ST_OPENED:
      //Serial.println("ST_OPENED");
      std::cout << "ST_OPENED" <<  std::endl;
      if(event == EV_CLOSE){
        currentState = ST_CLOSING;
        }
      break;

    case ST_CLOSING:
      //Serial.println("ST_CLOSING");
      std::cout << "ST_CLOSING" <<  std::endl;
      if(event == EV_DONE){
        currentState = ST_CLOSED;
        }
      if(event == EV_OPEN){
        currentState = ST_STOPPED;
          }
      break;
      
    case ST_STOPPED:
      //Serial.println("ST_STOPPED");
      std::cout << "ST_STOPPED" <<  std::endl;
      if(event == EV_OPEN){
        currentState = ST_OPENNING;
        }
      if(event == EV_CLOSE){
        currentState = ST_CLOSING;
          }
      break;
    default:
    	std::cout << "Error: Not handled state" << std::endl;
  }
}

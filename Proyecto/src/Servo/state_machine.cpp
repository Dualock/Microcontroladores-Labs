#include "Arduino.h"
#include "state_machine.h"
#include <Servo.h>

// Definicion de Pines
int led_closed = 9;
int led_open = 8;
int position = 0;
Servo myservo;

// FSM
void setup_fsm(){
  myservo.attach(11);
  pinMode(led_closed, OUTPUT);
  pinMode(led_open, OUTPUT);
  }
  
// Función para manejar eventos en la FSM
int eventHandler(States &currentState, Events &event, int current_position) {
  switch (currentState) {
    case ST_IDLE:
      Serial.println("ST_IDLE");
      currentState = ST_CLOSING;
      break;
      
    case ST_CLOSED:
      Serial.println("ST_CLOSED");
      digitalWrite(led_closed, HIGH);
      digitalWrite(led_open, LOW);
      if(event == EV_OPEN){
        currentState = ST_OPENNING;
        }
      break;
      
    case ST_OPENNING:
      Serial.println("ST_OPENNING");
      current_position = open_gate(current_position);
      if(event == EV_DONE){
        currentState = ST_OPENED;
        }
      else if(event == EV_CLOSE){
        currentState = ST_STOPPED;
          }
      break;
      
    case ST_OPENED:
      Serial.println("ST_OPENED");
      
      digitalWrite(led_open, HIGH);
      digitalWrite(led_closed, LOW);
      
      if(event == EV_CLOSE){
        currentState = ST_CLOSING;
        }
      break;

    case ST_CLOSING:
      Serial.println("ST_CLOSING");
      current_position = close_gate(current_position);
      if(event == EV_DONE){
        currentState = ST_CLOSED;
        }
      else if(event == EV_OPEN){
        currentState = ST_STOPPED;
          }
      break;
      
    case ST_STOPPED:
      Serial.println("ST_STOPPED");
      if(event == EV_OPEN){
        currentState = ST_OPENNING;
        }
      else if(event == EV_CLOSE){
        currentState = ST_CLOSING;
          }
      break;
  }
  return current_position;
}

// Funcion para abrir la compuerta
int open_gate(int current_position){
  while(current_position > 0) {
    current_position--;
    myservo.write(current_position);
    delay(20);                 // Se mueve 1 grado por cada 20ms
  }
  return current_position;
}

// Funcion para cerrar la compuerta
int close_gate(int current_position){
  position = current_position;
  while(current_position < 180) {
    current_position++;
    myservo.write(current_position);
    delay(20);               // Se mueve 1 grado por cada 20ms
  }
  return current_position;
}

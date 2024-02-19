
#include <Servo.h>
#include "state_machine.h"

// Definicion de Pines
int led_pin = 13; // LED naranja del arduino NANO BLE33
int openclose_btn = 3;
int action = 0; // 0 para cerrar, 1 para abrir

//
States currentState = ST_IDLE;
Events event = EV_NONE;

// Prototipos de funciones

// Variables globales
static int current_pos = 180;
void setup() {
  void setup_fsm();
  pinMode(openclose_btn, INPUT);
  attachInterrupt(digitalPinToInterrupt(openclose_btn), openclose_state, FALLING);
  eventHandler(currentState, event, current_pos);
}
// Interrupcion para generar eventos de apertura y cierre
void openclose_state(){
  if(action == 0){
    action = 1;
    }
  else{
    action = 0;
    }
  }


void loop() {
  if(action == 0){
   event = EV_CLOSE;
   current_pos = eventHandler(currentState, event, current_pos);
  }
  else if(action == 1){
    event = EV_CLOSE;
    current_pos = eventHandler(currentState, event, current_pos);
    }
  digitalWrite(led_pin, HIGH);
  delay(500);
  digitalWrite(led_pin, HIGH);
  delay(500);
}

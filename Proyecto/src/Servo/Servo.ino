
#include <Servo.h>
#include "state_machine.h"

int led_pin = 13; // LED naranja del arduino NANO BLE33
int led_closed = 9;
int led_open = 8;
int openclose_btn = 3;
int position = 0;
int action = 0; // 0 para cerrar, 1 para abrir
int RED_LED_TEST = 7;
Servo myservo;
// Prototipos de funciones

void setup() {
  myservo.attach(11);
  pinMode(led_pin, OUTPUT);
  pinMode(led_closed, OUTPUT);
  pinMode(led_open, OUTPUT);
  pinMode(RED_LED_TEST, OUTPUT);
  pinMode(openclose_btn, INPUT);
  attachInterrupt(digitalPinToInterrupt(openclose_btn), openclose_state, FALLING);
}
void openclose_state(){
  if(action == 0){
    action = 1;
    }
  else{
    action = 0;
    }
  }
int open_gate(int current_position){
  while(current_position > 0) {
    current_position--;
    myservo.write(current_position);
    delay(20);                       // Se mueve 1 grado por cada 20ms
  }
  return current_position;
}

int close_gate(int current_position){
  position = current_position;
  while(current_position < 180) {
    current_position++;
    myservo.write(current_position);
    delay(20);
  }
  return current_position;
}
  
void loop() {
  int current_pos = 180;
  setup();
  while(1){
    if(action == 0){
      current_pos = close_gate(current_pos);
    }
    else if(action == 1){
      current_pos = open_gate(current_pos);
      }
      // Encender LED de abierto
    if(current_pos == 0){
      digitalWrite(led_open, HIGH);
      digitalWrite(led_closed, LOW);
      }
    // Encender LED de cerrado
    else if(current_pos == 180){
      digitalWrite(led_closed, HIGH);
      digitalWrite(led_open, LOW);
      }
      Serial.println(action);
      blink(RED_LED_TEST);
      delay(100);
    }
    

}

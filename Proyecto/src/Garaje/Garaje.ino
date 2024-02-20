
#include <Servo.h>
#include "state_machine.h"
#include "compuerta.h"
// Definicion de Pines

// Salidas
int led_pin = LED_BUILTIN; // LED naranja del arduino NANO BLE33
int led_closed = 9;
int led_open = 8;
int control_pin = 11;

//Entradas
int open_btn = 2;
int close_btn = 3;


// Objeto Servo
Servo myservo;

// Estados y eventos de la FSM
States currentState;
Events event;

// Prototipos de funciones
void open_state();
void close_state();

// Variables globales
static int bluetooth_open_command = 0; // 1 para abrir
static int bluetooth_close_command = 1; // 1 para cerrar
int current_pos = 90;

// Funcion setup
void setup() {
  Serial.begin(9600);
  myservo.attach(control_pin);
  pinMode(led_closed, OUTPUT);
  pinMode(led_open, OUTPUT);
  
  // Funciones para interrupciones externas
  pinMode(open_btn, INPUT);
  pinMode(close_btn, INPUT);
  attachInterrupt(digitalPinToInterrupt(open_btn), open_state, FALLING);
  attachInterrupt(digitalPinToInterrupt(close_btn), close_state, FALLING);
  initStateMachine(currentState, event);
}
// Interrupciones para generar eventos de apertura y cierre
void open_state(){
  Serial.print("Interrupcion Externa Abrir: ");
  bluetooth_open_command = 1; // 1 para abrir
  bluetooth_close_command = 0; // 1 para cerrar
}
void close_state(){
  Serial.print("Interrupcion Externa Cerrar: ");
  bluetooth_open_command = 0; // 1 para abrir
  bluetooth_close_command = 1; // 1 para cerrar
}


void loop() {
  Serial.println("looping");
  // Posicion 180 grados, cierra
  eventHandler(currentState, event);
  if(bluetooth_close_command){
    if(current_pos < 180){
      current_pos = closeGate(myservo, current_pos);
      event = EV_CLOSE;
      }
    if(current_pos == 180){
      event = EV_DONE;
      }
    }
  // Posicion 0 grados, cierra
  if(bluetooth_open_command){
    
    if(current_pos == 0){
      event = EV_DONE;
    }
    if(current_pos>0){
      event = EV_OPEN;
      current_pos = openGate(myservo, current_pos);
    }
  }
  Serial.println(current_pos);
  if(current_pos == 180){
  digitalWrite(led_closed, HIGH);
  digitalWrite(led_open, LOW);
  }
  if(current_pos == 0){
  digitalWrite(led_closed, LOW);
  digitalWrite(led_open, HIGH);
  }
  delay(500);
}

/*

#include <Servo.h>
#include "compuerta.h"

int led = LED_BUILTIN;
int control_pin = 11;

Servo myservo;
void setup() {
  Serial.begin(9600);
  myservo.attach(control_pin);
  pinMode(led, OUTPUT);
  
}

void loop() {
  static int current_pos = 90;
  Serial.println(current_pos);
  current_pos = closeGate(myservo, current_pos);
  delay(1000);
  current_pos = openGate(myservo, current_pos);
  current_pos = hardWriteValue(myservo, current_pos, 120);
}
*/

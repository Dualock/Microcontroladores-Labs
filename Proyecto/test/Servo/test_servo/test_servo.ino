
#include <Servo.h>
#include "compuerta.h"

// Salidas
int led = LED_BUILTIN;
int control_pin = 11;

// Entradas
int bit0 = 4;
int bit1 = 5;
int bit2 = 6;

// Variables globales
int estado = -1;
int ultimo_estado = -1;
bool ready = false;

// Servo
Servo myservo;

// Prototipo de funciones
int maquina_estados(int posicion_servo);
int obtener_estado(); // Cambiar esta funcion por una que obtenga el estado por bluetooth
int default_close(int posicion_servo);


void setup() {
  // configuracion de perifericos 
  Serial.begin(9600);
  myservo.attach(control_pin);

  // Configuracion de salidas
  pinMode(led, OUTPUT);

  //Configuracion de entradas
  pinMode(bit0, INPUT);
  pinMode(bit1, INPUT);
  pinMode(bit2, INPUT);
}

void loop() {
  static int posicion_servo = 150;
  Serial.println("Cerrando por default al iniciar");
  while(ready == false){
    posicion_servo = default_close(posicion_servo);
    delay(100);
    }
  Serial.println("Listo!");
  while(ready == true){
    estado = obtener_estado();
    Serial.print("estado: ");
    Serial.println(estado);
    Serial.println();
    posicion_servo = maquina_estados(posicion_servo);
    delay(100);
    }
    
}
int default_close(int posicion_servo){
  posicion_servo = closeGate(myservo, posicion_servo, led);
  if(posicion_servo == CLOSED_VALUE){
    ready = true;
    }
    return posicion_servo;
  }
int maquina_estados(int posicion_servo){
  switch(estado){
    case 1:
      Serial.println("Abrir: ");
      posicion_servo = openGate(myservo, posicion_servo, led);
      ultimo_estado = 1;
      break;
    case 2:
      Serial.println("Cerrar: ");
      posicion_servo = closeGate(myservo, posicion_servo, led);
      ultimo_estado = 2;
      break;
    case 3:
      Serial.println("Continuar: ");
      estado = ultimo_estado;
      break;
    case 4:
      Serial.println("Pausa: ");
      break;
    default:
      Serial.println("Estado no identificado");
    }
    return posicion_servo;
  }
int obtener_estado(){
  if(digitalRead(bit0) == HIGH && digitalRead(bit1) == LOW && digitalRead(bit2) == LOW){
    return 1;
    }
  if(digitalRead(bit0) == LOW && digitalRead(bit1) == HIGH && digitalRead(bit2) == LOW){
    return 2;
    }
  if(digitalRead(bit0) == HIGH && digitalRead(bit1) == HIGH && digitalRead(bit2) == LOW){
    return ultimo_estado;
    }
  if(digitalRead(bit0) == LOW && digitalRead(bit1) == LOW && digitalRead(bit2) == HIGH){
    return 4;
    }
  else{
    return -1;
    }
  }

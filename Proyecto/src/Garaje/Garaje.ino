
#include <Servo.h>
#include <ArduinoBLE.h>
// Includes locales
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

//BLE
BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service
// create switch characteristic and allow remote device to read and write
BLEIntCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

// Prototipo de funciones
int maquina_estados(int posicion_servo); // Logica de estados simple
int get_bluetooth(); // Obtiene el estado por medio de BLE
int default_close(int posicion_servo); // funcion para cerrar el porton como posicion inicial

void ble_setup(){
  Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("TestLED");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(ledService);

  // add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);

  // add the service
  BLE.addService(ledService);

  ledCharacteristic.writeValue(0);

  // start advertising
   BLE.advertise();

  Serial.println("Bluetooth® device active, waiting for connections...");
  }

void setup() {
  // configuracion de perifericos
  ble_setup();
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
    estado = get_bluetooth();
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
    case 12592:
      posicion_servo = openGate(myservo, posicion_servo, led);
      Serial.println("Recibiendo por Bluetooth Estado 1: Abrir");
      ultimo_estado = 12592;
      break;
    case 12848:
      posicion_servo = closeGate(myservo, posicion_servo, led);
      Serial.println("Recibiendo por Bluetooth Estado 2: Cerrar");
      ultimo_estado = 12848;
      break;
    case 13104:
      estado = ultimo_estado;
      Serial.println("Recibiendo por Bluetooth Estado 3: Continuar");
      break;
    case 52:
      Serial.println("Recibiendo por Bluetooth Estado 4: Pausa");
      break;  
    default:
      Serial.println("Estado no identificado");
    }
    return posicion_servo;
  }
int get_bluetooth() {
  // poll for Bluetooth® Low Energy events
  BLE.poll();
  //Serial.println(estado);
  Serial.println(ledCharacteristic.value());
  if (ledCharacteristic.written()) {
    // update LED, either central has written to characteristic or button state has changed
    estado = ledCharacteristic.value(); // Guarda el comando obtenido
  }
  delay(500);
  return estado;
}

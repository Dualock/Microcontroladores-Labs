/*
  Button LED

  This example creates a Bluetooth® Low Energy peripheral with service that contains a
  characteristic to control an LED and another characteristic that
  represents the state of the button.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  - Button connected to pin 4

  You can use a generic Bluetooth® Low Energy central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

const int ledPin = LED_BUILTIN; // set ledPin to on-board LED
const int buttonPin = 4; // set buttonPin to digital pin 4
static int estado = -1;
BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEIntCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
// create button characteristic and allow remote device to get notifications
//BLEByteCharacteristic buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(ledPin, OUTPUT); // use the LED as an output

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

int get_bluetooth() {
  // poll for Bluetooth® Low Energy events
  BLE.poll();
  //Serial.println(estado);
  Serial.println(ledCharacteristic.value());
  if (ledCharacteristic.written()) {
    // update LED, either central has written to characteristic or button state has changed
    estado = ledCharacteristic.value(); // Guarda el comando obtenido
    
    if (estado == 12592) {
      digitalWrite(ledPin, HIGH);
      Serial.println("Recibiendo por Bluetooth Estado 1: Abrir");
    } 
    if (estado == 12848){
      digitalWrite(ledPin, LOW);
      Serial.println("Recibiendo por Bluetooth Estado 2: Cerrar");
    }
    if(estado == 13104){
      Serial.println("Recibiendo por Bluetooth Estado 3: Continuar");
      }
    if(estado == 52){
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin, LOW);
      Serial.println("Recibiendo por Bluetooth Estado 4: Pausa");
      }
  }
  delay(500);
  return estado;
}
void loop(){
  estado = get_bluetooth();
  Serial.print("Estado obtenido: ");
  Serial.println(estado);
  }
/*void deviceConnected(BLEDevice central) {
  Serial.print("Connected to central: ");
  Serial.println(central.address());
}*/

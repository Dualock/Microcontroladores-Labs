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
byte command;

BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
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

void loop() {
  // poll for Bluetooth® Low Energy events
  BLE.poll();
  Serial.println(command);
  Serial.println(ledCharacteristic.value());
  if (ledCharacteristic.written()) {
    // update LED, either central has written to characteristic or button state has changed
    command = ledCharacteristic.value(); // Guarda el comando obtenido
    if (command) {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED ON");
    } else{
      digitalWrite(ledPin, LOW);
      Serial.println("LED OFF");
    }
  }
  delay(500);
}
void deviceConnected(BLEDevice central) {
  Serial.print("Connected to central: ");
  Serial.println(central.address());
}

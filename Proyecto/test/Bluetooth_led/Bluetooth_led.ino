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

// Salidas
const int ledPin = LED_BUILTIN; // set ledPin to on-board LED

// Variables globales
static int estado = -1;
int ultimo_estado = -1;

BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEIntCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
// create button characteristic and allow remote device to get notifications
//BLEByteCharacteristic buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

// Function prototypes
int maquina_estados(int posicion_servo);
int get_bluetooth();

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
  }
  delay(500);
  return estado;
}
void loop(){
  static int posicion_servo = 150;
  estado = get_bluetooth();
  posicion_servo = maquina_estados(posicion_servo);
  delay(500);
  }

int maquina_estados(int posicion_servo){
  switch(estado){
    case 12592:
      //posicion_servo = openGate(myservo, posicion_servo, led);
      digitalWrite(ledPin, HIGH);
      Serial.println("Recibiendo por Bluetooth Estado 1: Abrir");
      ultimo_estado = 12592;
      break;
    case 12848:
      //posicion_servo = closeGate(myservo, posicion_servo, led);
      digitalWrite(ledPin, LOW);
      Serial.println("Recibiendo por Bluetooth Estado 2: Cerrar");
      ultimo_estado = 12848;
      break;
    case 13104:
      //estado = ultimo_estado;
      Serial.println("Recibiendo por Bluetooth Estado 3: Continuar");
      break;
    case 52:
      Serial.println("Recibiendo por Bluetooth Estado 4: Pausa");
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin, LOW);
      break;  
    default:
      Serial.println("Estado no identificado");
    }
    return posicion_servo;
  }
/*void deviceConnected(BLEDevice central) {
  Serial.print("Connected to central: ");
  Serial.println(central.address());
}*/

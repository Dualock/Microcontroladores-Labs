/* Arduino Control */
#include <ArduinoBLE.h>
#include <Servo.h> 
#define RED 22     
#define BLUE 24     
#define GREEN 23
#define LED_PWR 25
Servo servo1;         // Crea el servo1 con las librerias de Servo.h
BLEService ServicioLED ("180A");
BLEByteCharacteristic Color("2A57", BLERead | BLEWrite);

int estado=0;
int retardo=100;
int analogo=0;

void setup(){
  servo1.attach(4);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(LED_PWR, OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);//normalmente 13
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(3,OUTPUT);
  Serial.begin(9600);
  delay(1000);
  digitalWrite(RED,HIGH);
  digitalWrite(GREEN,HIGH);
  digitalWrite(BLUE,HIGH);
  digitalWrite(LED_PWR,HIGH);
  if (!BLE.begin()) {

    Serial.println("* Starting Bluetooth® Low Energy module failed!");

    while (1);

  }

  
  BLE.setLocalName("Nano 33 BLE (Central)"); 
  
  BLE.setAdvertisedService (ServicioLED);
  ServicioLED.addCharacteristic(Color);

  Serial.println(" ");
  BLE.addService(ServicioLED);
  Color.writeValue(0);
  BLE.advertise(); //Servicio ISM
  Serial.println("Arduino Nano 33 BLE Sense (Central Device)");
 }



 void loop(){
  BLEDevice central = BLE.central();
  Serial.println("- Discovering central device...");

  if (central) {
    Serial.println("* Peripheral device found!");
    Serial.print("* Device MAC address: ");
    Serial.println(central.address());



  delay(500);

while(central.connected()){
  if (Color.written()){
 if(Serial.available()>0){           // Si el puerto serie esta habilitadp 
        estado = Serial.read();      // Lee lo que llega por el puerto Serie
 }
     
 if(estado== '1'){                    // on/off de los pin 13 al pin 6
       digitalWrite(RED,LOW);
 }
 if(estado== '2' ){
       digitalWrite(RED,HIGH);
 }
 if(estado== 'Pausar'){ 
       digitalWrite(GREEN,LOW);
 }
 if(estado== 'Continuar' ){
       digitalWrite(GREEN,HIGH);
 }
 /*if(estado== 'A' ){      // Movimiento del SERVO
      servo1.write(0);  
      }
 if(estado== 'B' ){
      servo1.write(90); 
      }
 if(estado== 'C' ){
      servo1.write(180);  
      }
 */
 
      
if(estado=='q'){         // envia el valor leido del puerto analogo A0
  analogo=analogRead(A0);
  Serial.print(analogo);
  Serial.println("∞C");
  delay (retardo);
  estado=0;
 }
 delay(retardo);
 }}
 Serial.println(central.address());
 digitalWrite(LED_BUILTIN, LOW);
 digitalWrite(RED, HIGH);
 digitalWrite(RED, HIGH);
 digitalWrite(RED, HIGH);}
 }
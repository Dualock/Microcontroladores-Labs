
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
  delay(1000);
  // current_pos = hardWriteValue(myservo, current_pos, 120);
}

#ifndef COMPUERTA_H
#define COMPUERTA_H
#pragma once

#include <Servo.h>
#include "Arduino.h"

#define OPEN_VALUE 90
#define CLOSED_VALUE 180

// Prototipos de funciones
int closeGate(Servo &myservo, int current_pos, int LED);
int openGate(Servo &myservo, int current_pos, int LED);
int hardWriteValue(Servo &myservo, int current_pos, int val);

int closeGate(Servo &myservo, int current_pos, int LED)
{
  if(current_pos < CLOSED_VALUE){
    current_pos++;
    }
  if(current_pos >= CLOSED_VALUE){
    current_pos = CLOSED_VALUE;
    digitalWrite(LED, HIGH);
    }
    Serial.println(current_pos);
    myservo.write(current_pos);
    return current_pos;
}

int openGate(Servo &myservo, int current_pos, int LED)
{
  Serial.println(current_pos);
  if(current_pos == OPEN_VALUE){
    digitalWrite(LED, LOW);
    }
   else if(current_pos > OPEN_VALUE){
    current_pos--;
    }
    myservo.write(current_pos);
    return current_pos;
}

int hardWriteValue(Servo &myservo, int current_pos, int val){
    while(current_pos != val){
    Serial.println(current_pos);
    myservo.write(current_pos);
    if(val > current_pos){
      current_pos++;
      }
    else if(val < current_pos){
      current_pos--;
      }
    
    delay(20);
    }
    return current_pos;
}
#endif
